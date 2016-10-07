#include "basic.h"

extern pid_t shell_pgid;
extern struct termios shell_tmodes;
extern int shell_terminal;
extern int shell_is_interactive;

  
/* This function takes the GNU job-control shell tutorial as reference. My original code 
  didn't consider the situation in which the shell is not in foreground, so it would crash
  in some particular cases(e.g. run another my_shell in background)*/

void init_shell()
{
	
	shell_terminal = STDIN_FILENO;

  /* Test whether the fd shell_terminal refers to a terminal */
  shell_is_interactive = isatty (shell_terminal);
	  
	if (shell_is_interactive){
      /* Loop until we are in the foreground.  */
      while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ())) // set the global shell_pgid
        kill (- shell_pgid, SIGTTIN);

      /* Ignore interactive and job-control signals.  */
      signal (SIGINT, SIG_IGN);
      signal (SIGQUIT, SIG_IGN);
      signal (SIGTSTP, SIG_IGN);
      signal (SIGTTIN, SIG_IGN);
      signal (SIGTTOU, SIG_IGN);

      // this signal cannot be ignored in shell.If it does, the shell process would fail to
      // catch the signal for children's terminating. 
      // But why? in the GNU tutorial it says the default way to deal with signal is also ignore.
      // signal (SIGCHLD, SIG_IGN); 

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

/* These functions are for class Command. In fact, it's better to be named as Process, but 
   since I also use it to store the information for internal commands like cd and job
   control commands, finally I decide to call it Command. 
*/
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


void Command::add_parameter(std::string p)
{   
    parameters.push_back(p);
}


void Command::show_parameters()
{
    for (auto iter = parameters.begin(); iter != parameters.end(); iter++){
        std::cout << *iter << std::endl; 
    }
}


size_t Command::parameter_len()
{
	return parameters.size();
}


/* The following functions are for class Job. They simply iterate each process(though the name
    is command here, the reason has been explained above) and check whether all of them are 
    completed or stopped.
 *  If all processes of the job is stopped, is_stopped returns true, else returns false, 
    So does the is_completed.*/

bool Job::is_completed()
{
    for (auto iter = commands.begin(); iter != commands.end(); iter++)
        if (!iter->completed)
            return false;
    return true;
}


bool Job::is_stopped()
{
    for (auto iter = commands.begin(); iter != commands.end(); iter++)
        if (!iter->stopped)
            return false;
    return true;
}