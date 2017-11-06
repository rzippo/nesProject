#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "constants.h"
#include "net/rime/rime.h"
#include "addresses.h"
#include "dev/sht11/sht11-sensor.h"

#include "alarm_process.h"
#include "gateRimeStack.h"

static char isGateLocked = 1;

PROCESS(gate_init, "Gate Node init Process");

AUTOSTART_PROCESSES(&gate_init, &alarm_process);

void updateGateLockLEDs()
{
	if(isGateLocked)
	{
		leds_off(LEDS_GREEN);
		leds_on(LEDS_RED);
	}
	else
	{
		leds_on(LEDS_GREEN);
		leds_off(LEDS_RED);
	}
}

void processCUCommand(unsigned char command)
{
	if( command == ALARM_TOGGLE_COMMAND )
	{
		int postResult = process_post(&alarm_process, alarm_toggled_event, NULL);
		if( postResult == PROCESS_ERR_FULL)
			process_post_synch(&alarm_process, alarm_toggled_event, NULL);
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
					
					isGateLocked = !isGateLocked;
					updateGateLockLEDs();
					
					break;
				}
				
				case DOORS_OPEN_COMMAND:
				{
					printf("Doors opened\n");
					break;
				}
				
				case LIGHT_VALUE_COMMAND:
				{
					//TODO: implement measurement and send result back to CU
					printf("Light Value\n");
					break;
				}
				
				default:
					printf("Command %d not recognized from this node\n", command);
					break;
			}
		}
	}
}

PROCESS_THREAD(gate_init, ev, data)
{
	PROCESS_BEGIN();
		initGateRimeStack();
		updateGateLockLEDs();
	PROCESS_END();
}
