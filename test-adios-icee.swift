
import io;
import string;

@par @dispatch=WORKER (int z) main_leaf(string argv[], int k=0) "adios_icee" "0.0" "main_leaf_tcl";

int x = @par=2 main_leaf(split("adios_icee -w MPI --nstep=1", " "));
printf("write: %d\n", x);
int y = @par=2 main_leaf(split("adios_icee -w MPI --nstep=1 -c", " "), x);
printf("read: %d\n", y);
