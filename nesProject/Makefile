PROJECT_SOURCEFILES += \
    commons/addresses.c commons/alarm_process.c commons/lock.c commons/light.c \
    central_unit/cuRimeStack.c \
    gate/gateRimeStack.c gate/gateAutoOpeningProcess.c \
    door/doorRimeStack.c door/averageTemperatureProcess.c door/doorAutoOpeningProcess.c

CONTIKI = ../contiki
CONTIKI_WITH_RIME = 1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

include Makefile.include

<<<<<<< HEAD
mydirs:
	mkdir $(OBJECTDIR) -p
	mkdir $(OBJECTDIR)/commons -p
	mkdir $(OBJECTDIR)/gate -p
	mkdir $(OBJECTDIR)/door -p
	mkdir $(OBJECTDIR)/central_unit -p
	mkdir $(OBJECTDIR)/light_controller -p

=======
>>>>>>> d49337e361278da3a0379f27433f8c898bd9b1f6
gate: gate_node

cu: central_unit

<<<<<<< HEAD


=======
door: door_node
>>>>>>> d49337e361278da3a0379f27433f8c898bd9b1f6
