# - Find a ADIOS implementation
#
# === Variables ===
#
# This module will set the following variables:
#   ADIOS_USE_NOMPI       Can be set to ON to force the use of the NOMPI library
#                         Defaults to OFF.
#   ADIOS_USE_FORTRAN     Can be set to ON to force the use of Fortran
#                         Defaults to OFF.
#   ADIOS_USE_READONLY    Can be set to ON to force the use of readonly library
#                         Defaults to OFF.
#   ADIOS_FOUND           TRUE if FindADIOS found ADIOS flags
#   ADIOS_COMPILE_FLAGS   Compilation flags for ADIOS programs
#   ADIOS_INCLUDE_DIRS    Include path(s) for ADIOS header
#   ADIOS_LINK_FLAGS      Linking flags for ADIOS programs
#   ADIOS_LIBRARIES       All libraries to link ADIOS programs against
#
# === Example ===
#
#   set (ADIOS_USE_NOMPI OFF)
#   find_package (ADIOS)
#   if (ADIOS_FOUND)
#     add_definitions (${ADIOS_COMPILE_FLAGS})
#     include_directories (${ADIOS_INCLUDE_DIRS})
#     target_link_libraries(foo ${ADIOS_LIBRARIES})
#   endif ()
#
#   set (ADIOS_USE_NOMPI ON)
#   set (ADIOS_USE_FORTRAN ON)
#   set (ADIOS_USE_READONLY ON)
#   find_package (ADIOS)
#   if (ADIOS_FOUND)
#     add_definitions (${ADIOS_COMPILE_FLAGS})
#     include_directories (${ADIOS_INCLUDE_DIRS})
#     target_link_libraries(foo ${ADIOS_LIBRARIES})
#   endif ()
#
# NOTE:
# This module is written based FindMPI. Most routines are copied from FindMPI.

# include this to handle the QUIETLY and REQUIRED arguments
include(FindPackageHandleStandardArgs)
include(GetPrerequisites)

if(NOT DEFINED ADIOS_USE_NOMPI)
  set(ADIOS_USE_NOMPI FALSE)
endif()

if(NOT DEFINED ADIOS_USE_FORTRAN)
  set(ADIOS_USE_FORTRAN FALSE)
endif()

if(NOT DEFINED ADIOS_USE_READONLY)
  set(ADIOS_USE_READONLY FALSE)
endif()

if (ADIOS_USE_NOMPI)
  list(APPEND CONFIG_OPTION "-s")
endif()

if (ADIOS_USE_FORTRAN)
  list(APPEND CONFIG_OPTION "-f")
endif()

if (ADIOS_USE_READONLY)
  list(APPEND CONFIG_OPTION "-r")
endif()
#message("CONFIG_OPTION=${CONFIG_OPTION}")

