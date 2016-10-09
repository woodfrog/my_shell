/* Functions for handling job control. Some details references the GNU shell tutorial. And
the mainly referencing part is in the function update_status(the option of waitpid which is 
WNOHANG | WUNTRACED for updating the info of children without blocking the parent) */

#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H
#include "common.h"
#include "basic.h"

/* find the job specified by its process group id in the job list */
Job* find_job					(pid_t pid);

/* wait for the specified job */
void wait_for_job				(Job* job);

/* continue the specified job and put it in foreground */
void foreground_continue_job	(Job *job);

/* continue the specified job and put it in background */
void background_continue_job	(Job *job);

/* put the specified job in foreground */
void put_job_foreground			(Job* job);

/* find the specified process and change its status information */
int set_process_status			(pid_t pid, int status);

/* notify the user if some job's status has changed(e.g. stopped or completed)
	this function calls the update_status to update the status for each process
	whose status has changed but hasn't been waited yet. */
void job_notification			();

/* update the status of all processes before we type a new prompt
   but this function is not blocking. If there is no child whose status 
   changed before this moment we just do nothing. */
void update_status				();

/* the function for the internal command jobs, to print out all non-completed job's 
	information in stdout*/
void print_job_info				();


#endif