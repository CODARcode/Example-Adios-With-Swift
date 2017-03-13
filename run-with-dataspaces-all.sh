#!/bin/bash -ex

rm -f *.bp* conf
turbine -n 7 test-staging-all.tic --wmethod=DIMES --rmethod=DIMES
