COMP 1921: Programming Project
PGM Utilities
Made by Connor Armstrong [sc21ca]
Git repository: https://gitlab.com/comp1921_2122/sc21ca

Compiling: The makefile provided uses the gcc compiler with a number of flags, detailed in the comments. There are multiple targets which can be used, detailed below:
    make all - this will compile all utilities and its dependencies.
    make pgmEcho - this will compile pgmEcho and its dependencies ALONE.
    make pgmComp - this will compile pgmComp and its dependencies ALONE.
    make pgma2b - this will compile pgma2b and its dependencies ALONE.
    make pgmb2a - this will compile pgmb2a and its dependencies ALONE.
    make pgmReduce - this will compile pgmReduce and its dependencies ALONE.
    make clean - this will remove all object files and executables in the directory.

Testing: There is a bash testscript provided, in testscript.sh. In order to run this, run './testscript' from within the directory on a terminal. NOTE: it is important to 'make clean' and 'make all' before this.