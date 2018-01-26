
#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "commons/constants.h"
#include "net/rime/rime.h"
#include "commons/addresses.h"
#include "dev/sht11/sht11-sensor.h"

#include "door/doorRimeStack.h"
#include "commons/alarm_process.h"
#include "commons/lock.h"
#include "door/doorAutoOpeningProcess.h"
#include "door/averageTemperatureProcess.h"

PROCESS(door_node_main, "Door Node Main Process");

AUTOSTART_PROCESSES(&door_node_main, &alarm_process, &averageTemperatureProcess);

void processCUCommand(unsigned char command)
{
	if( command == ALARM_TOGGLE_COMMAND )
	{
		if(!isAlarmOn)
		{
			process_post_synch(&doorAutoOpeningProcess, alarm_toggled_event, NULL);
			process_post_synch(&alarm_process, alarm_toggled_event, NULL);
		}
		else
		{
			process_post_synch(&alarm_process, alarm_toggled_event, NULL);
			process_post_synch(&doorAutoOpeningProcess, alarm_toggled_event, NULL);
		}
	}
	else
	{
		if(isAlarmOn)
		{
			printf("Alarm is ON: command %c refused\n", command);
		}
		else
		{
			switch(command)
			{
				case DOORS_OPEN_COMMAND:
				{
					process_start(&doorAutoOpeningProcess, NULL);
					break;
				}
				
				case AVERAGE_TEMPERATURE_COMMAND:
				{	
					printf("Average temp is: %d\n",
						   (int) averageTemperature);

					//1 byte for cmd, 4 bytes for payload size, 
					//4 bytes for float
					unsigned char buff[9];

					//1byte for cmd, 4 bytes for float
					int* payloadSize = (int*)buff;
					*payloadSize = 5;

					*(buff+4) = AVERAGE_TEMPERATURE_COMMAND;

					float* floatBuff = (float*)(buff+5);
					*floatBuff = (float)averageTemperature;

                    sendFromDoorToCentralUnit(buff, 9);

					break;
				}
				
				default:
					printf("Command %d not recognized from this node\n", command);
					break;
			}
		}
	}
}

PROCESS_THREAD(door_node_main, ev, data)
{
    PROCESS_BEGIN();
		initDoorRimeStack();
		setLock(LOCKED);

		//we start with garden lights off
		leds_off(LEDS_GREEN);
		leds_on(LEDS_RED);

		SENSORS_ACTIVATE(button_sensor);

		while(1)
		{
			PROCESS_WAIT_EVENT();

			if(ev == sensors_event && data == &button_sensor){
				//if the alarm is on, commands must be ignored
				if(!isAlarmOn)
				{
					//toggle alarm lights
					leds_toggle(LEDS_GREEN | LEDS_RED);
				}
			}
		}
    PROCESS_END();
}
