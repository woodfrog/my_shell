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


class Command{
public:
    std::string name;
    std::list<std::string> parameters;
    CommandType type;
    bool is_process; // true if this is a external command
    bool completed = 0;
    bool stopped = 0;
    pid_t pid;
    int status;
    
    Command(): pid(0) {}
    
    Command(char *name): pid(0){
        this->name = name;
    }
    void add_parameter(std::string p);
    void show_parameters();
    CommandType check_type();
    size_t parameter_len();
};

class Job{
public:
  std::string command_line;
  std::list<Command> commands;
  bool is_bg;                   /* the job is in background or not */
  pid_t pgid;                   /* process group ID */
  struct termios tmodes;        /* saved terminal modes*/
  Job* next;                    /* point to next job */

  Job(): pgid(0){}
  bool is_completed();
  bool is_stopped();
};


#endif