#include "alarm_process.h"

#include "stdio.h"
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"

#include "constants.h"

process_event_t alarm_toggled_event;
unsigned char isAlarmOn = 0;

PROCESS(alarm_process, "Alarm blinking process");

PROCESS_THREAD(alarm_process, ev, data)
{
	static struct etimer alarmBlinkingTimer;
	static unsigned char ledStatusBeforeAlarm = 0;
	
	PROCESS_BEGIN();
	
		while(1)
		{
			PROCESS_WAIT_EVENT();
			
			if(ev == alarm_toggled_event)
			{
				if(isAlarmOn == 0)
				{
					printf("Alarm Toggled: ON\n");
					
					isAlarmOn = 1;
					ledStatusBeforeAlarm = leds_get();
					leds_on(LEDS_ALL);
					
					//divide by 2 because 1sec on + 1sec off = 2secs period 
					etimer_set( &alarmBlinkingTimer, ALARM_LED_PERIOD * CLOCK_SECOND / 2 );
				}
				else
				{
					printf("Alarm Toggled: OFF\n");
					
					isAlarmOn = 0;
					leds_set(ledStatusBeforeAlarm);
					etimer_stop(&alarmBlinkingTimer);
				}
			}
			else if ( 	isAlarmOn &&
						ev == PROCESS_EVENT_TIMER &&
						etimer_expired(&alarmBlinkingTimer) )
			{
				#if DEBUG
				printf("Alarm blinking\n");
				#endif
				leds_toggle(LEDS_ALL);
				etimer_reset(&alarmBlinkingTimer);
			}
		}
	
	PROCESS_END();
}
