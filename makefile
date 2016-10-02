#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CC = g++
CFLAGS  = -g -Wall -std=c++11

all: my_shell


my_shell:  main.o command_parser.o command_exec.o structure.o prompt.o
	$(CC) $(CFLAGS) -o my_shell command_parser.o command_exec.o structure.o prompt.o main.o


main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

prompt.o: prompt.cc prompt.h
	$(CC) $(CFLAGS) -c prompt.cc

structure.o:  structure.cc structure.h common.h
	$(CC) $(CFLAGS) -c structure.cc

command_parser.o:  command_parser.cc command_parser.h common.h structure.h 
	$(CC) $(CFLAGS) -c command_parser.cc

command_exec.o:  command_exec.cc command_exec.h common.h structure.h
	$(CC) $(CFLAGS) -c command_exec.cc

clean: 
	$(RM)  *.o *~
	$(RM)  my_shell

run:
	./my_shell