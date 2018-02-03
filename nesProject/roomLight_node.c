#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"

#include "commons/constants.h"
#include "commons/light.h"
#include "roomLight/roomLightRimeStack.h"

PROCESS(light_node_init, "Light node init process");
AUTOSTART_PROCESSES(&light_node_init);

unsigned int lightIntensity = LIGHT_DEFAULT_INTENSITY;
int lightOn = 0;

//by default, if the lights is switched on when no message 
//has been received, the light is at max intensity
int state = RL_TURN_ON;

void setLeds(unsigned char command)
{
    if(command == RL_DIM)
    {
        lightIntensity = LIGHT_DEFAULT_INTENSITY/10;
        leds_off(LEDS_ALL);
        leds_on(LEDS_BLUE);
        printf("Light dimmed to 10%%: %d\n", lightIntensity);
    }

    if(command == RL_BRIGHTEN)
    {
        lightIntensity = LIGHT_DEFAULT_INTENSITY*3/10;
        leds_off(LEDS_ALL);
        leds_on(LEDS_GREEN);
        printf("Light brightened to 30%%: %d\n", lightIntensity);
    }

    if(command == RL_TURN_ON)
    {
        lightIntensity = LIGHT_DEFAULT_INTENSITY;
        leds_off(LEDS_ALL);
        leds_on(LEDS_RED);
        printf("Light set at maximum intensity: %d\n", lightIntensity);
    }

    if (command == RL_OFF)
    {
        leds_off(LEDS_ALL);
        printf("Light turned off\n");
    }
}

void processHTCommand(unsigned char command)
{
    if(command == RL_DIM || command == RL_BRIGHTEN || command == RL_TURN_ON)
        state = command;
    else
    {
        printf("Command not recognized, ignored\n");
        return;
    }

    if(lightOn)
    {
        setLeds(command);
    }
}

PROCESS_THREAD(light_node_init, ev, data)
{
    PROCESS_BEGIN();
        initLightRimeStack();

        SENSORS_ACTIVATE(button_sensor);
        while(1)
        {
            PROCESS_WAIT_EVENT();
            if(ev == sensors_event && data == &button_sensor)
            {
                lightOn = !lightOn;
                if(lightOn)
                {
                    setLeds(state);
                }
                else
                {
                    setLeds(RL_OFF);
                }
                printf("Button pressed, next state: %s\n", lightOn == 0 ? "OFF" : "ON");
            }
        }

    PROCESS_END();
}