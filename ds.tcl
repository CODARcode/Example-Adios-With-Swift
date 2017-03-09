namespace eval ds {

    proc ds_tcl { outputs inputs args } {
        set z [ lindex $outputs 0 ]
        set j [ lindex $inputs 0 ]
        set k [ lindex $inputs 1 ]
        set l [ lindex $inputs 2 ]
        rule $k "ds::ds_tcl_body $z $j $k $l" {*}$args type $turbine::WORK
    }

    proc ds_tcl_body { z j k l } {
        # Retrieve j
        set j_value [ retrieve_integer $j ]
        # Retrieve k
        set k_value [ retrieve_integer $k ]
        set l_value [ retrieve_integer $l ]
        # Look up MPI information
        set comm [ turbine::c::task_comm ]
        set rank [ adlb::rank $comm ]
        # Run the user code
        set z_value [ common_run_server $j_value $k_value $l_value $comm]
        # Store result
        if { $rank == 0 } {
            store_integer $z $z_value
        }
    }
}
