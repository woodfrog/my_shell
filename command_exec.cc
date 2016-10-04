#include "command_exec.h"

void exec_commands(std::list<Command> &command_list)
{
	auto c = command_list.front(); // if c is not external command, then command_list only has one element.
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
				perror("chdir");
			break;
		}
		case JOBS :
			break;
		case FG :
			break;
		case BG :
			break;
		case EMPTY: //empty, just skip
			break;
		default : // execute external commands
			exec_piped_commands(command_list);
	}
}


void exec_piped_commands(std::list<Command> &command_list)
{
	int number_of_pipes = (command_list.size() - 1);  
	int fds[2*number_of_pipes];
	
	// create all pipes
	for (int i = 0; i < number_of_pipes; i++){
		if ( pipe(fds + i*2) < 0){
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	unsigned int i = 0;
	for (auto iter = command_list.begin(); iter != command_list.end(); iter++, i++){
		pid_t pid = fork();
		if (pid == 0){
			//child 
			
			if (i != 0){ // not the first command in the list
				close(STDIN_FILENO);
				dup2(fds[(i-1)*2], STDIN_FILENO);
			}

			if (i != command_list.size() -1 ){ // not the last command in the list
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
			
			if (execvp(iter->name.c_str(), args) < 0 ){ 
				// cleanup if failed !
				perror("child");
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

	int status;
    for(size_t i = 0; i < command_list.size(); i++){
        wait(&status);
        // if(WIFEXITED(status))
        // 	printf("child exited with = %d\n", WEXITSTATUS(status));
    }
}
