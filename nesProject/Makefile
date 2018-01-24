PROJECT_SOURCEFILES += \
    addresses.c alarm_process.c cuRimeStack.c gateRimeStack.c doorRimeStack.c \
    averageTemperatureProcess.c doorAutoOpeningProcess.c gateAutoOpeningProcess.c \
    lock.c

CONTIKI = ../contiki
CONTIKI_WITH_RIME = 1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

include Makefile.include


gate: gate_node

cu: central_unit

door: door_node


