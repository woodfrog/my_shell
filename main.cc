#include "command_parser.h"
#include "prompt.h"
#include "command_exec.h"
#include "common.h"
#include "job_control.h"

pid_t shell_pgid;                // the process group ID of shell
struct termios shell_tmodes;     // to store the shell's modes
int shell_terminal;              // the file descriptor 
int shell_is_interactive;

std::vector<Job&> job_table;

int main(void) 
{
    init_shell();    
      
    while (true) {
        print_prompt();

        // std::list<Command> command_list;
        Job job = new Job()

        parse_command(job);
        
        exec_command(job);
        
    }
    return 0;
}
