
import io;
import string;
import sys;
import location;

app(void signal) check_conf_exists () {
   "./check_conf_exists.sh"
//    "/bin/sleep" 5
}

@par @dispatch=WORKER (int z) ds(int j, int k, int l) "ds" "0.0" "ds_tcl";
@par @dispatch=WORKER (int z) main_leaf(string argv[]) "adios_icee" "0.0" "main_leaf_tcl";

argv_accept("wmethod", "rmethod");

string wmethod;
string rmethod;

wmethod = argv("wmethod", "MPI");
rmethod = argv("rmethod", "BP");

trace("Write method" + wmethod);
trace("Read method" + rmethod);

string a[] = hostmapList();
foreach value, index in a
{
  trace(value);
}

int b = @par=1 ds(1, 4, 0)  =>
trace("Starting server ... done.");

check_conf_exists() => {
	@par=2 main_leaf(split(sprintf("adios_icee --nstep=1 -c -r %s -w MPI --rparam=verbose=3",rmethod), " ")) =>
	trace("Reading ... done.");

	@par=2 main_leaf(split(sprintf("adios_icee --nstep=1 -w %s --wparam=verbose=3",wmethod), " ")) =>
	trace("Writing ... done.");
}
