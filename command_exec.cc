#include "command_exec.h"

extern pid_t shell_pgid;
extern struct termios shell_tmodes;
extern int shell_terminal;
extern int shell_is_interactive;

void exec_job(Job &job)
{
	auto c = job.commands.front(); // if c is not external command, then command_list only has one element.
	c.check_type();
	switch(c.type){
		case EXIT :
			exit(0);
			break;
		case CD :
		{
			std::string dir_name = c.parameters.front().content; // cd always has one parameter_len
			int cd_status = chdir(dir_name.c_str());
			if (cd_status < 0)
				perror("cd");
			break;
		}
		case JOBS :
			printf("sorry, I have no job control\n");
			break;
		case FG :
			printf("sorry, I have no job control\n");
			break;
		case BG :
			printf("sorry, I have no job control\n");
			break;
		case EMPTY: //empty, just skip
			break;
		default : // execute external commands
			exec_piped_commands(job);
	}
}


void exec_piped_commands(Job &job)
{
	int number_of_pipes = (job.commands.size() - 1);  
	int fds[2*number_of_pipes];
	
	// create all pipes
	for (int i = 0; i < number_of_pipes; i++){
		if ( pipe(fds + i*2) < 0){
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	unsigned int i = 0;
	for (auto iter = job.commands.begin(); iter != job.commands.end(); iter++, i++){
		pid_t pid = fork();
		if (pid == 0){
			//child 
			
			if (i != 0){ // not the first command in the list
				close(STDIN_FILENO);
				dup2(fds[(i-1)*2], STDIN_FILENO);
			}

			if (i != job.commands.size() -1 ){ // not the last command in the list
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
				args[index] = (char*)iter2->content.c_str();			
			}
			
			args[index] = NULL; // the last one should be NULL
			
			// if (shell_is_interactive){
			// 	pid = getpid();
			// 	if (!job.is_bg)
			// 		tcsetpgrp(shell_terminal, pid);
			// 	//the child would inherit the signal control of parent, should reset it
			// 	signal (SIGINT, SIG_DFL);
			//     signal (SIGQUIT, SIG_DFL);
			//     signal (SIGTSTP, SIG_DFL);
			//     signal (SIGTTIN, SIG_DFL);
			//     signal (SIGTTOU, SIG_DFL);
			//     signal (SIGCHLD, SIG_DFL);
			// }

			if (execvp(iter->name.c_str(), args) < 0 ){ 
				// cleanup if failed !
				perror(iter->name.c_str());
				exit(EXIT_FAILURE);
			}
		}
		
		else if (pid < 0){
			perror("fork");
		}
	}

	for(int i = 0; i < 2*number_of_pipes; i++){ // close all pipes in parent
        close(fds[i]);
    }

    // if (!shell_is_interactive)
    	

	int status;
    for(size_t i = 0; i < job.commands.size(); i++){
		if (!job.is_bg) // only wait if the job is a foreground job
        	wait(&status);
        // if(WIFEXITED(status))
        // 	printf("child exited with = %d\n", WEXITSTATUS(status));
    }
}
