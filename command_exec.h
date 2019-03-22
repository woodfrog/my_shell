/* The functions for execute commands, both internal and external. */

#ifndef COMMAND_EXEC_H
#define COMMAND_EXEC_H
#include "common.h"
#include "basic.h"
#include "job_control.h"

/* This is the high-level function which decides what to do according to the type of command. */
void exec_command		(Job *job); // return type not determined yet

/* Execute the specified job, including launch each process in the job and set up the pipes and
	signal control information for them. */
void exec_job			(Job *job);





#endif
