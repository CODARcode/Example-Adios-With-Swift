#!/usr/bin/env python
import numpy as np
import argparse
import sys
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('logfile', help='logfile')
parser.add_argument('stagelog', nargs='?', help='stagelog')
parser.add_argument('--filter', help='filter (e.g., "::5", ":10"')
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

elap1, stamp1 = get_timinglines(args.logfile)

elap2 = np.zeros((0, elap1.shape[1]), dtype=elap1.dtype)
stamp2 = np.zeros((0, stamp1.shape[1]), dtype=stamp1.dtype)
if args.stagelog is not None:
    elap2, stamp2 = get_timinglines(args.stagelog)

elap = np.vstack((elap1,elap2))
stamp = np.vstack((stamp1,stamp2))
if args.filter is not None:
    elap = eval('elap[%s]' % args.filter)
    stamp = eval('stamp[%s]' % args.filter)

plt.subplot(2, 1, 1)
p = plt.plot(elap)
plt.legend(p, range(elap.shape[1]), loc='best', fontsize='small')

plt.subplot(2, 1, 2)
idx = args.tstep
st = stamp[:,idx] - elap[:,idx]
ed = stamp[:,idx]
plt.errorbar(range(elap.shape[0]), ed, yerr=[elap[:,idx], (0,)*elap.shape[0]], fmt='none', elinewidth=2)
plt.show()
