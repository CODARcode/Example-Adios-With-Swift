enum transport_type {
    USE_DSPACES = 0,
    USE_DIMES = 1
};

int common_run_server(int num_sp, int num_cp, int type, MPI_Comm comm);
