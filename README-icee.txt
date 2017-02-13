
This is a test program to demonstrate Adios ICEE method for WAN staging.

== ICEE Initialization ==

In short, ICEE is sending information in a push way. I.e., once a
reader registers his connection information (hostname and port) to a
writer, the writer pushes data to the reader.

In ICEE, an initial connection is established between a writer and a
reader by the following steps:

1. Writer listens on a port, say P1, waiting a client (or multiple) to
register. Writer's listening port can be specified by "cm_port"
option when calling adios_select_method (when using Adios no xml init)
or in an xml file.

The following options can be used for initializing writer:

    cm_port : writer's listening port
    max_client : number of clients to register (default is 1)

2. A reader (or client) needs to know the writer's hostname and
port. Then, the reader connects to the writer and registers his
connection information (reader's hostname and port).

The following options can be specified on calling
adios_read_init_method:

    cm_host : reader's hostname
    cm_port : reader's port
    cm_remote_host : writer's hostname
    cm_remote_port : writer's port

A reader can register to multiple writers to receive data from
multiple sources. Then, use the following option:

    remote_list : specify multiple writer's location in the format:
    hostname1:port1,hostname2:port2,...

3. After successful registration, the writer will push data to the reader.

Note: In "passive" mode (--passive option), only one port (server's
port) needs to be open. A reader connects to a server and then the
server communicates through the channel. It can be useful in a case
where one side has a strict firewall setting.

== Example ==

This program adios_icee can work as a server (by default) and a
client (with -c option).

Case I: 1-to-1 connection (one writer and one reader)

We assume the following TCP/IP connection:

      server (writer)       client (reader)
node:    host1                  host2
port:    59997                  59999

Execution is as follows. First, run a server:

$ adios_icee -w ICEE -p 59997

Then, run a client in another terminal:

$ adios_icee -c -r ICEE -s host1 -t 59997 -p 59999

With a passive mode, client has no need to privde a port number:
$ adios_icee -w ICEE -p 59997 --passive
$ adios_icee -c -r ICEE -s host1 -t 59997 --passive


