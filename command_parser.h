#ifndef PARSER_H
#define PARSER_H
#include "basic.h"
#include "common.h"

void parse_command(Job *job);
bool find_pipe(char *s, int start_index, int end_index);
bool handle_bg_symbol(char *s);

#endif