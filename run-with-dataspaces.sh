#!/bin/bash -ex
#./build.sh

rm -f *.bp*
./run-dataspaces.py -s 1 -c 4 --mpirun=aprun : \
    --nompi turbine -n 5 test-adios-staging.tic --wmethod=DATASPACES --rmethod=DATASPACES
