#ifndef COMMAND_EXEC_H
#define COMMAND_EXEC_H
#include "common.h"
#include "structure.h"

void exec_commands(std::list<Command> &command_list); // return type not determined yet
void exec_piped_commands(std::list<Command> &command_list);







#endif