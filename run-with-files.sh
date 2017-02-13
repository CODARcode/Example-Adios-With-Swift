#!/bin/sh -ex

./build.sh

TURBINE_LOG=1 turbine -n 5 test-adios-icee.tic
