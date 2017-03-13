#include <stdio.h>
#include <mpi.h>
#include "dataspaces.h"
#include "ds.h"

int common_run_server(int num_sp, int num_cp, int type, MPI_Comm gcomm) {
    int err;
    int myrank;

    MPI_Comm_rank(gcomm, &myrank);
    printf("Rank %i Starting server...\n", myrank);
    if (type == USE_DSPACES) {
        struct ds_gspace *dsg;
        dsg = dsg_alloc(num_sp, num_cp, "dataspaces.conf", &gcomm);
        if (!dsg) {
            return -1;
        }       
        while(!dsg_complete(dsg)) {
            err = dsg_process(dsg);
            if(err < 0) {
	            break;
            }
        }
        MPI_Barrier(gcomm);
        dsg_free(dsg);
    /*
        if (err == 0)
            uloga("All ok.\n");
    */
        return 0;
    } else if (type == USE_DIMES) {
        struct dimes_server *dsg;
        printf("Doing server alloc\n");
        dsg = dimes_server_alloc(num_sp, num_cp, "dataspaces.conf", &gcomm);
        printf("Done with server alloc\n");
        if (!dsg) {
            return -1;
        }

        while (!dimes_server_complete(dsg)){
            err = dimes_server_process(dsg);
            if(err < 0) {
	            break;
            }
        }

        MPI_Barrier(gcomm);
        dimes_server_free(dsg);

    /*
        if (err == 0)
            uloga("All ok.\n");
    */
    }
    return 0;
}
