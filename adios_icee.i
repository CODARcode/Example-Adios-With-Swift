%module adios_icee

%{
  typedef int MPI_Comm;
  #include "adios_icee.h"
%}

typedef int MPI_Comm;

// This tells SWIG to treat char ** as a special case
%typemap(in) char ** {
     Tcl_Obj **listobjv;
     int       nitems;
     int       i;
     if (Tcl_ListObjGetElements(interp, $input, &nitems, &listobjv) == TCL_ERROR) {
        return TCL_ERROR;
     }
     $1 = (char **) malloc((nitems+1)*sizeof(char *));
     for (i = 0; i < nitems; i++) {
        $1[i] = Tcl_GetStringFromObj(listobjv[i],0);
     }
     $1[i] = 0;
}

// This gives SWIG some cleanup code that will get called after the function call
%typemap(freearg) char ** {
     if ($1) {
        free($1);
     }
}

// Now a test functions
%inline %{
int print_args(char **argv) {
    int i = 0;
    while (argv[i]) {
         printf("argv[%d] = %s\n", i,argv[i]);
         i++;
    }
    return i;
}
%}

%include "adios_icee.h"
