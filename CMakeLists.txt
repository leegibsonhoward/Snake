cmake_minimum_required(VERSION 2.6)
project(snake)

file(GLOB SOURCES "src/*.c")

add_executable(snake ${SOURCES})

#(TODO): portability
# As of now only support build for linux. the
# directory below is where the library is located
# on Ubuntu 20.04
target_link_libraries(snake /usr/lib/x86_64-linux-gnu/liballeg.so)

target_compile_options(snake PRIVATE -Wall -Wextra)