#!/usr/bin/env python
import numpy as np
import argparse
import sys
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('logfile', nargs='+', help='logfile')
parser.add_argument('--filter', help='filter (e.g., ":,:", ":,2:")')
parser.add_argument('--tstep', help='timestep', type=int, default=0)
args = parser.parse_args()

def get_timinglines(fname):
    selected = []
    npe = 0
    nsteps = 0
    with open(fname) as f:
        lines = f.readlines()
        for line in lines:
            if line.startswith('>>>'):
                selected.append(line)
            elif line.strip().startswith('PEs'):
                npe = int(line.strip().split()[2])
            elif line.strip().startswith('Steps'):
                nsteps = int(line.strip().split()[2])

    elap = np.zeros((npe,nsteps), dtype=np.float)
    stamp = np.zeros((npe,nsteps), dtype=np.float)
    for line in selected:
        x = line.split()
        elap[int(x[3]), int(x[2])] = float(x[4])
        stamp[int(x[3]), int(x[2])] = float(x[1])

    return elap, stamp

elap, stamp = get_timinglines(args.logfile[0])
for logfile in args.logfile[1:]:
    elap_, stamp_ = get_timinglines(logfile)
    elap = np.concatenate((elap, elap_), 0)
    stamp = np.concatenate((stamp, stamp_), 0)

if args.filter is not None:
    elap = eval('elap[%s]' % args.filter)
    stamp = eval('stamp[%s]' % args.filter)

#plt.subplot(2, 1, 1)
p = plt.plot(elap)
plt.legend(p, range(elap.shape[1]), loc='best', fontsize='small')

#plt.subplot(2, 1, 2)
plt.figure(0)
idx = args.tstep
st = stamp[:,idx] - elap[:,idx]
ed = stamp[:,idx]
plt.errorbar(range(elap.shape[0]), ed, yerr=[elap[:,idx], (0,)*elap.shape[0]], fmt='none', elinewidth=2)
plt.title('Index=%r' % idx)
plt.show()
