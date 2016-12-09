cmake_minimum_required(VERSION 2.6)

Project(RtmpServer)

# compile params
set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (SRC_FILE_LISTS ${SRC_DIR}/main/rs_main_server.cpp)

# 3rdlibrary
include_directories(${SRC_DIR}/include)

# common part
include_directories(${SRC_DIR}/common)
aux_source_directory(${SRC_DIR}/common SRC_FILE_LISTS)

# objs
set (RS_SERVER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/objs/)
set (EXECUTABLE_OUTPUT_PATH ${RS_SERVER_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${RS_SERVER_BINARY_DIR})

add_executable(rs_server ${SRC_FILE_LISTS})


# utest
set(UTEST_SRC_FILE ${SRC_DIR}/utest/rs_utest_main.cpp)

add_executable(utest_rs_server ${UTEST_SRC_FILE})
target_link_libraries(utest_rs_server ${SRC_DIR}/libs/libgtest.a)
