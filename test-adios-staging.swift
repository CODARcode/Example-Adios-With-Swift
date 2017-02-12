
import io;
import string;

@par @dispatch=WORKER (int z) main_leaf(string argv[]) "adios_icee" "0.0" "main_leaf_tcl";

int x;

//wait (x) {
  @par=2 main_leaf(split("adios_icee --nstep=1 -c -r FLEXPATH -w MPI --rparam=verbose=7", " ")) =>
  trace("Reading ... done.");
//}

x = @par=2 main_leaf(split("adios_icee --nstep=1 -w FLEXPATH --wparam=verbose=7", " ")) =>
trace("Writing ... done.");
