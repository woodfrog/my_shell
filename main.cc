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

        std::list<Command> command_list;

        parse_commonds(command_list);

        // test for parser1 
        // for (auto iter = command_list.begin(); iter != command_list.end(); iter++) {
        //     printf("%s\n", iter->name.c_str());
        //     iter->show_parameters();
        // }
        
        exec_commands(command_list);
        


    }
    return 0;
}
