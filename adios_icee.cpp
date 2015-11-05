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

using namespace std;

void usage(const char *argv0)
{
    fprintf(stderr, "usage: %s\n", argv0);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "-w [ MPI | ICEE ]\n");
    fprintf(stderr, "-r [ BP | ICEE ]\n");
    exit (1);
}

int main (int argc, char ** argv)
{
    struct icee_args_info args_info;

    if (icee_cmdline_parser (argc, argv, &args_info) != 0)
        exit(1) ;

    string adios_write_method = "POSIX1";
    enum ADIOS_READ_METHOD adios_read_method = ADIOS_READ_METHOD_BP;
    float  timeout_sec;
    
    int    NX;
    int    interval_sec = 5;
    
    if (args_info.writemethod_given)
        adios_write_method = string(args_info.writemethod_arg);
    
    if (args_info.readmethod_given)
    {
        if (string(args_info.readmethod_arg) == "BP") {
            adios_read_method = ADIOS_READ_METHOD_BP;
        } else if (string(args_info.readmethod_arg) == "ICEE") {
            adios_read_method = ADIOS_READ_METHOD_ICEE;
        } else {
            fprintf(stderr, "No read method: %s\n", optarg);
        }
    }
    
    NX = args_info.len_arg;
    timeout_sec = args_info.timeout_arg;

    int         rank, size;
    MPI_Comm    comm = MPI_COMM_WORLD;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    typedef enum {SERVER, CLIENT} mode_t;
    mode_t mode = SERVER;
    
    std::stringstream s;
    
    if (args_info.client_flag)
        mode = CLIENT;

    s << "verbose=" << args_info.verbose_arg << ";";
    
    if (args_info.host_given)
        s << "cm_host=" << args_info.host_arg << ";";
    
    if (args_info.port_given)
        s << "cm_port=" << args_info.port_arg + rank << ";";
    
    if (args_info.remotehost_given)
        s << "cm_remote_host=" << args_info.remotehost_arg << ";";
    
    if (args_info.remoteport_given)
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
    
    string initstr = s.str();

    
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    string filename = "icee.bp";
    
    if (mode == SERVER)
    {
        int         G, O;
        double      *t = (double *) malloc(NX * sizeof(double));
        uint64_t    adios_groupsize, adios_totalsize;
        
        adios_init_noxml (comm);
        adios_allocate_buffer (ADIOS_BUFFER_ALLOC_NOW, 1000);
        
        int64_t       m_adios_group;
        int64_t       m_adios_file;
        
        adios_declare_group (&m_adios_group, "restart", "", adios_flag_yes);
        adios_select_method (m_adios_group, adios_write_method.c_str(), initstr.c_str(), "");
        
        adios_define_var (m_adios_group, "NX"
                          ,"", adios_integer
                          ,0, 0, 0);
        
        adios_define_var (m_adios_group, "G"
                          ,"", adios_integer
                          ,0, 0, 0);
        
        adios_define_var (m_adios_group, "O"
                          ,"", adios_integer
                          ,0, 0, 0);
        
        adios_define_var (m_adios_group, "temperature"
                          ,"", adios_double
                          ,"NX", "G", "O");
        
        G = NX * size;
        if (rank==0) printf("NX = %d\n", NX);

        for (int it =0; it < 10; it++)
        {
            for (int i = 0; i < NX; i++)
                t[i] = rank + it + 1.0;
            
            MPI_Barrier(comm);
            double t_start = MPI_Wtime();
            
            adios_open (&m_adios_file, "restart", filename.c_str(), "a", comm);
            adios_groupsize = 4 + 4 + 4 + NX * 8;
            adios_group_size (m_adios_file, adios_groupsize, &adios_totalsize);
            
            adios_write(m_adios_file, "NX", (void *) &NX);
            adios_write(m_adios_file, "G", (void *) &G);
            O = rank * NX;
            adios_write(m_adios_file, "O", (void *) &O);
            adios_write(m_adios_file, "temperature", t);
            
            adios_close (m_adios_file);
            
            MPI_Barrier(comm);
            double t_end = MPI_Wtime();
            double t_elap = t_end - t_start;
            
            if (rank==0)
                printf("[%d] Wrote %'lld bytes, elapsed %.03f seconds, throughput %'.03f KB/sec\n",
                       it, adios_groupsize, t_elap, (double)adios_groupsize/t_elap/1024.0);
            
            sleep(interval_sec);
        }
        
        adios_finalize (rank);
    }
    else
    {
        ADIOS_FILE * f;
        ADIOS_VARINFO * v;
        ADIOS_SELECTION * sel;
        
        int steps = 0;
        int retval = 0;
        
        void * data = NULL;
        uint64_t start[2], count[2];
        
        adios_read_init_method (adios_read_method, comm, initstr.c_str());
        
        f = adios_read_open (filename.c_str(), adios_read_method,
                             comm, ADIOS_LOCKMODE_NONE, timeout_sec);
        if (adios_errno == err_file_not_found)
        {
            printf ("rank %d: Stream not found after waiting %f seconds: %s\n",
                    rank, timeout_sec, adios_errmsg());
            retval = adios_errno;
        }
        else if (adios_errno == err_end_of_stream)
        {
            printf ("rank %d: Stream terminated before open. %s\n", rank, adios_errmsg());
            retval = adios_errno;
        }
        else if (f == NULL) {
            printf ("rank %d: Error at opening stream: %s\n", rank, adios_errmsg());
            retval = adios_errno;
        }
        else
        {
            /* process file here... */
            v = adios_inq_var (f, "temperature");
            
            uint64_t slice_size = v->dims[0]/size;
            if (rank == size-1)
                slice_size = slice_size + v->dims[0]%size;
            
            start[0] = rank * slice_size;
            count[0] = slice_size;
            
            data = malloc (slice_size * sizeof(double));
            assert(data != NULL);
            
            /* Processing loop over the steps (we are already in the first one) */
            while (adios_errno != err_end_of_stream) {
                steps++; // steps start counting from 1
                
                sel = adios_selection_boundingbox (v->ndim, start, count);
                adios_schedule_read (f, sel, "temperature", 0, 1, data);
                adios_perform_reads (f, 1);
                
                double sum = 0.0;
                for (int i = 0; i < slice_size; i++)
                {
                    sum += *((double *)data + i);
                }
                
                printf("Step:%d, rank=%d: sum(data[%lld:%lld]) = %.01f\n",
                       f->current_step, rank, start[0], start[0]+count[0]-1, sum);
                
                // advance to 1) next available step with 2) blocking wait
                adios_advance_step (f, 0, timeout_sec);
                if (adios_errno == err_step_notready)
                {
                    printf ("rank %d: No new step arrived within the timeout. Quit. %s\n",
                            rank, adios_errmsg());
                    break; // quit while loop
                }
                
            }
            
            adios_read_close (f);
        }
        
        if (rank==0) 
            printf ("We have processed %d steps\n", steps);
        
        adios_read_finalize_method (adios_read_method);
        free (data);
    }


    MPI_Finalize ();
    return 1;
}

