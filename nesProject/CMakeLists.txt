cmake_minimum_required(VERSION 3.8)
project(nesProject)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES central_unit.c)
add_executable(nesProject ${SOURCE_FILES})


include_directories(D:/Documenti/GitHub/nesProject/contiki/core)

add_custom_target(contiki_build COMMAND make all TARGET=native
                        CLION_EXE_DIR=${PROJECT_BINARY_DIR})