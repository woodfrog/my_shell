#ifndef COMMAND_EXEC_H
#define COMMAND_EXEC_H
#include "common.h"
#include "structure.h"

void exec_command(Command c); // return type not determined yet
void exec_piped_commands(const std::list<Command> &command_list);







#endif