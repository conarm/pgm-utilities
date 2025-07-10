# PGM Utilities

A set of modules to manipulate .pgm image files in various ways 

## Building

The makefile provided uses the gcc compiler with a number of flags, detailed in the comments. There are multiple targets, detailed below:

 - make all - compile all utilities and its dependencies.
 - make pgmEcho - compile pgmEcho and its dependencies ALONE.
 - make pgmComp - compile pgmComp and its dependencies ALONE.
 - make pgma2b - compile pgma2b and its dependencies ALONE.
 - make pgmb2a - compile pgmb2a and its dependencies ALONE.
 - make pgmReduce - compile pgmReduce and its dependencies ALONE.
 - make clean - remove all object files and executables in the directory.

## Testing

There is a bash testscript provided, in testscript.sh. In order to run this, run './testscript' from within the directory on a terminal. NOTE: it is important to 'make clean' and 'make all' before this.

## Example files

Examples of PGM files to run through the tool can be found [here](https://people.sc.fsu.edu/~jburkardt/data/pgma/pgma.html).

Since PGM files are encoded in ASCII, the tools deal heavily with string manipulation.
