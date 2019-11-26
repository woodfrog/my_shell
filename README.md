# MyShell
------------------------

A mini-shell that supports majority of Unix terminal shell’s functionalities implemented with C++.

The following basic functions are implemented:
 
1. internal commands, e.g. **jobs, cd, exit**.
2. external program execution, e.g. **cat textfile | gzip -c | tail -n 10**.

*job controls* was the most difficult implementation in this project. See the **conceptions** section for more details on core components implementation.

## Installation
To compile the codes:
```
make
```
To run the complied shell program:
```
./my_shell
```


## Conceptions

* job: A UNIX job contains multiple processes linked with pipes. Each job has a process group id as its identifier. This id is always the process id of its leader process. Therefore, a job can be interpreted as a group of processes which can be run, continued, and terminated simultaneously. 

* process: A program in execution with a process control block(PCB) to store associated information such as state, PC, opened files. **In the shell, the process id of each process must be recorded.** The process id can be retrieved from the operating system using the system call **getpid**.

* external program: Programs stored in the file system. The shell uses fork() to spawn a child process and uses exec-like functions to replace the memory of the child with the specified external program. The system call would search the specified program in PATH system environment variable.

* internal commands: commands such as cd, exit and job control commands are implemented and invoked by the shell.

* signal: A form of inter-process communication(IPC) used in UNIX and UNIX-like operating systems. Signal is a type of asynchronous notification sent to a process. When a signal is sent, the objective process will be interrupted. A process has a default handler to deal with each type of signals. The default handler can be ignored or replaced by a user-handler. In the shell, signals are important for parent processes and child processes to communicate.

* pipe: Pipe is a special kernel file same as stdin and stdout. Ordinary pipes are used to implement IPC between processes with relationships, such as parent-child processes or sibling processes. Akin to stdin and stdout, A pipe can have file descriptors(FD) pointing to it, processes which have the corresponding FD can read or write from the pipe. 

* Each pipe has two sides, one for reading and the other for writing. As a result of this characteristic, a single pipe can only support unidirectional communication.
    
* To avoid leaking of FD, it is important for **every process (both parent and child)** to close all of its pipes after completing reading or writing. If a process fails to close some FP, the pipe pointed by this FD will remain open. As a consequence, those processes which read from this pipe would consider that the input has not ended as the EOF has not appeared, and the processes will wait until the input is completed. However, in fact, the input can never be completed since some process forgot to close the pipe. The result is those processes will wait forever and the parent of those processes (or the shell) will also continuously wait for their children to terminate. Eventually, the shell will hang up and have no response to the user.   



## Implementation Details

* Terminal control and signals 

    There is only one group of process controlling the terminal at one moment, which gets the input from stdin. 
    
    When the shell is run, the first step is to check whether it is in the foreground. Only a foreground process can be given the control of the terminal; else a signal must be sent to terminate it. **tcsetpgrp()**(terminal control, set process group) gives the shell the control of the terminal.
    
    Additionally, the shell cannot be hanged up by Ctrl+Z or forced to be terminated by Ctrl+C. Therefore, the signal SIGTSTP and SIGINT need to be set to SIG_IGN correspondingly using the system call **signal()** 

    Other signals include: SIGTTIN, SIGTTOU, SIGCHLD
