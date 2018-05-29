#include <net/linkaddr.h>
#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"

#include "commons/constants.h"
#include "central_unit/cuRimeStack.h"


PROCESS(central_unit_main, "Central Unit Main Process");

AUTOSTART_PROCESSES(&central_unit_main);

void command_switch(unsigned char command)
{
    static int isAlarmOn = 0;

    if(command == ALARM_TOGGLE_COMMAND)
    {
        printf("Alarm Toggled\n");
        isAlarmOn = !isAlarmOn;
        char alarmCommand;
        if(isAlarmOn)
            alarmCommand = ALARM_ON_COMMAND;
        else
            alarmCommand = ALARM_OFF_COMMAND;

        broadcastAlarm(alarmCommand);
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

        case SHUT_OFF_LIGHTS_COMMAND:
        {
                printf("Broadcasting lights shut off\n");
                broadcastShutOff();
                break;
        }

        default:
                printf("There is no command with id %d\n", command);
                break;
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

void processMboxMessage(unsigned char* message, int payloadSize)
{
    unsigned char cmd = message[0];

    if(cmd == MBOX_EMPTY)
    {
        printf("Mailbox is empty\n");
        leds_off(LEDS_BLUE);
    }
    else if(cmd == MBOX_FULL)
    {
        printf("Mailbox is full\n");
        leds_on(LEDS_BLUE);
    }
}

PROCESS_THREAD(central_unit_main, ev, data)
{
	static struct etimer commandTimeout;
	static unsigned char buttonCount = 0;
	
	PROCESS_BEGIN();

	SENSORS_ACTIVATE(button_sensor);
    leds_off(LEDS_ALL);
    initCURimeStack();

	while(1)
	{
		PROCESS_WAIT_EVENT();

		if(ev == sensors_event && data == &button_sensor){
			++buttonCount;
			printf("Button pressed %d times\n", buttonCount);

			if(buttonCount == 1)//first press, set the timer
			{
				etimer_set( &commandTimeout, COMMAND_TIMEOUT * CLOCK_SECOND );
			}
			else
			{
				etimer_restart(&commandTimeout);
			}
		}
		else if(ev == PROCESS_EVENT_TIMER)
		{
			if(buttonCount != 0 && etimer_expired(&commandTimeout))
			{
				printf("Timer expired: count is %d\n", buttonCount);
				etimer_stop(&commandTimeout);
				command_switch(buttonCount);
				buttonCount = 0;
			}
		}
	}
    PROCESS_END();
}
