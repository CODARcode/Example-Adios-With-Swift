#!/bin/sh -ex

./build.sh

TURBINE_LOG=1 turbine -l -n 5 test-adios-icee.tic
