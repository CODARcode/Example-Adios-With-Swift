CXX = mpicxx
CPPFLAGS = -g -DVERBOSE=5 -D_NX=1000

ADIOS_INC = $(shell adios_config -c)
ADIOS_LIB = $(shell adios_config -l)

all: adios_write adios_read

adios_write: adios_write.cpp
	$(CXX) $(CPPFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

adios_read: adios_read.cpp
	$(CXX) $(CPPFLAGS) $(ADIOS_INC) -o $@ $^ $(ADIOS_LIB)

clean:
	rm -f *.o *.*~ adios_write adios_read
