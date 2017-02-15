#!/usr/bin/env python

'''
This is a wrapper command for running dataspaces_server with multiple applications.

The general form of this comand is as follows:
$ run-dataspaces.py SERVER_COMMAND [ : APPLICATION_COMMAND ] *

More details of options will be printed with '-h' option:
$ run-dataspaces.py -h
'''

import sys
import argparse
import subprocess
import os
import time
import logging

def cmdlist(argv):
    '''
    return list of list
    '''
    cmds = list()
    _args = list()
    for x in argv:
        if x == ':':
            cmds.append(_args)
            _args = list()
        else:
            _args.append(x)

    cmds.append(_args)
    _args = list()

    return cmds

def getstds(args):
    if args.oe is None:
        f_stdout = open(args.stdout, 'w') if args.stdout is not None else None
        f_stderr = open(args.stderr, 'w') if args.stdout is not None else None
    else:
        f = open(args.oe, 'w')
        f_stdout = f
        f_stderr = f
    return (f_stdout, f_stderr)

def main():
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')
    def usage():
        print ('USAGE: run-dataspaces.py <SERVER_COMMAND> [ : <APP_COMMAND> ]*')
        print ('====================')
        parser_svr.print_help()
        print ('--------------------')
        parser_cmd.print_help()
        print ('--------------------')
        sys.exit()

    ##import ipdb; ipdb.set_trace()
    nserver = 0
    nclient = 0
    parser_svr = argparse.ArgumentParser(prog='SERVER_COMMAND', add_help=False)
    parser_svr.add_argument('--nserver', '-s', help='num. of servers', type=int, default=1)
    parser_svr.add_argument('--nclient', '-c', help='num. of clients (will overwrite estimated number)', type=int, default=0)
    parser_svr.add_argument('--mpirun', help='mpirun command', default='mpirun')
    parser_svr.add_argument('--stdout', '-o', help='stdout')
    parser_svr.add_argument('--stderr', '-e', help='stderr')
    parser_svr.add_argument('--oe', help='merging stdout and stderr')
    parser_svr.add_argument('--dryrun', action='store_true', help='dryrun')

    parser_cmd = argparse.ArgumentParser(prog='APP_COMMAND', add_help=False)
    parser_cmd.add_argument('--np', '-n', help='num. of processes', type=int, default=1)
    parser_cmd.add_argument('CMDS', help='app commands', nargs=argparse.REMAINDER)
    parser_cmd.add_argument('--stdout', '-o', help='stdout')
    parser_cmd.add_argument('--stderr', '-e', help='stderr')
    parser_cmd.add_argument('--oe', help='merging stdout and stderr')
    parser_cmd.add_argument('--nompi', action='store_true', help='no mpirun')

    cmds = cmdlist(sys.argv[1:])
    if len(cmds) < 2:
        usage()

    args, _unknown = parser_svr.parse_known_args(cmds[0])
    nserver = args.nserver
    if len(_unknown) > 0:
        usage()

    args_cmd_list = list()
    for cmd in cmds[1:]:
        args_cmd, _unknown = parser_cmd.parse_known_args(cmd)
        setattr(args_cmd, 'UNKNOWN', _unknown)
        nclient += args_cmd.np
        args_cmd_list.append(args_cmd)

    if args.nclient > 0:
        nclient = args.nclient

    # Check config file and remove previous conf file
    if not os.path.exists('dataspaces.conf'):
        logging.error('Error: no config file (dataspaces.conf). Exit.')
        sys.exit()

    os.remove('conf') if os.path.isfile('conf') else None

    # Run server
    ds_cmd = '%(mpirun)s -n %(nserver)d dataspaces_server -s%(nserver)d -c%(nclient)d' % \
        {'mpirun':args.mpirun, 'nserver':args.nserver, 'nclient':nclient}

    logging.debug('CMD: %s' % ds_cmd)
    if not args.dryrun:
        f_stdout, f_stderr = getstds(args)
        p0 = subprocess.Popen(ds_cmd.split(),
                stdout=f_stdout, stderr=f_stderr,
                close_fds=True)

    if not args.dryrun:
        while not os.path.exists('conf'):
            time.sleep(5)

        logging.debug('dataspaces_server is ready.')

        env = os.environ.copy()
        with open('conf') as f:
            lines = f.readlines()
            for ln in lines:
                try:
                    k, v = ln.rstrip('\n').split('=')
                    env[k] = v
                except:
                    pass

        logging.info('P2TNID=%s' % env['P2TNID'])
        logging.info('P2TPID=%s' % env['P2TPID'])

    # Run client
    for a in args_cmd_list:
        if not a.nompi:
            cl_cmd = ' '.join(['%(mpirun)s -n %(np)d' % {'mpirun':args.mpirun, 'np':a.np}, ' '.join(a.UNKNOWN), ' '.join(a.CMDS)])
        else:
            cl_cmd = ' '.join([' '.join(a.CMDS)])

        logging.debug('CMD: %s' % cl_cmd)
        if not args.dryrun:
            f_stdout, f_stderr = getstds(a)
            p1 = subprocess.Popen(cl_cmd.split(), env=env,
                    stdout=f_stdout, stderr=f_stderr,
                    close_fds=True)

    if not args.dryrun:
        p0.wait()
    logging.info('Done.')

if __name__ == '__main__':
    main()
