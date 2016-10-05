#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CC = g++
CFLAGS  = -g -Wall -std=c++11

all: my_shell


my_shell:  main.o command_parser.o command_exec.o basic.o prompt.o
	$(CC) $(CFLAGS) -o my_shell command_parser.o command_exec.o basic.o prompt.o main.o


main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

prompt.o: prompt.cc prompt.h
	$(CC) $(CFLAGS) -c prompt.cc

basic.o:  basic.cc basic.h common.h
	$(CC) $(CFLAGS) -c basic.cc

command_parser.o:  command_parser.cc command_parser.h common.h basic.h 
	$(CC) $(CFLAGS) -c command_parser.cc

command_exec.o:  command_exec.cc command_exec.h common.h basic.h
	$(CC) $(CFLAGS) -c command_exec.cc
	
job_control.o: job_control.cc common.h job_control.h
	$(CC) $(CFLAGS) -c job_control.cc

clean: 
	$(RM)  *.o *~
	$(RM)  my_shell

run:
	./my_shell