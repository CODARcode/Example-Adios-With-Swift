%module ds

%include "ds.h"

%{

  typedef int MPI_Comm;
  #include "ds.h"
%}

typedef int MPI_Comm;
