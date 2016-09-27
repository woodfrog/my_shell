/*The declaration of the data structure to store all information of 
  commonds in a single input.
 *The structure should be a list of commonds. And each command should
   contains a list of paramters */
   
// I need a list for both parameters and commands!

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "common.h"

typedef struct parameter{
    char content[20];
    struct parameter *next;
} Parameter;

// typedef struct parameter_list{
//     Parameter *head;
//     Parameter *tail;
// } ParameterList;

typedef struct command{
    char name[20];
    struct parameter *p_parameter;
    struct command *next; // point to the next commond
} Command;

#endif