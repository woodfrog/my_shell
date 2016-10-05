/* The declaration of the data structure to store all information of 
   commonds in a single input. */

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "common.h"
#include <list>
#include <string.h>

void init_shell();

enum CommandType{
    CD,
    EXIT,
    JOBS,
    FG,
    BG,
    NON_BUILT_IN,
    EMPTY
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
    CommandType check_type();
    size_t parameter_len();
};

class Job{
public:
  std::list<Command> commands;
  bool is_bg;
  pid_t pgid;                   /* process group ID */
  struct termios tmodes;        /* saved terminal modes*/
};


#endif