#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H
#include "common.h"
#include "basic.h"

void wait_for_job(Job &job);

int mark_process_status(pid_t pid, int status);

void put_job_foreground(Job &job);


#endif