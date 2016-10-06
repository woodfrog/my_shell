#include <stdio.h>
#include "command_parser.h"
#include "prompt.h"
#include "command_exec.h"
#include "common.h"
#include "job_control.h"
#include <list>

pid_t shell_pgid;                // the process group ID of shell
struct termios shell_tmodes;     // to store the shell's modes
int shell_terminal;              // the file descriptor 
int shell_is_interactive;
    
int main(void) {
    
    init_shell();
  
    while (true) {
        print_prompt();

        // std::list<Command> command_list;
        Job job;

        parse_job(job);
        
        exec_job(job);
        
        // test for parser 
        // for (auto iter = job.commands.begin(); iter != job.commands.end(); iter++) {
        //     printf("%s\n", iter->name.c_str());
        //     iter->show_parameters();
        // }
    }
    return 0;
}
