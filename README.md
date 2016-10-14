# MyShell
------------------------

This is a mini shell with basic functions including:
 
1. internal commands like job control, cd, exit.
2. external program execution.

The main difficulty is the implementation of job control. Now I'm going to review some core parts of this shell.

## Conceptions

* job: A job contains a bunch of processes linked with pipes. Each job has an identifier which is its process group id. This id is always the process id of its leader process. Therefore, a job can be interpreted as a group of process which can be ran, continued, stopped together. 


* process: A program in execution with a PCB to store associated information like state, PC, opened files. **In shell we need to record the process id of each process.** And this can be retrieved from the operating system using the system call **getpid**.

* external program: Programs in certain place in the file system. The shell uses fork() to spawn a child process and uses exec-like functions to replace the child process' memory with the specified new program. The system call would search the specified program in PATH.

* internal commands: commands like cd, exit and job control commands, which is implemented in shell.

* signal: A form of inter-process communication(IPC) used in UNIX and UNIX-like operating systems. It's a kind of asynchronous notification sent to a process. When a signal is sent, the objective process will be interrupted. For each signal, a process has a default handler to deal with it. But it can be changed to ignore or replaced by a user-handler. In shell, signals is very important for parent process(the shell) and child processes to communicate.

* pipe: pipe is a special kernel file just like stdin and stdout. And ordinary pipes are used to implement IPC between processes having some relationship(like parent-child or children of the same parent). Like stdin and stdout, pipe can have file descriptors(FD) pointing to it, processes who have the corresponding FD can read or write from the pipe. 

	And each pipe has two sides, one for read, the other for write, so a single pipe can only support unidirectional communication.
	
	To avoid the leak of FD, It's important for **every process (both parent and child)** to close all its pipes after complete reading or writing . If some process fail to close some FP, the pipe pointed by this FD will remain opened. Therefore, those processes who read from this pipe would consider that the input hasn't ended(just like the EOF hasn't appeared), and it tends to wait until the input is completed. But in fact, the input can never been completed since some process forget to close the pipe. The result is those processes wait forever and the parent of those processes(maybe the shell) also wait their children to terminate forever--then we would see the shell just hang up and have no more response to user.   



## Some Implementation Details

* Terminal control and signals 

	There is only one group of process controlling the terminal at one moment, which gets the input from stdin. 
	
	When the shell is ran, the first step is to check whether it is ran in foreground. Only when it is a foreground process can we give the control of terminal to it, or we need to send signal to it to stop it continuously. If the shell is indeed a foreground process, we then use **tcsetpgrp()**(terminal control, set process group) to give it the control of terminal.
	
	Besides, the shell cannot be hanged up by Ctrl+Z or forced to terminate by Ctrl+C. Therefore, the signal SIGTSTP and SIGINT need to be set to SIG_IGN correspondingly using the system call **signal()** 

	For other signals: SIGTTIN, SIGTTOU, SIGCHLD

* Pipes







