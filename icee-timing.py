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
parser.add_argument('--display', help='xwin display', action='store_true', default=False)
parser.add_argument('--save', help='save images', action='store_true', default=True)
parser.add_argument('--summary', help='print summary', action='store_true', default=True)
parser.add_argument('--byrow', help='print summary by rows', action='store_true', default=False)
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
    elap[:] = np.nan
    stamp[:] = np.nan
    thrp[:] = np.nan

    for line in selected:
        x = line.split()
        elap[int(x[2]), int(x[3])] = float(x[4])
        stamp[int(x[2]), int(x[3])] = float(x[1])
        thrp[int(x[2]), int(x[3])] = locale.atof(x[5]) ##local:5, global:6

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
                '%9.3f' % np.nanmean(elap[t,:]), \
                '%9.3f' % np.nanstd(elap[t,:], ddof=1), \
                '%9.3f' % np.nanmin(elap[t,:]), \
                '%9.3f' % np.nanmax(elap[t,:])

        print '%9s' % '---'
        print '%9s' % 'AVG', \
            '%9.3f' % np.nanmean(elap[:,0]), \
            '%9.3f' % np.nanmean(elap[:,:])
        print '%9s' % 'STD', \
            '%9.3f' % np.nanstd(elap[:,0], ddof=1), \
            '%9.3f' % np.nanstd(elap[:,:], ddof=1)
    else:
        print '%7s' % 'SEQ', ' '.join(map(lambda x: '%7s' % ('#'+str(x)), range(nsteps))), \
            '%7s' % 'AVG', '%7s' % 'STD'
        print '%7s' % 'Time', ' '.join(map(lambda x: '%7.3f'%x, elap[:,0])), \
            '%7.3f' % np.nanmean(elap[:,0]), '%7.3f' % np.nanstd(elap[:,0], ddof=1)
        print '%7s' % 'AVG', ' '.join(map(lambda x: '%7.3f'%x, np.nanmean(elap, 1))), \
            '%7.3f' % np.nanmean(elap[:,:]), '%7.3f' % np.nanstd(elap[:,:], ddof=1)
        print '%7s' % 'STD', ' '.join(map(lambda x: '%7.3f'%x, np.nanstd(elap, 1, ddof=1)))
        print '%7s' % 'MIN', ' '.join(map(lambda x: '%7.3f'%x, np.nanmin(elap, 1)))
        print '%7s' % 'MAX', ' '.join(map(lambda x: '%7.3f'%x, np.nanmax(elap, 1)))


if args.save:
    import matplotlib as mpl
    import matplotlib.pyplot as plt
    from matplotlib.ticker import FormatStrFormatter
    from matplotlib import ticker
    import matplotlib.mlab as mlab
    import os

    mpl.rcParams['figure.figsize'] = (5.0, 3.75) # Original: 8x6

    def to_percent(y, position):
        ## Ignore the passed in position. This has the effect of scaling the default
        ## tick locations.
        s = str(100 * y)

        # The percent symbol needs escaping in latex
        if mpl.rcParams['text.usetex'] is True:
            return s + r'$\%$'
        else:
            return s + '%'

    def plot_hist(x):
        f, (ax_box, ax_hist) = plt.subplots(2, sharex=True, \
                                            gridspec_kw={"height_ratios": (.15, .85)})
        bp = ax_box.boxplot(x, vert=False)
        h = ax_hist.hist(x, bins=20, weights=np.ones_like(x)/x.size*100.)

        ## confirm that the axes line up
        xlims = np.array([ax_box.get_xlim(), ax_hist.get_xlim()])
        for ax in [ax_box, ax_hist]:
            ax.set_xlim([xlims.min(), xlims.max()])

        ##ax_box.set_xticklabels([])  # clear out overlapping xlabels
        ax_box.set_yticks([])  # don't need that 1 tick mark

        f.subplots_adjust(hspace=0)
        plt.setp([a.get_xticklabels() for a in f.axes[:-1]], visible=False)

    def reject_outliers(data, m=2):
        return data[abs(data - np.mean(data)) < m * np.std(data, ddof=1)]

    prefix = os.path.splitext(os.path.basename(args.logfile[0]))[0]
    if len(args.logfile) > 1:
        prefix = '%s+%dmore'%(prefix, len(args.logfile)-1)

    plt.figure(1)
    p = plt.plot(elap)
    if ymin is not None:
        plt.ylim((ymin, plt.ylim()[1]))
    plt.legend(p, range(elap.shape[1]), loc='center left', bbox_to_anchor=(1,.5), fontsize='small', title='Rank')
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
    plt.xlabel('Timestep')
    plt.ylabel('Time (s)')

    plt.figure(2)
    p = plt.plot(elap.T)
    if ymin is not None:
        plt.ylim((ymin, plt.ylim()[1]))
    plt.legend(p, range(elap.shape[0]), loc='center left', bbox_to_anchor=(1,.5), fontsize='small', title='Timestep')
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
    plt.xlabel('Rank')
    plt.ylabel('Time (s)')

    plt.figure(3)
    st = stamp[tid,:] - elap[tid,:]
    ed = stamp[tid,:]
    plt.errorbar(range(elap.shape[1]), ed, yerr=[elap[tid,:], (0,)*elap.shape[1]], fmt='none', elinewidth=2)
    plt.title('Step=%r' % tid)
    plt.xlabel('Rank')
    plt.ylabel('Timeline (s)')

    fig = plt.figure(4)
    x = np.ravel(thrp)
    #x = np.ravel(elap)
    x = x[~np.isnan(x)]
    x = reject_outliers(x, m=3)
    #n, bins, patches = ax.hist(x, bins=20, weights=np.ones_like(x)/x.size*100.)
    n, bins, patches = plt.hist(x, bins=20, normed=1)
    bincenters = 0.5*(bins[1:]+bins[:-1])
    #y = mlab.normpdf(bincenters, np.mean(x), np.std(x))*100.
    y = mlab.normpdf(bincenters, np.mean(x), np.std(x, ddof=1))
    l = plt.plot(bincenters, y, 'r--', linewidth=2)

    formatter = ticker.ScalarFormatter(useMathText=True)
    formatter.set_scientific(True)
    formatter.set_powerlimits((-1,1))
    plt.gca().yaxis.set_major_formatter(formatter)

    #formatter = FuncFormatter(to_percent)
    #plt.gca().yaxis.set_major_formatter(formatter)
    #ax.yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
    #plt.ylabel('Frequency (%)')
    plt.xlabel('I/O Throughput (MiB/s)')
    plt.ylabel('Probability')
    #plt.grid(True)
    plt.savefig(prefix+'-fig4.pdf', bbox_inches='tight')
    plt.savefig(prefix+'-fig4.png', bbox_inches='tight')

    plt.figure(5)
    plt.errorbar(np.arange(thrp.shape[0]), np.nanmean(thrp, 1), yerr=np.nanstd(thrp, 1, ddof=1))
    if ymin is not None:
        plt.ylim((ymin, plt.ylim()[1]))
    plt.xlim((plt.xlim()[0]-1, plt.xlim()[1]))
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
    plt.xlabel('Timestep')
    plt.ylabel('I/O Throughput (MiB/s)')
    plt.savefig(prefix+'-fig5.pdf', bbox_inches='tight')
    plt.savefig(prefix+'-fig5.png', bbox_inches='tight')

    if args.display: plt.show()
