CC = mpicc
CXX = mpicxx
CFLAGS = -g

ADIOS_INC = $(shell adios_config -c)
ADIOS_LIB = $(shell adios_config -l)

INCS = $(ADIOS_INC)
LIBS = $(ADIOS_LIB)

.PHONE: all clean ggo

all: adios_icee

.c.o: 
	$(CC) $(CFLAGS) $(INCS) -c $<

.cpp.o:
	$(CXX) $(CFLAGS) $(INCS) -c $<

adios_write: adios_write.cpp
	$(CXX) $(CFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

adios_read: adios_read.cpp icee_cmdline.c
	$(CXX) $(CFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

adios_icee: adios_icee.o icee_cmdline.o
	$(CXX) $(CFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

ggo: 
	gengetopt --input=icee_cmdline.ggo

clean:
	rm -f *.o *.*~ adios_write adios_read adios_icee
