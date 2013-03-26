PROJECT(Ma_Solution)

cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 OLD)
# for installing targets 
SET(CMAKE_INSTALL_PREFIX ${MA_SOLUTION_BINARY_DIR})

IF (WIN32)
add_subdirectory(Plotter)
ENDIF (WIN32)

IF (UNIX)
add_subdirectory(Plotter)
ENDIF (UNIX)

