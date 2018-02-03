#include <net/linkaddr.h>
#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"

#include "commons/constants.h"
#include "commons/command_process.h"
#include "central_unit/cuRimeStack.h"


PROCESS(central_unit_main, "Central Unit Main Process");

AUTOSTART_PROCESSES(&central_unit_main, &command_process);


void command_switch(unsigned char command)
{
    static int isAlarmOn = 0;

    if(command == ALARM_TOGGLE_COMMAND)
    {
        printf("Alarm Toggled\n");
        isAlarmOn = !isAlarmOn;
        
        sendDoorNode(&command, 1);
        sendGateNode(&command, 1);

        return;
    }

    //if alarm is on, ignore commands
    if(isAlarmOn)
    {
       printf("Alarm is on, command ignored!\n");
       return; 
    }

    switch(command)
    {
        case GATELOCK_TOGGLE_COMMAND:
        {
            printf("Gate Lock Toggled\n");

            sendGateNode(&command, 1);
            
            break;
        }
        
        case DOORS_OPEN_COMMAND:
        {
            printf("Doors opened\n");
            
            sendDoorNode(&command, 1);
            sendGateNode(&command, 1);
            
            break;
        }
        
        case AVERAGE_TEMPERATURE_COMMAND:
        {
            printf("Average temp\n");
    
            sendDoorNode(&command, 1);
            
            break;
        }
        
        case LIGHT_VALUE_COMMAND:
        {
            printf("Light Value\n");

            sendGateNode(&command, 1);

            break;
        }
    }
}

void processDoorMessage(unsigned char* message, int payloadSize)
{
    unsigned char cmd = message[0];

    if(cmd == AVERAGE_TEMPERATURE_COMMAND)
    {
        float averageTemperature = *( (float*)(message+1));
        printf("Average received temp is: %d\n",
               (int) averageTemperature);
    }

}

void processGateMessage(unsigned char* message, int payloadSize)
{
    unsigned char cmd = message[0];

    if(cmd == LIGHT_VALUE_COMMAND)
    {
        float lightValue = *( (float*)(message+1));
        printf("Light value is: %d\n",
               (int) lightValue);
    }
}

PROCESS_THREAD(central_unit_main, ev, data)
{
    PROCESS_BEGIN();
	initCURimeStack();
    leds_off(LEDS_ALL);
    PROCESS_END();
}
