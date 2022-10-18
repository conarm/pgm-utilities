# makefile for pgm utilities

# compiler name and flags
CC=gcc -g -std=c99 -fmax-errors=1 -Wall -Werror

# the set of executables
TARGETS=pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble

# all target to compile all target utilities
all:	${TARGETS}

# targets to compile individual utilities
pgmEcho:	pgmEcho.o fileOperations.o errorHandling.o
	${CC} -o pgmEcho pgmEcho.o fileOperations.o errorHandling.o

pgmComp:	pgmComp.o fileOperations.o errorHandling.o
	${CC} -o pgmComp pgmComp.o fileOperations.o errorHandling.o

pgma2b:	pgma2b.o fileOperations.o errorHandling.o
	${CC} -o pgma2b pgma2b.o fileOperations.o errorHandling.o

pgmb2a:	pgmb2a.o fileOperations.o errorHandling.o
	${CC} -o pgmb2a pgmb2a.o fileOperations.o errorHandling.o

pgmReduce:	pgmReduce.o fileOperations.o errorHandling.o
	${CC} -o pgmReduce pgmReduce.o fileOperations.o errorHandling.o

pgmTile:	pgmTile.o fileOperations.o errorHandling.o
	${CC} -o pgmTile pgmTile.o fileOperations.o errorHandling.o

pgmAssemble:	pgmAssemble.o fileOperations.o errorHandling.o
	${CC} -o pgmAssemble pgmAssemble.o fileOperations.o errorHandling.o

# clean option to remove all targets and corresponding object files
clean:
	rm -f *.o ${TARGETS}
