
import io;
import string;
import sys;

@par @dispatch=WORKER (int z) main_leaf(string argv[]) "adios_icee" "0.0" "main_leaf_tcl";

argv_accept("wmethod", "rmethod");

string wmethod;
string rmethod;

wmethod = argv("wmethod", "MPI");
rmethod = argv("rmethod", "BP");

trace("Write method" + wmethod);
trace("Read method" + rmethod);

@par=2 main_leaf(split(sprintf("adios_icee --nstep=1 -c -r %s -w MPI --rparam=verbose=3",rmethod), " ")) =>
trace("Reading ... done.");

@par=2 main_leaf(split(sprintf("adios_icee --nstep=1 -w %s --wparam=verbose=3",wmethod), " ")) =>
trace("Writing ... done.");
