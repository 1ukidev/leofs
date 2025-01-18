cmake_minimum_required(VERSION 3.25)

project(leofs LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall --pedantic -g")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(FUSE REQUIRED)

include_directories(${FUSE_INCLUDE_DIR})
add_executable(leofs src/main.c)
target_link_libraries(leofs ${FUSE_LIBRARIES})
