#include "command_exec.h"

void exec_command(Command c)
{
	c.check_type();
	switch(c.type){
		case EXIT :
			exit(0);
			break;
		case CD :

			break;
		case JOBS :
			break;
		case FG :
			break;
		case BG :
			break;
		case EMPTY:
			// std::cout << "hhh empty" << std::endl;
			break;
		default :
			pid_t pid;
			pid = fork();

			if (pid < 0){
				fprintf(stderr, "Fork Failed in exec_command");
			}

			else if (pid == 0){ // child process 
				char ** args = new char* [c.parameter_len() + 2]; // one for command name, one for NULL
				args[0] = new char[c.name.size()];
				strncpy(args[0], c.name.c_str(), c.name.size());
				size_t index = 1;
				for(auto iter = c.parameters.begin(); 
					iter!=c.parameters.end(); 
					iter++, index++)
				{
					args[index] = new char[(*iter).content.size()];
					strncpy(args[index], (*iter).content.c_str(), (*iter).content.size());
				}
				args[index] = NULL; // the last one should be NULL
				
				// std::cout << "i'm going to run " << c.name <<"size :" << c.name.size() << std::endl;
				execvp(c.name.c_str(), args);
				// cleanup if failed
				for(size_t i = 0; i < c.parameter_len(); i++)
					delete[] args[i];
				delete[] args;
				exit(errno);
			}

			else{ // parent process 
				int status;
				wait(&status); // wait for child to complete.
				printf("%s completed with status %d\n", c.name.c_str(), status);
			}
			



	}

}


void exec_piped_commands(const std::list<Command> &command_list)
{
	int number_of_pipes = 2* (command_list.size() - 1); 
	int fds[number_of_pipes];
	printf("number of pipes : %d\n", number_of_pipes);
	// create all pipes
	for (int i = 0; i< number_of_pipes; i++){
		if ( pipe(fds + i*2) < 0){
			perror("create pipe failed");
			exit(EXIT_FAILURE);
		}
	}

	int i = 0;
	for (auto iter = command_list.begin(); iter != command_list.end(); iter++, i++){
		pid_t pid = fork();
		if (pid == 0){
			//child 
			std::cout << "i = " << i << std::endl;
			printf("command name: %s\n", iter->name.c_str());
			if (i != 0){ // not the first command in the list
				std::cout<< "set stdin" << std::endl;
				dup2(fds[(i-1)*2], 0);
				close(fds[(i-1)*2]);
				close(fds[(i-1)*2+1]);
			}

			if (i != command_list.size() -1 ){ // not the last command in the list
				std::cout<< "set stdout" << std::endl;
				dup2(fds[i*2+1], 1);
				close(fds[i*2+1]);
				// close(fds[i*2]);
			}

			auto c = *iter;
			char ** args = new char* [c.parameter_len() + 2]; // one for command name, one for NULL
			args[0] = new char[c.name.size()];
			strncpy(args[0], c.name.c_str(), c.name.size());
			size_t index = 1;
			for(auto iter = c.parameters.begin(); 
				iter!=c.parameters.end(); 
				iter++, index++)
			{
				args[index] = new char[(*iter).content.size()];
				strncpy(args[index], (*iter).content.c_str(), (*iter).content.size());
				printf("arg : %s\n", args[index]);
			}
			args[index] = NULL; // the last one should be NULL
			execvp(iter->name.c_str(), args);
			// cleanup if failed
			for(size_t i = 0; i < c.parameter_len(); i++)
				delete[] args[i];
			delete[] args;
			exit(errno);
		}
		else if (pid < 0){
			fprintf(stderr, "Fork Failed in exec_command");
		}
	}

	for(int i = 0; i < number_of_pipes; i++){
        close(fds[i]);
    }

	int status;
    for(int i = 0; i < command_list.size(); i++){
        wait(&status);
        printf("%d completed with status %d\n", i, status);
    }
}
