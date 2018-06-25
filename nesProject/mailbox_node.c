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
	static char mboxLoad = 0;

	PROCESS_BEGIN();
	
	SENSORS_ACTIVATE(button_sensor);
	initMboxRimeStack();

	while(1)
	{
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
		mboxLoad = (mboxLoad + 1) % (MBOX_MAX_LOAD + 1);
		
		if(mboxLoad == 0)
			printf("Mailbox is empty.\n");
		else if(mboxLoad == MBOX_MAX_LOAD)
			printf("Mailbox is full!\n");
		else
			printf("Mailbox is at %d0%% load\n", mboxLoad);
		
		unsigned char command = mboxLoad;
		sendFromMboxToCentralUnit(&command, 1);
	}
	PROCESS_END();
}
