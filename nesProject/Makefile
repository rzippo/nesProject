PROJECT_SOURCEFILES += \
    commons/addresses.c commons/alarm_process.c commons/lock.c commons/light.c commons/command_process.c \
    central_unit/cuRimeStack.c \
    gate/gateRimeStack.c gate/gateAutoOpeningProcess.c \
    door/doorRimeStack.c door/averageTemperatureProcess.c door/doorAutoOpeningProcess.c \
	  roomLight/roomLightRimeStack.c \
	  mailbox/mboxRimeStack.c \

CONTIKI = ../contiki
CONTIKI_WITH_RIME = 1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

include ../contiki/Makefile.include

mydirs:
	mkdir $(OBJECTDIR) -p
	mkdir $(OBJECTDIR)/commons -p
	mkdir $(OBJECTDIR)/gate -p
	mkdir $(OBJECTDIR)/door -p
	mkdir $(OBJECTDIR)/central_unit -p
	mkdir $(OBJECTDIR)/roomLight -p
	mkdir $(OBJECTDIR)/mailbox -p

gate: mydirs gate_node

cu: mydirs central_unit

door: mydirs door_node

roomLight: mydirs roomLight_node