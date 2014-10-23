
This is an examle to demonstrate Adios ICEE method for WAN staging.

This program consists of two Adios executables; adios_write and
adios_read. adios_write will send data through TCP/IP to adios_read.

We assume the following TCP/IP connection:

       adios_write            adios_read
node:    host1                  host2
port:    59997                  59999


Execution is as follows. First, run the writer:

$ adios_write -w ICEE -h host1 -p 59997

Then, run the reader in another terminal:

$ adios_read -w ICEE -h host2 -p 59999 -s host1 -t 59997

