//
// Created by Raff on 10/11/2017.
//

#include "gateAutoOpeningProcess.h"

#include "contiki.h"
#include "stdio.h"
#include "dev/leds.h"

#include "commons/constants.h"
#include "commons/lock.h"
#include "commons/alarm_process.h"

PROCESS(gateAutoOpeningProcess, "Gate auto opening process");

PROCESS_THREAD(gateAutoOpeningProcess, ev, data)
{
	static struct etimer blinkingTimer;
	static int blinkings;
	
	PROCESS_BEGIN();
		printf("Gate auto opening: gate opened\n");
				setLock(UNLOCKED);
		
		printf("Gate auto opening: blinking started\n");
		
		blinkings = 0;
		
		leds_on(LEDS_BLUE);
		
		while(blinkings < AUTO_OPENING_BLINKINGS - 1)
		{
			etimer_set(&blinkingTimer, (AUTO_OPENING_LED_PERIOD / 2) * CLOCK_SECOND);
			PROCESS_WAIT_EVENT();
			if(ev == PROCESS_EVENT_TIMER && etimer_expired(&blinkingTimer))
			{
				printf("Gate auto opening: blinking\n");
				leds_toggle(LEDS_BLUE);
				etimer_reset(&blinkingTimer);
				blinkings++;
			}
			else if(ev == alarm_toggled_event)
			{
				printf("Gate auto opening: blinking interrupted by alarm\n");
				etimer_stop(&blinkingTimer);
				PROCESS_WAIT_EVENT_UNTIL(ev == alarm_toggled_event);
				printf("Gate auto opening: alarm stopped, resuming blinking\n");
			}
		}
		
		printf("Gate auto opening: blinking stopped\n");
		printf("Gate auto opening: gate locked\n");
				setLock(LOCKED);
	PROCESS_END();
}
