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

enum CommandType{
    cd,
    exit,
    jobs,
    non_built_in
};

class Parameter{
public:
    std::string content;
    
    Parameter(char *content){
        this->content = content;
    }
};


class Command{
public:
    std::string name;
    std::list<Parameter> parameters;
    CommandType type;
    
    Command(){}
    
    Command(char *name){
        this->name = name;
    }
    void add_parameter(Parameter p);
    void show_parameters();
    void check_type();
};



#endif