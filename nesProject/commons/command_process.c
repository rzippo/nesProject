#include "stdio.h"
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"

#include "constants.h"
#include <net/linkaddr.h>
#include "dev/button-sensor.h"
#include "alarm_process.h"
#include "command_process.h"

extern void command_switch(unsigned char command);

PROCESS(command_process, "Command picker process");

PROCESS_THREAD(command_process, ev, data)
{
	static struct etimer commandTimeout;
	static unsigned char buttonCount = 0;
	
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(button_sensor);
	
	while(1)
	{
		PROCESS_WAIT_EVENT();

		if(ev == sensors_event && data == &button_sensor){
			++buttonCount;
			printf("Button pressed %d times\n", buttonCount);

			if(buttonCount == 1)//first press, set the timer
			{
				etimer_set( &commandTimeout, COMMAND_TIMEOUT * CLOCK_SECOND );
			}
			else
			{
				etimer_restart(&commandTimeout);
			}
		}
		else if(ev == PROCESS_EVENT_TIMER)
		{
			if(buttonCount != 0 && etimer_expired(&commandTimeout))
			{
				printf("Timer expired: count is %d\n", buttonCount);
				etimer_stop(&commandTimeout);
				command_switch(buttonCount);
				buttonCount = 0;
			}
		}
	}
    PROCESS_END();
}
