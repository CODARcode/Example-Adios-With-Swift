This is an example how Adios function can be used with Swift-T.

The procedure will be as follows:
C/C++ ->
C/C++ Wrapper (by using Swig) ->
Tcl script to call the wrapper ->
Swift script

Steps
-----

### 1. Make a function

Create a function in C/C++ to expose as a shared lib:
```
int main_leaf (int argc, char ** argv, MPI_Comm world_comm)
```
and build a shared lib (*.so)

Related file: ```adios_icee.cxx```

### 2. Using Swig

Using Swig to create a wrapper to be called in TCL. For this purpose
we need to define *.i file as an input for Swig

Related files: ```adios_icee.i```

In this example, we use the following comand to build:
```
swig -c++ adios_icee.i
$(CXX) -c -fPIC $(TCL_INC) adios_icee_wrap.cxx
$(CXX) -shared -o libadios_icee.so \
       adios_icee_wrap.o adios_icee.o filelock.o icee_cmdline.o \
       $(TCL_LIB) $(LIBS)
```

### 3. Create a Tcl script to call the wrapper
Related files: ```adios_icee.tcl```

### 4. Swift script
Related files: ```test-adios-icee.swift```


Build and run
-------------

To build:
```
$ build.sh
```

We will describe here 3 run cases:


### File-based coupling
```
$ turbine -n 5 test-adios-staging.tic --wmethod=MPI --rmethod=BP
```

### Flexpath
```
$ turbine -n 5 test-adios-staging.tic --wmethod=FLEXPATH --rmethod=FLEXPATH
```

### DataSpaces
```
$ run-dataspaces.py -s 1 -c 4 : \
  --nompi turbine -n 5 test-adios-staging.tic \
  --wmethod=DATASPACES --rmethod=DATASPACES
```

run-dataspaces.py is a wrapper to run dataspaces_server, which is
available at https://github.com/jychoi-hpc/run-dataspaces

Note on Titan
-------------

### 1. load modules and set LD_LIBRARY_PATH env
```
module use -a /lustre/atlas/world-shared/csc143/jyc/titan/sw/modulefiles
module unload PrgEnv-pgi
module unload PrgEnv-gnu
module unload PrgEnv-cray
module unload PrgEnv-intel

module load PrgEnv-gnu
module load swift-t
module load adios/1.12-devel
module load swig
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
```

### 2. Use --mpirun=aprun option to run-dataspaces.py
```
$ run-dataspaces.py -s 1 -c 4 --mpirun=aprun : \
  --nompi turbine -n 5 test-adios-staging.tic \
  --wmethod=DATASPACES --rmethod=DATASPACES
```

### 3. Debugging
The following envs can be used for debugging:
```
MPICH_CPUMASK_DISPLAY=1
TURBINE_LOG=1
TURBINE_LAUNCH_OPTIONS
```

Note on Cori
------------

To run with DataSpaces:
```
$ run-dataspaces.py -s 1 -c 4 --mpirun="srun -N 1" : \
  --nompi srun -n 5 -N 1 /global/u1/w/wozniak/Public/sfw/tcl-8.6.6/bin/tclsh8.6 \
  test-adios-staging.tic '--wmethod=DIMES' '--rmethod=DIMES'
```
