CFLAGS = $(shell pkg-config --cflags glib-2.0)
CLIBS = $(shell pkg-config --libs glib-2.0)

all : main.o file_inf.o file_chunk.o logger.o lexer.o word.o
	mpicc main.o file_chunk.o file_inf.o logger.o lexer.o word.o -o driver.out $(CFLAGS) $(CLIBS)

main.o :
	mpicc -c main.c $(CFLAGS) $(CLIBS)

logger.o:
	mpicc -c logger.c	$(CFLAGS) $(CLIBS)

lexer.o:
	mpicc -c lexer.c	$(CFLAGS) $(CLIBS)
	
file_inf.o:
	mpicc -c file_inf.c $(CFLAGS) $(CLIBS)

file_chunk.o:
	mpicc -c file_chunk.c

word.o:
	mpicc -c word.c	
	
clean: 
	rm -f main.o file_inf.o file_chunk.o lexer.o logger.o word.o driver.out