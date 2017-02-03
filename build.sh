#!/bin/bash -e

source $(turbine -S)

set -x

make lib
tclsh make-package.tcl > pkgIndex.tcl

stc -r $PWD test-adios-icee.swift
