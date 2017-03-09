#!/bin/bash -e

source $(turbine -S)

set -x

export TCL_INCLUDE_SPEC=$TCL_INCLUDE_SPEC
export TCL_LIB_SPEC=$TCL_LIB_SPEC

make lib
make dslib
tclsh make-package.tcl > pkgIndex.tcl
tclsh make-package-ds.tcl >> pkgIndex.tcl

stc -r $PWD test-adios-icee.swift
stc -r $PWD test-adios-staging.swift
stc -r $PWD test-staging-all.swift
