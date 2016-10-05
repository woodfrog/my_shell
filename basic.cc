#include "basic.h"

/*this function is from the CNU shell tutorial, my original code simply */
void init_shell()
{
	pid_t shell_pgid;
	struct termios shell_tmodes;
	int shell_terminal;
	int shell_is_interactive;
	
	shell_terminal = STDIN_FILENO;
  	shell_is_interactive = isatty (shell_terminal);
	  
	if (shell_is_interactive)
    {
      /* Loop until we are in the foreground.  */
      while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
        kill (- shell_pgid, SIGTTIN);

      /* Ignore interactive and job-control signals.  */
      signal (SIGINT, SIG_IGN);
      signal (SIGQUIT, SIG_IGN);
      signal (SIGTSTP, SIG_IGN);
      signal (SIGTTIN, SIG_IGN);
      signal (SIGTTOU, SIG_IGN);
      signal (SIGCHLD, SIG_IGN);

      /* Put ourselves in our own process group.  */
      shell_pgid = getpid ();
      if (setpgid (shell_pgid, shell_pgid) < 0){
		  perror ("Couldn't put the shell in its own process group");
          exit (1);
      }

      /* Grab control of the terminal.  */
      tcsetpgrp (shell_terminal, shell_pgid);

      /* Save default terminal attributes for shell.  */
      tcgetattr (shell_terminal, &shell_tmodes);
    }
}


void Command::add_parameter(Parameter p)
{   
    parameters.push_back(p);
}


void Command::show_parameters()
{
    for (auto iter = parameters.begin(); iter != parameters.end(); iter++){
        std::cout << iter->content << std::endl; 
    }
}

CommandType Command::check_type()
{
	if (name == "cd")
		type = CD;
	else if(name =="exit")
		type = EXIT;
	else if(name == "jobs")
		type = JOBS;
	else if(name == "fg")
		type = FG;
	else if(name == "bg")
		type = BG;
	else if(name == "")
		type = EMPTY;
	else
		type = NON_BUILT_IN;
	return type;
}

size_t Command::parameter_len()
{
	return parameters.size();
}



// #ifdef DEBUG
// using namespace std;
// int main(void)
// {    
//     list<Command> l;
//     Command c = Command("test");
//     c.add_parameter(Parameter("spam"));
//     c.add_parameter(Parameter("fooooooo"));
//     c.show_parameters();
//     return 0;
// }
// #endif