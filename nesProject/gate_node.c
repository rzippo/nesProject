#include "stdio.h"
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "net/rime/rime.h"

#include "commons/constants.h"
#include "commons/alarm_process.h"
#include "commons/lock.h"
#include "commons/light.h"
#include "gate/gateAutoOpeningProcess.h"
#include "gate/gateRimeStack.h"

//Used to trigger initialization code inside alarm process
#define GATE_NODE_INIT
AUTOSTART_PROCESSES(&alarm_process);

void processCUBroadcastCommand(unsigned char command)
{
	switch(command)
	{
		case ALARM_ON_COMMAND:
		{
			if(!isAlarmOn)
			{
				process_post_synch(&gateAutoOpeningProcess, alarm_on_event, NULL);
				process_post_synch(&alarm_process, alarm_on_event, NULL);
			}
			else
			{
				printf("Alarm is already ON: command refused\n");
			}
			break;
		}

		case ALARM_OFF_COMMAND:
		{
			if(isAlarmOn)
			{
				process_post_synch(&alarm_process, alarm_off_event, NULL);
				process_post_synch(&gateAutoOpeningProcess, alarm_off_event, NULL);
			}
			else
			{
				printf("Alarm is already OFF: command refused\n");
			}
			break;
		}

		case DOORS_AUTO_OPEN_COMMAND:
		{
			process_start(&gateAutoOpeningProcess, NULL);
			break;
		}

		default:
			printf("Unrecognized command in alarm broadcast channel\n");
	}
}

void processCUCommand(unsigned char command)
{
	if(isAlarmOn)
	{
		printf("Alarm is ON: command %c refused\n", command);
	}
	else
	{
		switch(command)
		{
			case GATELOCK_TOGGLE_COMMAND:
			{
				printf("Gate Lock Toggled\n");
				toogleLock();
				
				break;
			}
			
			case LIGHT_VALUE_COMMAND:
			{					
				double externalLight = getExternalLight();
				printf("External light value: %d\n", 
					(int) externalLight);

				//1 byte for cmd, 4 bytes for payload size, 
				//4 bytes for float
				unsigned char buff[9];

				//payload size is 5 bytes, cmd:1bytes; float:4bytes
				int* payloadSize = (int*)buff;
				*payloadSize = 5;
				*(buff+4) = LIGHT_VALUE_COMMAND;
				float* floatBuff = (float*)(buff+5);
				*floatBuff = (float) externalLight;
				
				sendFromGateToCentralUnit(buff,9);
				break;
			}
			
			default:
				printf("Command %d not recognized from this node\n", command);
				break;
		}
	}
}