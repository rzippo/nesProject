#include "stdio.h"
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "net/rime/rime.h"

#include "commons/constants.h"
#include "mailbox/mboxRimeStack.h"

PROCESS(mbox_node_main, "Mbox Node init Process");
AUTOSTART_PROCESSES(&mbox_node_main);

PROCESS_THREAD(mbox_node_main, ev, data)
{
	static int isFull = 0;

	PROCESS_BEGIN();
	
	SENSORS_ACTIVATE(button_sensor);
	initMboxRimeStack();

	while(1)
	{
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
		if(isFull)
		{
			isFull = 0;
			printf("Mailbox is empty\n");
			unsigned char command = MBOX_EMPTY;
			sendFromMboxToCentralUnit(&command, 1);
		}
		else
		{
			isFull = 1;
			printf("Mailbox is full\n");
			unsigned char command = MBOX_FULL;
			sendFromMboxToCentralUnit(&command, 1);
		}	
	}
	PROCESS_END();
}
