cmake_minimum_required (VERSION 2.8)
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules)
 
PROJECT (test-adios-icee)

#set(ADIOS_USE_MPI FALSE)
set(ADIOS_USE_MPI TRUE)
find_package (ADIOS REQUIRED)
add_definitions (${ADIOS_COMPILE_FLAGS})
include_directories (${ADIOS_INCLUDE_DIRS})
message("ADIOS_INCLUDE_DIRS=${ADIOS_INCLUDE_DIRS}")
message("ADIOS_LIBRARIES=${ADIOS_LIBRARIES}")


find_package (MPI REQUIRED)
include_directories (${MPI_CXX_INCLUDE_PATH})

add_executable (adios_icee
  adios_icee.cpp
  icee_cmdline.c
  filelock.c
  ${HEADER_FILES})
target_link_libraries (adios_icee
  ${ADIOS_LIBRARIES}
  ${MPI_CXX_LIBRARIES})
