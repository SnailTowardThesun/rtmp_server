cmake_minimum_required(VERSION 2.6)

Project(RtmpServer)

# compile params
set(CMAKE_CXX_FLAGS "-g -std=c++11 -pthread -Wall")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (SRC_FILE_LISTS ${SRC_DIR}/main/rs_main_server.cpp src/common/rs_common.cpp)

# 3rdlibrary
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/googletest-release-1.8.0)
else()
    execute_process(COMMAND "tar" "xvf" "googletest-release-1.8.0.tar"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary
        )
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/googletest-release-1.8.0)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1)
else()
    execute_process(COMMAND "tar" "xvf" "libuv-v1.9.1.tar.gz"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary
        )

    # compile
    execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1/autogen.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1
        )
    execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1/configure
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1
        )
    execute_process(COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1
        )

endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1)

# common part
include_directories(${SRC_DIR}/common)
aux_source_directory(${SRC_DIR}/common SRC_FILE_LISTS)

# kernel part
include_directories(${SRC_DIR}/kernel)
aux_source_directory(${SRC_DIR}/kernel SRC_FILE_LISTS)

# protocol part
include_directories(${SRC_DIR}/protocol)
aux_source_directory(${SRC_DIR}/protocol SRC_FILE_LISTS)

# module part
include_directories(${SRC_DIR}/module)
aux_source_directory(${SRC_DIR}/module SRC_FILE_LISTS)

# objs
set (RS_SERVER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/objs/)
set (EXECUTABLE_OUTPUT_PATH ${RS_SERVER_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${RS_SERVER_BINARY_DIR})

# libuv
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1/include)

add_executable(rs_server ${SRC_FILE_LISTS})
target_link_libraries(rs_server ${CMAKE_CURRENT_SOURCE_DIR}/3rdlibrary/libuv-v1.9.1/.libs/libuv.a)

add_subdirectory(${SRC_DIR}/utest)
