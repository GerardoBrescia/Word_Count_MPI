CFLAGS = $(shell pkg-config --cflags glib-2.0)
CLIBS = $(shell pkg-config --libs glib-2.0)

IDIR = ../lib
CXXFLAGS += -I$(IDIR)

all : main.o file_inf.o file_chunk.o logger.o lexer.o word.o
	mpicc main.o file_chunk.o file_inf.o logger.o lexer.o word.o -o count.out $(CFLAGS) $(CLIBS)

main.o :
	mpicc $(CXXFLAGS) -c main.c $(CFLAGS) $(CLIBS)

logger.o:
	mpicc $(CXXFLAGS) -c logger.c	$(CFLAGS) $(CLIBS)

lexer.o:
	mpicc $(CXXFLAGS) -c lexer.c	$(CFLAGS) $(CLIBS)
	
file_inf.o:
	mpicc $(CXXFLAGS) -c file_inf.c $(CFLAGS) $(CLIBS)

file_chunk.o:
	mpicc $(CXXFLAGS) -c file_chunk.c

word.o:
	mpicc $(CXXFLAGS) -c word.c	
	
clean: 
	rm -f main.o file_inf.o file_chunk.o lexer.o logger.o word.o count.out