cmake_minimum_required(VERSION 3.29)
project(traffic C)
set(CMAKE_C_STANDARD 23)

add_executable(traffic src/main.c assets/plc.h)
target_link_libraries(traffic PRIVATE raylib)
