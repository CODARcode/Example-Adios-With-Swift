namespace eval adios_icee {

    proc main_leaf_tcl { outputs inputs args } {
        set z [ lindex $outputs 0 ]
        set A [ lindex $inputs 0 ]
        rule $A  "adios_icee::main_leaf_tcl_body $z $A" {*}$args type $turbine::WORK
    }

    proc main_leaf_tcl_body { z A} {
        # Look up MPI information
        set comm [ turbine::c::task_comm ]
        set rank [ adlb::rank $comm ]

        set length [ adlb::container_size $A ]
        set tds [ adlb::enumerate $A dict all 0 ]
        set argv [ list ]
        # Fill argv with blanks
        dict for { i v } $tds {
            lappend argv 0
        }
        # Set values at ordered list positions
        dict for { i v } $tds {
            lset argv $i $v
        }
        puts "argv=$argv"

        # Run the user code
        set z_value [ main_leaf $length $argv $comm ]
        # Store result
        if { $rank == 0 } {
            store_integer $z $z_value
        }
    }
}
