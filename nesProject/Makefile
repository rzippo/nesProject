PROJECT_SOURCEFILES += \
    commons/addresses.c commons/alarm_process.c commons/lock.c commons/light.c \
    central_unit/cuRimeStack.c \
    gate/gateRimeStack.c gate/gateAutoOpeningProcess.c \
    door/doorRimeStack.c door/averageTemperatureProcess.c door/doorAutoOpeningProcess.c

CONTIKI = ../contiki
CONTIKI_WITH_RIME = 1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

include Makefile.include

mydirs:
	mkdir $(OBJECTDIR) -p
	mkdir $(OBJECTDIR)/commons -p
	mkdir $(OBJECTDIR)/gate -p
	mkdir $(OBJECTDIR)/door -p
	mkdir $(OBJECTDIR)/central_unit -p

gate: gate_node

cu: central_unit

door: door_node