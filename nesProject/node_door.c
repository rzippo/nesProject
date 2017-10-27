//
// Created by enric on 27/10/2017.
//
#include "stdio.h"

#include "contiki.h"

#include "sys/etimer.h"

#include "dev/leds.h"

#include "dev/button-sensor.h"

#include "constants.h"


static struct etimer alarmTimer;
static unsigned char alarm = 0;
static unsigned char ledStatus = 0;

void command_switch(int command)
{
    switch(command)
    {
        case ALARM_TOGGLE_COMMAND: //Alarm toggle
            //TODO:alarm toggle
            printf("Alarm Toggled\n");
            if(alarm == 0)
            {
                alarm = 1;
                ledStatus = leds_get();
                leds_on(LEDS_ALL);
                etimer_set(&alarmTimer, ALARM_LED_PERIOD*CLOCK_SECOND);
            }
            else
            {
                alarm = 0;
                leds_set(ledStatus);
                etimer_stop(&alarmTimer);
            }
            break;
        case GATELOCK_TOGGLE_COMMAND: //Gate lock toggle
            //TODO:gate lock toggle
            printf("Gate Lock Toggled\n");
            break;
        case DOORS_OPEN_COMMAND: //Open doors
            //TODO:open doors
            printf("Doors opened\n");
            break;
        case AVERAGE_TEMPERATURE_COMMAND: //Average temp
            //TODO:average temp
            printf("Average temp\n");
            break;
        case LIGHT_VALUE_COMMAND: //Light value
            //TODO: light value
            printf("Light Value\n");
            break;
        default: //error, no command
            printf("There is no command with id %d\n", command);
            break;
    }
}


PROCESS(central_unit_main, "Central Unit Main Process");

AUTOSTART_PROCESSES(&central_unit_main);

PROCESS_THREAD(central_unit_main, ev, data)
{
    PROCESS_BEGIN();

                while(1) {

                    PROCESS_WAIT_EVENT();

                    if(ev == sensors_event && data == &button_sensor){

                    }else if(ev == PROCESS_EVENT_TIMER)
                    {
                        if(alarm && etimer_expired(&alarmTimer))
                        {
                            leds_toggle(LEDS_ALL);
                            etimer_reset(&alarmTimer);
                        }
                    }

    PROCESS_END();
}