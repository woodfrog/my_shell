/* This file declares the functions for parsing the input */

#ifndef PARSER_H
#define PARSER_H
#include "basic.h"
#include "common.h"

void parse_command		(Job *job);

/* locate the position of pipe for separating commands */
bool find_pipe			(char *s, int start_index, int end_index);

/* check the existence of the symbol &*/ 
bool handle_bg_symbol	(char *s);

#endif