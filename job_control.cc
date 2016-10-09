#include "job_control.h"

extern pid_t shell_pgid;
extern struct termios shell_tmodes;
extern int shell_terminal;
extern int shell_is_interactive;

extern Job* first_job;

Job* find_job(pid_t pgid)
{
    for (Job *j = first_job; j; j = j->next)
        if (j->pgid == pgid)
            return j;
    return NULL;
}

void update_status()
{
    int status;
    pid_t pid;
    
    do{
        /* choose the option WNOHANG | WUNTRACED since we only need check the status of 
            children processes but don't need to wait for them to change state */
        pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
    }while (!set_process_status(pid, status));
}

void wait_for_job(Job *job)
{
	int status;
	pid_t pid;
	
	do{
        /* choose the option WUNTRACED since we need to wait until some child's status
            changed and then we deal with the change by calling mark_process_status */
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
        if (pid < 0)
            perror("wait for job ");
    } while( !set_process_status(pid, status) && !job->is_stopped() && !job->is_completed());;
}


int set_process_status(pid_t pid, int status)
{
    if (pid > 0){ // if pid is 0, no child has changed status, no need for check
        for (auto j  = first_job; j; j = j->next )
            for (auto iter_process = j->commands.begin(); iter_process!=j->commands.end(); iter_process++){
                if (iter_process->pid == pid){
                    // find the specified process
                    iter_process->status = status;
                    if (WIFSTOPPED(status)){
                        iter_process->stopped = true;
                    }
                    else{
                        iter_process->completed = true;
                    }
                    return 0;                
                }
            }
        fprintf(stderr, "NO child process %d. \n", (int)pid);
        return -1;  
    }
    else
        return -1;
}


void put_job_foreground(Job* job)
{
    tcsetpgrp(shell_terminal, job->pgid); //let this job control the terminal
    
    wait_for_job(job);
    
    tcsetpgrp(shell_terminal, shell_pgid);
    
    tcgetattr(shell_terminal, &job->tmodes);
    tcsetattr(shell_terminal, TCSADRAIN, &shell_tmodes); // restore the terminal modes
    
}


void foreground_continue_job(Job *job)
{
    //first of all, set the stopped flag for each process in the group to be false
    for (auto iter = job->commands.begin(); iter != job->commands.end(); iter++)
        iter->stopped = false;
    job->is_notified = false;
    
    if (job->is_bg){ // is a background job
        job->is_bg = false;         // change it to a foreground job
    }
    
    tcsetpgrp(shell_terminal, job->pgid);
    
    if (job->status == "Stopped")
        if (kill( -job->pgid, SIGCONT) < 0) // send the signal for continue to the whole group
            perror("continue foreground");
    
    wait_for_job(job);
        
    // reset the shell terminal, put our shell back to the foreground
    tcsetpgrp(shell_terminal, shell_pgid);
}


void background_continue_job(Job *job)
{
    job->is_bg = true;
    for (auto iter = job->commands.begin(); iter != job->commands.end(); iter++)
        iter->stopped = false;
        
    job->is_notified = false;
    if (job->status == "Stopped"){
        if (kill( -job->pgid, SIGCONT) < 0) // send the signal for continue to the whole group    
            perror("continue background");
        job->status = "Running";
    }
}


void job_notification()
{
    // update status for those have changed
    update_status();
    
    Job *jnext, *jlast = NULL;
    
    
    for(Job* j = first_job; j; j = jnext){
        jnext = j->next;
        
        if(j->is_completed()){
            if (j->is_bg) // show the notification only when the completed job is on background
                std::cout << long(j->pgid) << "  Completed  "  <<  j->command_line << std::endl;
            
            /* delete it from the job list */
            if (jlast) // the deleted one is not the first in the list
               jlast->next = jnext;
            else // the deleted one is the first in the list
               first_job = jnext;
               
            delete j; // delete the job since it has completed
        }
        
        else if(j->is_stopped() && !j->is_notified){
            jlast = j;
            std::cout << long(j->pgid) << "  Stopped  "  <<  j->command_line << std::endl;
            j->status = "Stopped";
            j->is_notified = true;
        }
        else
            jlast = j;
    }
}

void print_job_info()
{
    for (Job *j = first_job; j; j = j->next){
        std::cout << long(j->pgid) << "  "  << j->status << "   " <<  j->command_line << std::endl;
    }
}