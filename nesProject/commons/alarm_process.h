#ifndef NESPROJECT_ALARM_PROCESS_H
#define NESPROJECT_ALARM_PROCESS_H

#include "contiki.h"

extern process_event_t alarm_toggled_event;
extern unsigned char isAlarmOn;

PROCESS_NAME(alarm_process);

#endif //NESPROJECT_ALARM_PROCESS_H
