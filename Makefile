CC = mpicc
CXX = mpicxx
CFLAGS = -g -DVERSION=$(shell git describe --abbrev=7 --dirty --always --tags) -fPIC
LDFLAGS =

ADIOS_INC = $(shell adios_config -c)
ADIOS_LIB = $(shell adios_config -l)

TCL_INCLUDE_SPEC ?= -I/opt/local/include
TCL_LIB_SPEC ?= -L/opt/local/lib -ltcl

INCS = $(ADIOS_INC)
LIBS = $(ADIOS_LIB)

ifneq (,${HOST})
  SYSTEMS := ${HOST}
else
  SYSTEMS := $(shell hostname)
endif

ifneq (,$(findstring titan, $(SYSTEMS)))
  CC = cc
  CXX = CC
endif

ifneq (,$(findstring cori, $(SYSTEMS)))
  CC = cc
  CXX = CC
  LDFLAGS += -zmuldefs
endif

ifneq (,$(findstring sith, $(SYSTEMS)))
  LDFLAGS += -Wl,--allow-multiple-definition
endif

.PHONE: all clean ggo

all: adios_icee

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $<

.cpp.o:
	$(CXX) $(CFLAGS) $(INCS) -c $<

.cxx.o:
	$(CXX) $(CFLAGS) $(INCS) -c $<

adios_write: adios_write.cpp
	$(CXX) $(CFLAGS) $(INCS) -o $@ $^ $(LIBS)

adios_read: adios_read.cpp icee_cmdline.c
	$(CXX) $(CFLAGS) $(INCS) -o $@ $^ $(LIBS)

adios_icee: adios_icee.o icee_cmdline.o filelock.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

lib: adios_icee.o filelock.o icee_cmdline.o
	swig -c++ adios_icee.i
	$(CXX) -c -fPIC $(TCL_INCLUDE_SPEC) adios_icee_wrap.cxx
	$(CXX) -shared -o libadios_icee.so adios_icee_wrap.o adios_icee.o filelock.o icee_cmdline.o $(TCL_LIB_SPEC) $(LIBS)

common.o: common.c
	$(CC) -c -fPIC -I . $(DATASPACES_INC) common.c

dslib: common.o
	swig ds.i
	$(CC) -c -fPIC $(TCL_INCLUDE_SPEC) ds_wrap.c
	$(CC) -shared -o libds.so common.o ds_wrap.o $(DATASPACES_LIB) -ldspaces -ldscommon -ldart

ggo:
	gengetopt --input=icee_cmdline.ggo --no-handle-version

clean:
	rm -f *.o *.*~ adios_write adios_read adios_icee
