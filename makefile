# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = g++
CFLAGS  = -g -Wall

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
all: my_shell


#
my_shell:  main.o command_parser.o command_exec.o structure.o prompt.o
	$(CC) $(CFLAGS) -o my_shell command_parser.o command_exec.o structure.o prompt.o main.o

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

prompt.o: prompt.cc prompt.h
	$(CC) $(CFLAGS) -c prompt.cc

structure.o:  structure.cc structure.h common.h
	$(CC) $(CFLAGS) -c structure.cc

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
command_parser.o:  command_parser.cc command_parser.h common.h structure.h 
	$(CC) $(CFLAGS) -c command_parser.cc

# To create the object file scanner.o, we need the source files
# scanner.c and scanner.h:
#
command_exec.o:  command_exec.cc command_exec.h common.h structure.h
	$(CC) $(CFLAGS) -c command_exec.cc

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) count *.o *~