set(ARGS "-c")
list(APPEND ARGS ${CONFIG_OPTION})
execute_process(
  COMMAND adios_config ${ARGS}
  OUTPUT_VARIABLE ADIOS_COMPILE_CMDLINE OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# Extract compile flags from the compile command line
string(REGEX MATCHALL "(^| )-[Df]([^\" ]+|\"[^\"]+\")" ADIOS_ALL_COMPILE_FLAGS "${ADIOS_COMPILE_CMDLINE}")
set(ADIOS_COMPILE_FLAGS_WORK)

foreach(FLAG ${ADIOS_ALL_COMPILE_FLAGS})
  if (ADIOS_COMPILE_FLAGS_WORK)
    set(ADIOS_COMPILE_FLAGS_WORK "${ADIOS_COMPILE_FLAGS_WORK} ${FLAG}")
  else()
    set(ADIOS_COMPILE_FLAGS_WORK ${FLAG})
  endif()
endforeach()

# Extract include paths from compile command line
string(REGEX MATCHALL "(^| )-I([^\" ]+|\"[^\"]+\")" ADIOS_ALL_INCLUDE_PATHS "${ADIOS_COMPILE_CMDLINE}")
unset(ADIOS_INCLUDE_DIRS_WORK)
foreach(IPATH ${ADIOS_ALL_INCLUDE_PATHS})
  string(REGEX REPLACE "^ ?-I" "" IPATH ${IPATH})
  string(REGEX REPLACE "//" "/" IPATH ${IPATH})
  list(APPEND ADIOS_INCLUDE_DIRS_WORK ${IPATH})
endforeach()

set(ARGS "-l")
list(APPEND ARGS ${CONFIG_OPTION})
execute_process(
  COMMAND adios_config ${ARGS}
  OUTPUT_VARIABLE  ADIOS_LINK_CMDLINE OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# Extract linker paths from the link command line
string(REGEX MATCHALL "(^| |-Wl,)-L([^\" ]+|\"[^\"]+\")" ADIOS_ALL_LINK_PATHS "${ADIOS_LINK_CMDLINE}")
unset(ADIOS_LINK_PATH)
foreach(LPATH ${ADIOS_ALL_LINK_PATHS})
  string(REGEX REPLACE "^(| |-Wl,)-L" "" LPATH ${LPATH})
  string(REGEX REPLACE "//" "/" LPATH ${LPATH})
  list(APPEND ADIOS_LINK_PATH ${LPATH})
endforeach()

# try using showme:libdirs if extracting didn't work.
if (NOT ADIOS_LINK_PATH)
  set(ADIOS_LINK_PATH ${ADIOS_LIBDIRS})
  separate_arguments(ADIOS_LINK_PATH)
endif()

# Extract linker flags from the link command line
string(REGEX MATCHALL "(^| )-Wl,([^\" ]+|\"[^\"]+\")" ADIOS_ALL_LINK_FLAGS "${ADIOS_LINK_CMDLINE}")
set(ADIOS_LINK_FLAGS_WORK)
foreach(FLAG ${ADIOS_ALL_LINK_FLAGS})
  if (ADIOS_LINK_FLAGS_WORK)
    set(ADIOS_LINK_FLAGS_WORK "${ADIOS_LINK_FLAGS_WORK} ${FLAG}")
  else()
    set(ADIOS_LINK_FLAGS_WORK ${FLAG})
  endif()
endforeach()

# Extract the set of libraries to link against from the link command
# line
string(REGEX MATCHALL "(^| )-l([^\" ]+|\"[^\"]+\")" ADIOS_LIBNAMES "${ADIOS_LINK_CMDLINE}")

# Determine full path names for all of the libraries that one needs
# to link against in an MPI program
unset(ADIOS_LIBRARIES_WORK)
foreach(LIB ${ADIOS_LIBNAMES})
  string(REGEX REPLACE "^ ?-l" "" LIB ${LIB})
  # ADIOS_LIB is cached by find_library, but we don't want that.  Clear it first.
  set(ADIOS_LIB "ADIOS_LIB-NOTFOUND" CACHE FILEPATH "Cleared" FORCE)
  find_library(ADIOS_LIB NAMES ${LIB} HINTS ${ADIOS_LINK_PATH})

  if (ADIOS_LIB)
    list(APPEND ADIOS_LIBRARIES_WORK ${ADIOS_LIB})
  elseif (NOT ADIOS_FIND_QUIETLY)
    message(WARNING "Unable to find ADIOS library ${LIB}")
  endif()
endforeach()

# Sanity check ADIOS_LIBRARIES to make sure there are enough libraries
list(LENGTH ADIOS_LIBRARIES_WORK ADIOS_NUMLIBS)
list(LENGTH ADIOS_LIBNAMES ADIOS_NUMLIBS_EXPECTED)
if (NOT ADIOS_NUMLIBS EQUAL ADIOS_NUMLIBS_EXPECTED)
  message("Expected ${ADIOS_NUMLIBS_EXPECTED} but found ${ADIOS_NUMLIBS}")
  set(ADIOS_LIBRARIES_WORK "ADIOS_LIBRARIES-NOTFOUND")
endif()

# If we found MPI, set up all of the appropriate cache entries
set(ADIOS_COMPILE_FLAGS ${ADIOS_COMPILE_FLAGS_WORK} CACHE STRING "ADIOS compilation flags"         FORCE)
set(ADIOS_INCLUDE_DIRS  ${ADIOS_INCLUDE_DIRS_WORK}  CACHE STRING "ADIOS include path"              FORCE)
set(ADIOS_LINK_FLAGS    ${ADIOS_LINK_FLAGS_WORK}    CACHE STRING "ADIOS linking flags"             FORCE)
set(ADIOS_LIBRARIES     ${ADIOS_LIBRARIES_WORK}     CACHE STRING "ADIOS libraries to link against" FORCE)

find_package_handle_standard_args(ADIOS DEFAULT_MSG ADIOS_INCLUDE_DIRS ADIOS_LIBRARIES)
