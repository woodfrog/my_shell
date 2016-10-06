#include "command_parser.h"
#include "prompt.h"
#include "command_exec.h"
#include "common.h"
#include "job_control.h"

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
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
