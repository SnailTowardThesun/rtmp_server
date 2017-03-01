cmake_minimum_required(VERSION 2.6)

Project(test)


set(CMAKE_CXX_FLAGS "-g -pthread -std=c++11 -Wall")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/googletest-release-1.8.0/googletest/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/googletest-release-1.8.0/googletest/)

set(TEST_SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/rs_test_main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC_FILE)

# common part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../common TEST_SRC_FILE)

# kernel part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../kernel)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../kernel TEST_SRC_FILE)

# protocol part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../protocol)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../protocol TEST_SRC_FILE)

# module part
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../module)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../module TEST_SRC_FILE)

# 3rdparty
# libuv
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/libuv-v1.9.1/include)

add_executable(test_rs_server ${TEST_SRC_FILE})
target_link_libraries(test_rs_server gtest_main)
target_link_libraries(test_rs_server ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdlibrary/libuv-v1.9.1/.libs/libuv.a)
