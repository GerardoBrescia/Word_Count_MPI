CFLAGS = $(shell pkg-config --cflags glib-2.0)
CLIBS = $(shell pkg-config --libs glib-2.0)

all : main.o file_inf.o file_chunk.o logger.o
	mpicc main.o file_chunk.o file_inf.o logger.o -o driver.out $(CFLAGS) $(CLIBS)

main.o :
	mpicc -c main.c $(CFLAGS) $(CLIBS)

logger.o:
	mpicc -c logger.c	$(CFLAGS) $(CLIBS)
	
file_inf.o:
	mpicc -c file_inf.c $(CFLAGS) $(CLIBS)

file_chunk.o:
	mpicc -c file_chunk.c
	
clean: 
	rm -f main.o file_inf.o file_chunk.o logger.o driver.out