This is an example how Adios function can be used with Swift-T.

The procedure will be as follows:
C/C++ ->
C/C++ Wrapper (by using Swig) ->
Tcl script to call the wrapper ->
Swift script

Steps
=====

1. Make a function

Create a function in C/C++ to expose as a shared lib:

int main_leaf (int argc, char ** argv, MPI_Comm world_comm)

and build a shared lib (*.so)

Related file:
adios_icee.cxx

2. Using Swig

Using Swig to create a wrapper to be called in TCL. For this purpose
we need to define *.i file as an input for Swig

Related files:
adios_icee.i

In this example, we use the following comand to build:
swig -c++ adios_icee.i
$(CXX) -c -fPIC $(TCL_INC) adios_icee_wrap.cxx
$(CXX) -shared -o libadios_icee.so adios_icee_wrap.o adios_icee.o filelock.o icee_cmdline.o $(TCL_LIB) $(LIBS)

3. Create a Tcl script to call the wrapper
adios_icee.tcl

4. Swift script
test-adios-icee.swift


Build and run
=============

To build:
$ build.sh

To run:
$ test-adios-icee.sh
