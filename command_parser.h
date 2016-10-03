#ifndef PARSER_H
#define PARSER_H
#include "structure.h"
#include "common.h"

void parse_commond(std::list<Command> &command_list);
bool find_pipe(char *s, int start_index, int end_index);
bool handle_bg_symbol(char *s);

#endif