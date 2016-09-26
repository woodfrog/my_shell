#ifndef PARSER_H
#define PARSER_H
#include "structure.h"
#include "common.h"

void parse_commond(Command *commond);
bool find_pipe(char *s, int start_index, int end_index);


#endif