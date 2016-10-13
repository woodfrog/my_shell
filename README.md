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







