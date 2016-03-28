/*
 * ADIOS is freely available under the terms of the BSD license described
 * in the COPYING file in the top level directory of this source distribution.
 *
 * Copyright (c) 2008 - 2009.  UT-BATTELLE, LLC. All rights reserved.
 */

/* ADIOS ICEE Example
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <locale.h>
#include "mpi.h"
#include "adios.h"
#include "adios_read.h"
#include "adios_error.h"
#include <string>
#include "icee_cmdline.h"
#include <sstream>
#include <time.h>
#include <sys/resource.h>

using namespace std;

#ifndef ATYPE
#define ATYPE       double
#endif
#ifndef ADIOS_ATYPE
#define ADIOS_ATYPE adios_double
#endif

#ifndef VERSION
#define VERSIOn NA
#endif
#define xstr(s) stringfy(s)
#define stringfy(s) #s

#include <sys/time.h>
struct timeval adios_timer_tp;
double adios_gettime()
{
    gettimeofday(&adios_timer_tp, NULL); \
    return  ((double)adios_timer_tp.tv_sec + ((double)adios_timer_tp.tv_usec)/1000000.0);
}

void sleep_with_interval (double timeout_sec, int interval_ms)
{
    int stay_in_poll_loop = 1;
    double t1 = adios_gettime();
    double elapsed_time;

    struct timespec delay;
    delay.tv_sec = interval_ms/1000;
    delay.tv_nsec = (((uint64_t)interval_ms * 1000000L)%1000000000L);

    while (stay_in_poll_loop)
    {
        elapsed_time = adios_gettime() - t1;

        if (elapsed_time > timeout_sec)
            stay_in_poll_loop = 0;
        else
            nanosleep(&delay, NULL);
    }
    //printf("Spent: %g (sec)\n", elapsed_time);
}

int main (int argc, char ** argv)
{
    setlinebuf(stdout);

    struct icee_args_info args_info;

    if (icee_cmdline_parser (argc, argv, &args_info) != 0)
        exit(1);

    if (args_info.version_given)
    {
        printf("%s (version: %s)\n", argv[0], xstr(VERSION));
        exit(1);
    }

    string adios_write_method = "POSIX1";
    enum ADIOS_READ_METHOD adios_read_method = ADIOS_READ_METHOD_BP;

    uint64_t NX = args_info.len_arg;
    uint64_t NY = args_info.chunk_arg;

    float timeout_sec = args_info.timeout_arg;
    int   interval_sec = args_info.sleep_arg;
    int   nsteps = args_info.nsteps_arg;

    if (args_info.writemethod_given)
        adios_write_method = string(args_info.writemethod_arg);

    if (args_info.readmethod_given)
    {
        if (string(args_info.readmethod_arg) == "BP") {
            adios_read_method = ADIOS_READ_METHOD_BP;
        } else if (string(args_info.readmethod_arg) == "ICEE") {
            adios_read_method = ADIOS_READ_METHOD_ICEE;
        } else if (string(args_info.readmethod_arg) == "DIMES") {
            adios_read_method = ADIOS_READ_METHOD_DIMES;
        } else if (string(args_info.readmethod_arg) == "DATASPACES") {
            adios_read_method = ADIOS_READ_METHOD_DATASPACES;
        } else {
            fprintf(stderr, "No read method: %s\n", optarg);
        }
    }


    int         rank, size;
    MPI_Comm    comm = MPI_COMM_WORLD;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    typedef enum {SERVER, CLIENT} mode_t;
    mode_t mode = SERVER;

    std::stringstream s;
    //time_t ltime;
    //char *timetext;

    if (args_info.client_flag)
        mode = CLIENT;

    string initstr = string(args_info.params_arg);
    string prefix = string(args_info.prefix_arg);

    if (adios_write_method == "ICEE")
    {
        s << "verbose=" << args_info.verbose_arg << ";";

        if (args_info.host_given)
            s << "cm_host=" << args_info.host_arg << ";";

        s << "cm_port=" << args_info.port_arg + rank << ";";

        if (args_info.remotehost_given)
            s << "cm_remote_host=" << args_info.remotehost_arg << ";";

        s << "cm_remote_port=" << args_info.remoteport_arg + rank << ";";

        if (args_info.method_given)
            s << "transport=" << args_info.method_arg << ";";

        if (args_info.passive_given)
            s << "is_passive=" << args_info.passive_flag << ";";

        if (args_info.isnative_given)
            s << "use_native_contact=" << args_info.isnative_flag << ";";

        if (args_info.nclient_given)
            s << "max_client=" << args_info.nclient_arg << ";";

        if (args_info.remotelist_given)
            s << "remote_list=" << args_info.remotelist_arg << ";";

        if (args_info.attrlist_given)
            s << "attr_list=" << args_info.attrlist_arg << ";";

        initstr = initstr + ";" + s.str();
    }

    setlocale(LC_NUMERIC, "en_US.UTF-8");
    string fname = "icee.bp";
    if (prefix.length() > 0)
        fname = prefix + "/" + fname;

    if (mode == SERVER)
    {
        uint64_t    G, O;
        ATYPE        *t = (ATYPE *) malloc(NX * NY * sizeof(ATYPE));
        assert(t != NULL);
        uint64_t    adios_groupsize, adios_totalsize;

        adios_init_noxml (comm);
        adios_allocate_buffer (ADIOS_BUFFER_ALLOC_NOW, ((NX * NY * sizeof(ATYPE))>>20) + 1L);
        //adios_set_max_buffer_size (((NX * NY * sizeof(ATYPE))>>20) + 1L);

        int64_t       m_adios_group;
        int64_t       m_adios_file;
        string amode = "w";

        adios_declare_group (&m_adios_group, "restart", "", adios_flag_no);
        adios_select_method (m_adios_group, adios_write_method.c_str(), initstr.c_str(), "");

        adios_define_var (m_adios_group, "NX"
                          ,"", adios_long
                          ,0, 0, 0);

        adios_define_var (m_adios_group, "NY"
                          ,"", adios_long
                          ,0, 0, 0);

        adios_define_var (m_adios_group, "G"
                          ,"", adios_long
                          ,0, 0, 0);

        adios_define_var (m_adios_group, "O"
                          ,"", adios_long
                          ,0, 0, 0);

        adios_define_var (m_adios_group, "temperature"
                          ,"", ADIOS_ATYPE
                          ,"NX,NY", "G,NY", "O,0");

        G = NX * size;

        if (rank==0)
        {
            printf("===== SUMMARY =====\n");
            printf("%10s : %s\n", "Method", adios_write_method.c_str());
            printf("%10s : %s\n", "Params", initstr.c_str());
            printf("%10s : %'d (seconds)\n", "Interval", interval_sec);
            printf("%10s : %'d\n", "PEs", size);
            printf("%10s : %'llu x %'llu\n", "Local dims", NX, NY);
            printf("%10s : %'.02f (MiB/proc)\n", "Data/PE", NX*NY*sizeof(ATYPE)/1024.0/1024.0);
            printf("%10s : %'.02f (MiB)\n", "Total", G*NY*sizeof(ATYPE)/1024.0/1024.0);
            printf("%10s : %'d\n", "Steps", nsteps);
            printf("===================\n\n");
        }

        for (int it =0; it < nsteps; it++)
        {
            for (uint64_t i = 0; i < NX*NY; i++)
                t[i] = rank + it + 1.0;

            if (args_info.append_flag)
                amode = (!it)? "w" : "a";

            double t1 = MPI_Wtime();
            adios_open (&m_adios_file, "restart", fname.c_str(), amode.c_str(), comm);
            adios_groupsize = 4 * 8 + NX * NY * sizeof(ATYPE);
            adios_group_size (m_adios_file, adios_groupsize, &adios_totalsize);
            //adios_set_max_buffer_size (adios_groupsize*size/1024L/1024L+1); // in MB

            double t2 = MPI_Wtime();
            adios_write(m_adios_file, "NX", (void *) &NX);
            adios_write(m_adios_file, "NY", (void *) &NY);
            adios_write(m_adios_file, "G", (void *) &G);
            O = rank * NX;
            adios_write(m_adios_file, "O", (void *) &O);
            adios_write(m_adios_file, "temperature", t);
            double t3 = MPI_Wtime();
            adios_close (m_adios_file);
            double t4 = MPI_Wtime();

            double t_elap14 = t4-t1;
            double t_elap24 = t4-t2;
            double t_elap34 = t4-t3;

            //ltime=time(NULL);
            //timetext = asctime(localtime(&ltime));
            //timetext[24] = '\0';

            if (it==0 && rank==0)
            {
                printf("    %14s %5s %5s %9s %9s %9s %9s %9s %9s\n", "timestep",   "seq",  "rank",   "t1(sec)",   "(MiB/s)",   "t2(sec)",   "(MiB/s)",   "t3(sec)",   "(MiB/s)");
                printf("    %14s %5s %5s %9s %9s %9s %9s %9s %9s\n", "--------", "-----", "-----", "---------", "---------", "---------", "---------", "---------", "---------");
            }
            MPI_Barrier(comm);
            sleep_with_interval((double)interval_sec, 100);

            printf(">>> %14.03f %5d %5d %9.03e %9.03f %9.03e %9.03f %9.03e %9.03f\n",
                   t3, it, rank,
                   t_elap14, (double)adios_groupsize/t_elap14/1024.0/1024.0,
                   t_elap24, (double)adios_groupsize/t_elap24/1024.0/1024.0,
                   t_elap34, (double)adios_groupsize/t_elap34/1024.0/1024.0);
        }

        adios_finalize (rank);
    }
    else
    {
        ADIOS_FILE * f;
        ADIOS_VARINFO * v;
        ADIOS_SELECTION * sel;
        int err;

        ATYPE *data = NULL;
        uint64_t start[2], count[2];

        err = adios_read_init_method (adios_read_method, comm, initstr.c_str());
        if (!err) {
            printf ("%s\n", adios_errmsg());
        }

        f = adios_read_open (fname.c_str(), adios_read_method, comm,
                             ADIOS_LOCKMODE_ALL, timeout_sec);
        if (adios_errno == err_file_not_found)
        {
            printf ("rank %d: Stream not found after waiting %f seconds: %s\n",
                    rank, timeout_sec, adios_errmsg());
        }
        else if (adios_errno == err_end_of_stream)
        {
            printf ("rank %d: Stream terminated before open. %s\n", rank, adios_errmsg());
        }
        else if (f == NULL) {
            printf ("rank %d: Error at opening stream: %s\n", rank, adios_errmsg());
        }
        else
        {
            /* process file here... */
            v = adios_inq_var (f, "temperature");

            uint64_t slice_size = v->dims[0]/size;
            uint64_t chunk_size = v->dims[1];

            start[0] = rank * slice_size;
            count[0] = slice_size;
            start[1] = 0;
            count[1] = chunk_size;

            if (rank == size-1)
            {
                slice_size = v->dims[0] - rank * slice_size;
                count[0] = slice_size;
            }

            if (rank==0)
            {
                printf("===== SUMMARY =====\n");
                printf("%10s : %s\n", "Method", args_info.readmethod_arg);
                printf("%10s : %s\n", "Params", initstr.c_str());
                printf("%10s : %'d (seconds)\n", "Interval", interval_sec);
                printf("%10s : %'d\n", "PEs", size);
                printf("%10s : %'llu x %'llu\n", "Global dims", v->dims[0], v->dims[1]);
                printf("%10s : %'d\n", "Steps", nsteps);
                printf("===================\n\n");
            }

            data = (ATYPE *) malloc (slice_size * chunk_size * sizeof(ATYPE));
            assert(data != NULL);

            if (rank==0)
            {
                printf("    %14s %5s %5s %9s %9s %s\n", "timestep",   "seq",  "rank", "time(sec)",   "(MiB/s)", "check");
                printf("    %14s %5s %5s %9s %9s %s\n", "--------", "-----", "-----", "---------", "---------", "-----");
            }

            /* Processing loop over the steps (we are already in the first one) */
            //while (adios_errno != err_end_of_stream) {
            for (int it =0; it < nsteps; it++)
            {

                //MPI_Barrier(comm);
                double t_start = MPI_Wtime();

                sel = adios_selection_boundingbox (v->ndim, start, count);
                adios_schedule_read_byid (f, sel, v->varid, 0, 1, data);
                adios_perform_reads (f, 1);
                adios_release_step (f);

                //MPI_Barrier(comm);
                double t_end = MPI_Wtime();
                double t_elap = t_end - t_start;

                //ltime=time(NULL);
                //timetext = asctime(localtime(&ltime));
                //timetext[24] = '\0';

                double sum = 0.0;
                for (uint64_t i = 0; i < slice_size * chunk_size; i++)
                {
                    sum += (double) data[i];
                }

                printf(">>> %14.03f %5d %5d %9.03e %9.03f %'.0f\n",
                       t_end, f->current_step, rank, t_elap,
                       (double)(count[0]*chunk_size)*sizeof(ATYPE)/t_elap/1024.0/1024.0,
                       sum);

                if (it==nsteps-1) break;

                sleep_with_interval((double)interval_sec, 100);

                // advance to 1) next available step with 2) blocking wait
                adios_advance_step (f, 0, timeout_sec);
                if (adios_errno == err_step_notready)
                {
                    printf ("rank %d: No new step arrived within the timeout. Quit. %s\n",
                            rank, adios_errmsg());
                    break; // quit while loop
                }

                if (adios_errno == err_end_of_stream)
                {
                    printf ("rank %d: End of stream. Quit. %s\n",
                            rank, adios_errmsg());
                    break; // quit while loop
                }
            }

            adios_read_close (f);
            free (data);
        }

        adios_read_finalize_method (adios_read_method);
    }

    if (rank==0)
    {
        printf ("Done.\n");

        struct rusage r_usage;
        getrusage(RUSAGE_SELF,&r_usage);
        printf("Memory usage: %'.2f MiB\n", r_usage.ru_maxrss/1024.0/1024.0);

        void *v = NULL;
        int vval=-1, flag=-1;
        MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &v, &flag);
        if (flag)
            vval = *(int*)v;
        printf("MPI_WTIME_IS_GLOBAL: %d\n", vval);
    }


    MPI_Finalize ();
    return 0;
}
