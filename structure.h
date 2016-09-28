/*The declaration of the data structure to store all information of 
  commonds in a single input.
 *The structure should be a list of commonds. And each command should
   contains a list of paramters */
   
// I need a list for both parameters and commands!

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "common.h"
#include <list>
#include <string.h>


class Parameter{
public:
    char content[20];
    
    Parameter(char *content){
        strncpy(this->content, content, sizeof(this->content));
    }
};


class Command{
public:
    char name[20];
    std::list<Parameter> parameters;
    
    Command(){}
    
    Command(char *name){
        strncpy(this->name, name, sizeof(this->name));
    }
    
    void add_parameter(Parameter p);
    void show_parameters();
};


#endif