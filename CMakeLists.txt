cmake_minimum_required (VERSION 2.8)
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules)
 
PROJECT (test-adios-icee)

#set(ADIOS_USE_MPI FALSE)
set(ADIOS_USE_MPI TRUE)
find_package (ADIOS REQUIRED)
add_definitions (${ADIOS_COMPILE_FLAGS})
include_directories (${ADIOS_INCLUDE_PATH})

find_package (MPI REQUIRED)
include_directories (${MPI_CXX_INCLUDE_PATH})

add_executable (adios_write
  adios_write.cpp
  ${HEADER_FILES})
target_link_libraries (adios_write
  ${ADIOS_LIBRARIES}
  ${MPI_CXX_LIBRARIES})

add_executable (adios_read
  adios_read.cpp
  ${HEADER_FILES})
target_link_libraries (adios_read
  ${ADIOS_LIBRARIES}
  ${MPI_CXX_LIBRARIES})
