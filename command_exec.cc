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
				// cleanup
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