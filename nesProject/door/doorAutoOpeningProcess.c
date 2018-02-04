#include "doorAutoOpeningProcess.h"

#include "contiki.h"
#include "sys/clock.h"
#include "stdio.h"
#include "dev/leds.h"

#include "commons/constants.h"
#include "commons/lock.h"
#include "commons/alarm_process.h"

PROCESS(doorAutoOpeningProcess, "Door auto opening process");

PROCESS_THREAD(doorAutoOpeningProcess, ev, data)
{
	static struct etimer initialDelay;
	static struct etimer blinkingTimer;
	static int blinkings;
	static clock_time_t remainingDelay;
	
	PROCESS_BEGIN();
		
		printf("Door auto opening: started\n");

		remainingDelay = DOOR_AUTO_OPENING_DELAY * CLOCK_SECOND;

		while(1)
		{
			printf("Door auto opening: waiting initial delay\n");
			etimer_set(&initialDelay, remainingDelay);
			PROCESS_WAIT_EVENT();
			if(ev == PROCESS_EVENT_TIMER && etimer_expired(&initialDelay))
				break;
			else if( ev == alarm_toggled_event)
			{
				printf("Door auto opening: delay interrupted by alarm\n");
				remainingDelay = etimer_expiration_time(&initialDelay) - clock_time();
				etimer_stop(&initialDelay);
				PROCESS_WAIT_EVENT_UNTIL(ev == alarm_toggled_event);
				printf("Door auto opening: alarm stopped, resuming delay\n");
			}
		}
		
		printf("Door auto opening: door opened\n");
		setLock(UNLOCKED);
		
		printf("Door auto opening: blinking started\n");
		
		blinkings = 0;
		
		leds_on(LEDS_BLUE);
		
		while(blinkings < AUTO_OPENING_BLINKINGS - 1)
		{
			etimer_set(&blinkingTimer, (AUTO_OPENING_LED_PERIOD / 2) * CLOCK_SECOND);
			PROCESS_WAIT_EVENT();
			if(ev == PROCESS_EVENT_TIMER && etimer_expired(&blinkingTimer))
			{
				printf("Door auto opening: blinking\n");
				leds_toggle(LEDS_BLUE);
				etimer_reset(&blinkingTimer);
				blinkings++;
			}
			else if(ev == alarm_toggled_event)
			{
				printf("Door auto opening: blinking interrupted by alarm\n");
				etimer_stop(&blinkingTimer);
				PROCESS_WAIT_EVENT_UNTIL(ev == alarm_toggled_event);
				printf("Door auto opening: alarm stopped, resuming blinking\n");
			}
		}
		
		printf("Door auto opening: blinking stopped\n");
		printf("Gate auto opening: door locked\n");
		setLock(LOCKED);
	PROCESS_END();
}
