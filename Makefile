CXX = mpicxx
CPPFLAGS = -g

ADIOS_INC = $(shell adios_config -c)
ADIOS_LIB = $(shell adios_config -l)

.PHONE: all clean

all: adios_write adios_read

adios_write: adios_write.cpp
	$(CXX) $(CPPFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

adios_read: adios_read.cpp cmdline.c
	$(CXX) $(CPPFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

cmdline.c: icee_cmdline.ggo
	gengetopt --input=$^

clean:
	rm -f cmdline.c cmdline.h *.o *.*~ adios_write adios_read
