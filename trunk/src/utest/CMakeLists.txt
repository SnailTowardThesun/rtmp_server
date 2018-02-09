cmake_minimum_required(VERSION 2.6)

Project(utest)


set(CMAKE_CXX_FLAGS "-g -pthread -std=c++11 -Wall")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/googletest-release-1.8.0/googletest/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/googletest-release-1.8.0/googletest/)

add_library(gtest ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/googletest-release-1.8.0/googletest/src/gtest-all.cc)
add_library(gtest_main ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/googletest-release-1.8.0/googletest/src/gtest_main.cc)
target_link_libraries(gtest_main gtest)

set(UTEST_SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/rs_utest_main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} UTEST_SRC_FILE)

# common part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../common UTEST_SRC_FILE)

# kernel part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../kernel)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../kernel UTEST_SRC_FILE)

# protocol part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../protocol)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../protocol UTEST_SRC_FILE)

# module part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../module)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../module UTEST_SRC_FILE)

# 3rdparty
# libuv
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/libuv-v1.9.1/include)
# rapidjson
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/rapidjson-1.1.0/include)

add_executable(utest_rs_server ${UTEST_SRC_FILE})
target_link_libraries(utest_rs_server gtest_main)
target_link_libraries(utest_rs_server ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/libuv-v1.9.1/.libs/libuv.a)
