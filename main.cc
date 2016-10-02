#include <stdio.h>
#include "command_parser.h"
#include "prompt.h"
#include "command_exec.h"
#include "common.h"
#include <list>

using namespace std;

int main(void) {
    while (true) {
        print_prompt();

        std::list<Command> commond_list;

        parse_commond(commond_list);


        for (auto iter = commond_list.begin(); iter != commond_list.end(); iter++) {
            exec_command(*iter);
        }


    }
    return 0;
}
