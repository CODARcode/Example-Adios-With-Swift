#!/bin/bash -ex

rm -f *.bp* conf
turbine -n 6 test-staging-all.tic --wmethod=DATASPACES --rmethod=DATASPACES
