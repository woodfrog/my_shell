/*The declaration of the data structure to store all information of 
  commonds in a single input.
 *The structure should be a list of commonds. And each command should
   contains a list of paramters */
#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct parameter{
    char content[20];
    struct parameter *next;
} parameter;




typedef struct command{
    char name[20];
    struct parameter *p_parameter;
    struct command *next; // point to the next commond
} command;





#endif