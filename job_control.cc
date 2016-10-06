#include "job_control.h"

extern pid_t shell_pgid;
extern struct termios shell_tmodes;
extern int shell_terminal;
extern int shell_is_interactive;

extern Job* first_job;

void wait_for_job(Job *job)
{
	int status;
	pid_t pid;
	
	do{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
        if (pid < 0)
            perror("wait for job ");
        printf("the pid returned is %d\n", (int)pid);
    } while( !mark_process_status(pid, status) && !job->is_stopped() && !job->is_completed());;
}


void put_job_foreground(Job* job)
{
    tcsetpgrp(shell_terminal, job->pgid); //let this job control the terminal
    
    wait_for_job(job);
    
    printf("return the terminal to shell\n");
    tcsetpgrp(shell_terminal, shell_pgid);
    
    tcgetattr(shell_terminal, &job->tmodes);
    tcsetattr(shell_terminal, TCSADRAIN, &shell_tmodes);
    
}

int mark_process_status(pid_t pid, int status)
{
    if (pid > 0){ // if pid is 0, no child has changed status, no need for check
        for (auto j  = first_job; j; j = j->next )
            for (auto iter_process = j->commands.begin(); iter_process!=j->commands.end(); iter_process++){
                printf("process %d\n", (int)iter_process->pid);
                if (iter_process->pid == pid){
                    // find the process whose status changed
                    printf("find process %d\n", (int)pid);
                    iter_process->status = status;
                    if (WIFSTOPPED(status))
                    {
                        iter_process->stopped = true;
                        printf("set stopped\n");
                    }
                    else{
                        iter_process->completed = true;
                        if (WIFSIGNALED(status))
                            fprintf(stderr, "%d: Terminated by signal %d. \n",
                            (int)pid, WTERMSIG(iter_process->status));
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