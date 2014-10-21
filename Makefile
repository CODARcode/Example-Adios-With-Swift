CC = mpicc
FC = mpif90

ADIOS_INC = $(shell adios_config -c)
ADIOS_LIB = $(shell adios_config -l)

all: adios_write adios_read

adios_write: adios_write.c
	$(CC) -g $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

adios_read: adios_read.c
	$(CC) -g $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

clean:
	rm -f *.o *.*~ adios_write adios_read
