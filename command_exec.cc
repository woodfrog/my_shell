#include "command_exec.h"

extern pid_t shell_pgid;
extern struct termios shell_tmodes;
extern int shell_terminal;
extern int shell_is_interactive;

extern Job* first_job;

void exec_command(Job *job)
{
	auto c = job->commands.front(); // if c is not external command, then command_list only has one element.
	c.check_type();
	switch(c.type){
		case EXIT :
			exit(0);
			break;
		case CD :
		{
			std::string dir_name = c.parameters.front(); // cd always has one parameter_len
			int cd_status = chdir(dir_name.c_str());
			if (cd_status < 0)
				perror("cd");
			break;
		}
		case JOBS :
			print_job_info();
			break;
		case FG :
			if (!first_job)
				fprintf(stderr, "fg: no job can be put into foreground\n");
			else if (c.parameter_len() == 0) // no specified job, and there is a job  
				foreground_continue_job(first_job);
			else{
				std::string pgid_str = c.parameters.front();
				pid_t pgid = atoi(pgid_str.c_str());
				Job *job = find_job(pgid);
				if (job)		
					foreground_continue_job(job);
				else
					fprintf(stderr, "fg: no job can be put into foreground\n");
			}
			break;
		case BG :
			if (!first_job)
				fprintf(stderr, "bg: no such job\n");
			else if (c.parameter_len() == 0) // no specified job, and there is a job  
				background_continue_job(first_job);
			else{
				std::string pgid_str = c.parameters.front();
				pid_t pgid = atoi(pgid_str.c_str());
				Job *job = find_job(pgid);
				if (job)		
					background_continue_job(job);
				else
					fprintf(stderr, "bg: no such job\n");
			}
			break;
		case EMPTY: //empty, just skip
			break;
		default : // execute external commands (the true job)
			exec_job(job);
	}
}


void exec_job(Job *job)
{
	int number_of_pipes = (job->commands.size() - 1);  
	int fds[2*number_of_pipes];

	job->next = first_job; // insert the job at the head of the list
	first_job = job;	
	
	// create all pipes
	for (int i = 0; i < number_of_pipes; i++){
		if ( pipe(fds + i*2) < 0){
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	unsigned int i = 0;
	for (auto iter = job->commands.begin(); iter != job->commands.end(); iter++, i++){
		pid_t pid = fork();
		if (pid == 0){
			//child 
			
			if (i != 0){ // not the first command in the list
				close(STDIN_FILENO);
				dup2(fds[(i-1)*2], STDIN_FILENO);
			}

			if (i != job->commands.size() -1 ){ // not the last command in the list
				close(STDOUT_FILENO);
				dup2(fds[i*2+1], STDOUT_FILENO);
			}

			for(int i = 0; i < 2*number_of_pipes; i++){ // close all pipes in child
        		close(fds[i]);
    		}
			
			char *args[ 2 + iter->parameter_len()]; // one for command name, one for NULL
			args[0] = (char*)iter->name.c_str();
			size_t index = 1;
			
			for(auto iter2 = iter->parameters.begin(); 
				iter2!=iter->parameters.end(); 
				iter2++, index++)
			{
				args[index] = (char*)(iter2->c_str());			
			}
			
			args[index] = NULL; // the last one should be NULL
			
			if (shell_is_interactive){
				pid_t pid = getpid();
				if (job->pgid == 0) // set the first process's pid as the group's pgid
					job->pgid = pid;
				setpgid(pid, job->pgid); // put the process specified by pid into the group specified by pgid
				
				if (!job->is_bg)
					tcsetpgrp(shell_terminal, pid);
					
				//the child would inherit the signal control of parent, should reset it
				signal (SIGINT, SIG_DFL);
			    signal (SIGQUIT, SIG_DFL);
			    signal (SIGTSTP, SIG_DFL);
			    signal (SIGTTIN, SIG_DFL);
			    signal (SIGTTOU, SIG_DFL);
			    signal (SIGCHLD, SIG_DFL);
			}

			if (execvp(iter->name.c_str(), args) < 0 ){ 
				perror(iter->name.c_str());
				exit(EXIT_FAILURE);
			}
		}
		
		else if (pid < 0){
			perror("fork");
		}
		else{
			/* in parent process */
			
			iter->pid = pid; // set the pid for each forked processes in parent process
			if (shell_is_interactive){
				if (job->pgid == 0)
					job->pgid = pid;
				setpgid(pid, job->pgid); // shell also put the process specified by pid into the correct group
			}
		}
	}
	
	
	for(int i = 0; i < 2*number_of_pipes; i++){ // close all pipes in parent
        close(fds[i]);
    }

	    if (!shell_is_interactive)
		wait_for_job(job);
	else if (!job->is_bg)
		put_job_foreground(job);
		// do nothing, if in background, we just don't wait this job and continue the parent.

}


