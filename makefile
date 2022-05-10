CLASSDIR = ../b191210004
INCLUDE = -I$(CLASSDIR)/include
LIBS = $(CLASSDIR)/lib/libfdr.a
CC = gcc

EX  =  bin/program

all: $(EX) 

clean: 
	rm -f bin/*

bin/program: src/program.c $(LIBS)
	$(CC) $(INCLUDE) -o bin/program src/program.c $(LIBS)

