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

PROCESS(gate_node_init, "Gate Node init Process");
AUTOSTART_PROCESSES(&gate_node_init, &alarm_process);

void processCUCommand(unsigned char command)
{
	if( command == ALARM_TOGGLE_COMMAND )
	{
		if(!isAlarmOn)
		{
			process_post_synch(&gateAutoOpeningProcess, alarm_toggled_event, NULL);
			process_post_synch(&alarm_process, alarm_toggled_event, NULL);
		}
		else
		{
			process_post_synch(&alarm_process, alarm_toggled_event, NULL);
			process_post_synch(&gateAutoOpeningProcess, alarm_toggled_event, NULL);
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
				case GATELOCK_TOGGLE_COMMAND:
				{
					printf("Gate Lock Toggled\n");
					toogleLock();
					
					break;
				}
				
				case DOORS_OPEN_COMMAND:
				{
					process_start(&gateAutoOpeningProcess, NULL);
					break;
				}
				
				case LIGHT_VALUE_COMMAND:
				{
					//TODO: send result back to CU
					
					double externalLight = getExternalLight();
					printf("External light value: %d\n", (int) externalLight);
					
					break;
				}
				
				default:
					printf("Command %d not recognized from this node\n", command);
					break;
			}
		}
	}
}

PROCESS_THREAD(gate_node_init, ev, data)
{
	PROCESS_BEGIN();
		initGateRimeStack();
				setLock(LOCKED);
	PROCESS_END();
}
