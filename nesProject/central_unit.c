#include "stdio.h"

#include "contiki.h"

#include "sys/etimer.h"

#include "dev/leds.h"

#include "dev/button-sensor.h"

#include "constants.h"

static struct etimer commandTimeout;
static unsigned char buttonCount = 0;

void command_switch(int command)
{
    switch(command)
    {
        case ALARM_TOGGLE_COMMAND: //Alarm toggle
            //TODO:alarm toggle
            printf("Alarm Toggled\n");
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

                SENSORS_ACTIVATE(button_sensor);

                while(1) {

                    PROCESS_WAIT_EVENT();

                    if(ev == sensors_event && data == &button_sensor){
                        ++buttonCount;
                        printf("Button pressed %d times\n", buttonCount);

                        if(buttonCount == 1)//first press, set the timer
                        {
                            etimer_set(&commandTimeout, COMMAND_TIMEOUT*CLOCK_SECOND);
                        }
                        else
                        {
                            etimer_restart(&commandTimeout);
                        }
                    }else if(ev == PROCESS_EVENT_TIMER)
                    {
                        if(buttonCount != 0 && etimer_expired(&commandTimeout)){
                            printf("Timer expired\n");
                            etimer_stop(&commandTimeout);
                            command_switch(buttonCount);
                            buttonCount = 0;
                        }
                    }
                }

    PROCESS_END();
}
