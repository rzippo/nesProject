#include "stdio.h"
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "net/rime/rime.h"

#include "commons/constants.h"
#include "commons/command_process.h"
#include "commons/lock.h"
#include "commons/light.h"
#include "mailbox/mboxRimeStack.h"

PROCESS(mbox_node_init, "Mbox Node init Process");
AUTOSTART_PROCESSES(&mbox_node_init, &command_process);

void command_switch(unsigned char command)
{
	if(command == MBOX_EMPTY)
	{
		printf("Mailbox is empty\n");
		sendFromMboxToCentralUnit(&command, 1);
	}
	else if(command == MBOX_FULL)
	{
		printf("Mailbox is full\n");
		sendFromMboxToCentralUnit(&command, 1);
	}
}

PROCESS_THREAD(mbox_node_init, ev, data)
{
	PROCESS_BEGIN();
		initMboxRimeStack();
	PROCESS_END();
}
