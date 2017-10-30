cmake_minimum_required(VERSION 3.8)
project(nesProject)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES central_unit.c door_node.c constants.h addresses.h addresses.c)
add_executable(nesProject ${SOURCE_FILES} door_node.c constants.h addresses.h addresses.c)


include_directories(D:/Documenti/GitHub/nesProject/contiki/core D:/Documenti/GitHub/nesProject/contiki)

add_custom_target(contiki_build COMMAND make all TARGET=native
                        CLION_EXE_DIR=${PROJECT_BINARY_DIR})