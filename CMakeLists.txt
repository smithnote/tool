cmake_minimum_required(VERSION 2.8)
project(tool C CXX)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "-D_FILE_OFFSET_BITS=64")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

MESSAGE(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})

set(THREAD_POOL_FILES thread_pool.cpp)
add_library(thread_pool SHARED ${THREAD_POOL_FILES})
install(TARGETS thread_pool DESTINATION lib)

set(CONNECTION_POOL_FILES connection_pool.cpp mysql_connection.cpp redis_connection.cpp)
add_library(connection_pool SHARED ${CONNECTION_POOL_FILES})
install(TARGETS connection_pool DESTINATION lib)


enable_testing()
add_subdirectory(unittest)
