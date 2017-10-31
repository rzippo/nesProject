cmake_minimum_required(VERSION 3.8)
project(nesProject)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        central_unit.c
        door_node.c
        constants.h
        addresses.h
        addresses.c
        alarm_process.h
        alarm_process.c
        gate_node.c
        gateRimeStack.c
        gateRimeStack.h
        cuRimeStack.c
        cuRimeStack.h
        doorRimeStack.c
        doorRimeStack.h

        )
add_executable(nesProject ${SOURCE_FILES} door_node.c constants.h addresses.h addresses.c)


include_directories(../contiki/core ../contiki)

add_custom_target(contiki_build COMMAND make all TARGET=native
                        CLION_EXE_DIR=${PROJECT_BINARY_DIR})
