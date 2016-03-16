#!/usr/bin/env python
import numpy as np
import argparse
import sys
import locale

locale.setlocale( locale.LC_ALL, 'en_US.UTF-8' )

parser = argparse.ArgumentParser()
parser.add_argument('logfile', nargs='+', help='logfile')
parser.add_argument('--filter', help='filter (e.g., ":,:", ":,2:")')
parser.add_argument('--step', help='timestep', type=int, default=0)
parser.add_argument('--rank', help='rank', type=int, default=0)
parser.add_argument('--ymin', help='ymin', type=float)
parser.add_argument('--display', action='store_true')
parser.add_argument('--summary', action='store_true', default=True)
parser.add_argument('--byrow', action='store_true', default=False)
args = parser.parse_args()

ymin = None if args.ymin is None else args.ymin
tid = args.step
pid = args.rank

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
                npe = locale.atoi(line.strip().split()[2])
            elif line.strip().startswith('Steps'):
                nsteps = locale.atoi(line.strip().split()[2])

    elap = np.zeros((nsteps,npe), dtype=np.float)
    stamp = np.zeros((nsteps,npe), dtype=np.float)
    thrp = np.zeros((nsteps,npe), dtype=np.float)
    for line in selected:
        x = line.split()
        elap[int(x[2]), int(x[3])] = float(x[4])
        stamp[int(x[2]), int(x[3])] = float(x[1])
        thrp[int(x[2]), int(x[3])] = locale.atof(x[5])

    return elap, stamp, thrp

def print_summary(fname):
    print 'Logfilename:', fname
    with open(fname) as f:
        lines = f.readlines()
        verbose = False
        for line in lines:
            if line.startswith('====='):
                verbose = not verbose
                if not verbose:
                    print line
                    break

            if (verbose):
                print line.rstrip()

for logfile in args.logfile:
    print_summary(logfile)

elap, stamp, thrp = get_timinglines(args.logfile[0])
for logfile in args.logfile[1:]:
    elap_, stamp_, thrp_ = get_timinglines(logfile)
    elap = np.concatenate((elap, elap_), 1)
    stamp = np.concatenate((stamp, stamp_), 1)
    thrp = np.concatenate((thrp, thrp_), 1)

if args.filter is not None:
    elap = eval('elap[%s]' % args.filter)
    stamp = eval('stamp[%s]' % args.filter)
    thrp = eval('thrp[%s]' % args.filter)

if args.summary:
    nsteps = elap.shape[0]
    if args.byrow:
        print '%9s %9s %9s %9s %9s %9s' % ('SEQ', 'Time(s)', 'AVG', 'STD', 'MIN', 'MAX')
        for t in range(nsteps):
            print '%9d' % t, \
                '%9.3f' % elap[t,0], \
                '%9.3f' % np.mean(elap[t,:]), \
                '%9.3f' % np.std(elap[t,:]), \
                '%9.3f' % np.min(elap[t,:]), \
                '%9.3f' % np.max(elap[t,:])

        print '%9s' % '---'
        print '%9s' % 'AVG', \
            '%9.3f' % np.mean(elap[:,0]), \
            '%9.3f' % np.mean(elap[:,:])
        print '%9s' % 'STD', \
            '%9.3f' % np.std(elap[:,0]), \
            '%9.3f' % np.std(elap[:,:])
    else:
        print '%7s' % 'SEQ', ' '.join(map(lambda x: '%7s' % ('#'+str(x)), range(nsteps))), \
            '%7s' % 'AVG', '%7s' % 'STD'
        print '%7s' % 'Time', ' '.join(map(lambda x: '%7.3f'%x, elap[:,0])), \
            '%7.3f' % np.mean(elap[:,0]), '%7.3f' % np.std(elap[:,0]) 
        print '%7s' % 'AVG', ' '.join(map(lambda x: '%7.3f'%x, np.mean(elap, 1))), \
            '%7.3f' % np.mean(elap[:,:]), '%7.3f' % np.std(elap[:,:])             
        print '%7s' % 'STD', ' '.join(map(lambda x: '%7.3f'%x, np.std(elap, 1)))
        print '%7s' % 'MIN', ' '.join(map(lambda x: '%7.3f'%x, np.min(elap, 1)))
        print '%7s' % 'MAX', ' '.join(map(lambda x: '%7.3f'%x, np.max(elap, 1)))
        

if args.display:
    import matplotlib.pyplot as plt

    plt.figure(1)
    p = plt.plot(elap)
    if ymin is not None:
        plt.ylim((ymin ,plt.ylim()[1]))
        plt.legend(p, range(elap.shape[1]), loc='best', fontsize='small')
        plt.xlabel('Timestep')

    plt.figure(2)
    p = plt.plot(elap.T)
    if ymin is not None:
        plt.ylim((ymin ,plt.ylim()[1]))
    plt.legend(p, range(elap.shape[0]), loc='best', fontsize='small')
    plt.xlabel('Rank')

    plt.figure(3)
    st = stamp[tid,:] - elap[tid,:]
    ed = stamp[tid,:]
    plt.errorbar(range(elap.shape[1]), ed, yerr=[elap[tid,:], (0,)*elap.shape[1]], fmt='none', elinewidth=2)
    plt.title('Step=%r' % tid)
    plt.xlabel('Rank')

    plt.figure(4)
    plt.hist(np.ravel(thrp), 20)
    
    plt.show()
