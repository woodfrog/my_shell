#include "command_parser.h"
#include "prompt.h"
#include "command_exec.h"
#include "common.h"
#include "job_control.h"

pid_t shell_pgid;                // the process group ID of shell
struct termios shell_tmodes;     // to store the shell's modes
int shell_terminal;              // the file descriptor 
int shell_is_interactive;

Job* first_job = NULL;

int main(void) 
{
    init_shell();    
      
    while (true) {
        
        job_notification();

        print_prompt();

        // std::list<Command> command_list;
        Job *job = new Job;

        parse_command(job);
        
        exec_command(job);
        
    }
    return 0;
}
