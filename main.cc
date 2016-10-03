#include <stdio.h>
#include "command_parser.h"
#include "prompt.h"
#include "command_exec.h"
#include "common.h"
#include <list>

using namespace std;

int main(void) {
    while (true) {
    	// make the shell ignore Ctrl+C and Ctrl+Z
        // signal(SIGTSTP, SIG_IGN); 
		// signal(SIGINT, SIG_IGN);
        print_prompt();

        std::list<Command> commond_list;

        parse_commond(commond_list);


       	exec_piped_commands(commond_list); 
        // for (auto iter = commond_list.begin(); iter != commond_list.end(); iter++) {
        //     // printf("%s\n", iter->name.c_str());
        //     exec_command(*iter);
        // }


    }
    return 0;
}
