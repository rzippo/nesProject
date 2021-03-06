#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"

#include "commons/constants.h"
#include "commons/light.h"
#include "roomLight/roomLightRimeStack.h"

PROCESS(light_node_main, "Light node init process");
PROCESS(light_adjuster, "Light adjuster process");
AUTOSTART_PROCESSES(&light_node_main);

process_event_t light_on_event;
process_event_t light_off_event;

int lightOn = 0;
double lightExponentialAverage = 0.0;
double alpha = ((double)LIGHT_ADJUSTMENT_PERIOD_SECONDS)/LIGHT_ADJUSTMENT_RESPONSE_TIME_SECONDS;

void processCUCommand(unsigned char command)
{
    if(command == SHUT_OFF_LIGHTS_COMMAND)
    {
        lightOn = 0;
        process_post_synch(&light_adjuster, light_off_event, NULL);
    }
    else 
    {
        printf("Command not recognized, ignored.\n");
    }
}

PROCESS_THREAD(light_node_main, ev, data)
{
    PROCESS_BEGIN();
        //Allocate events
        light_on_event = process_alloc_event();
        light_off_event = process_alloc_event();
        
        //Initialize exponential mean
        lightExponentialAverage = getExternalLight();
        process_start(&light_adjuster, NULL);
        SENSORS_ACTIVATE(button_sensor);

        initLightRimeStack();
       
        while(1)
        {
            PROCESS_WAIT_EVENT();
            if(ev == sensors_event && data == &button_sensor)
            {
                lightOn = !lightOn;
                printf("Button pressed, next state: %d\n", lightOn);
                switch(lightOn)
                {
                    case 0:
                        process_post_synch(&light_adjuster, light_off_event, NULL);
                        break;

                    case 1:
                        process_post_synch(&light_adjuster, light_on_event, NULL);
                        break;
                }
            }
        }

    PROCESS_END();
}

void adjustLight()
{
    double ligthMeasurement = getExternalLight();
    lightExponentialAverage = ligthMeasurement * alpha + lightExponentialAverage * (1 - alpha);
    printf("Detected light average %d\n", (int) lightExponentialAverage);    
    printf("Adjusting light by %d\n", (int) (LIGHT_TARGET_INTENSITY - lightExponentialAverage));
}


PROCESS_THREAD(light_adjuster, ev, data)
{
    static struct etimer lightAdjustmentTimer;

    PROCESS_BEGIN();
        while(1)
        {
            PROCESS_WAIT_EVENT();
            if(ev == light_on_event)
            {
                printf("Light is now on\n");
                leds_on(LEDS_BLUE); 
                adjustLight();
                etimer_set(&lightAdjustmentTimer, LIGHT_ADJUSTMENT_PERIOD_SECONDS * CLOCK_SECOND);
            }
            else if(ev == light_off_event)
            {
                printf("Light is now off\n");
                leds_off(LEDS_BLUE);
                etimer_stop(&lightAdjustmentTimer);
            }
            else if(ev == PROCESS_EVENT_TIMER && etimer_expired(&lightAdjustmentTimer))
            {
                adjustLight();
                etimer_reset(&lightAdjustmentTimer);
            }
        }
    PROCESS_END();
}