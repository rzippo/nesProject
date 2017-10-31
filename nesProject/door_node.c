
#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "constants.h"
#include "net/rime/rime.h"
#include "addresses.h"
#include "dev/sht11/sht11-sensor.h"

#include "alarm_process.h"
#include "doorRimeStack.h"

static struct etimer temperatureTimer;

static float temperatures[MAX_TEMPERATURE_READINGS];
static int lastTemperatureIndex = 0;

PROCESS(door_node_main, "Door Node Main Process");

AUTOSTART_PROCESSES(&door_node_main, &alarm_process);

void processCUCommand(unsigned char command)
{
	if( command == ALARM_TOGGLE_COMMAND )
	{
		int postResult = process_post(&alarm_process, alarm_toggled_event, NULL);
		if( postResult == PROCESS_ERR_FULL)
			process_post_synch(&alarm_process, alarm_toggled_event, NULL);
	}
	else
	{
		if(isAlarmOn)
		{
			printf("Alarm is ON: command %c refused\n", command);
		}
		else
		{
			switch(command)
			{
				case DOORS_OPEN_COMMAND:
				{
					printf("Doors opened\n");
					break;
				}
				
				case AVERAGE_TEMPERATURE_COMMAND:
				{	//TODO:average temp
					printf("Average temp\n");
					break;
				}
				
				case LIGHT_VALUE_COMMAND:
				{
					printf("Light Value\n");
					break;
				}
				
				default:
					printf("Command %d not recognized from this node\n", command);
					break;
			}
		}
	}
}

PROCESS_THREAD(door_node_main, ev, data)
{
    PROCESS_BEGIN();
				initDoorRimeStack();
				
				SENSORS_ACTIVATE(sht11_sensor);
			
				etimer_set(&temperatureTimer, TEMPERATURE_MEASURE_PERIOD * CLOCK_SECOND);
				
				while(1)
				{
                    //printf("I wait for event\n");

                    PROCESS_WAIT_EVENT();

                    //printf("Event arrived\n");

                    if (ev == sensors_event && data == &button_sensor)
					{

                    }
					else if (ev == PROCESS_EVENT_TIMER)
					{
                        if (etimer_expired(&temperatureTimer))
                        {
                            // Sht11 header file tells us that this is the conversion formula
                            // Temperature in Celsius (t in 14 bits resolution at 3 Volts)
                            // T = -39.60 + 0.01*t
                            int measuredTemperature = sht11_sensor.value(SHT11_SENSOR_TEMP);
                            double convertedTemperature = -39.60 + 0.01*measuredTemperature;
                            //printf("Temp measured %d\n", (int)convertedTemperature);

                            temperatures[lastTemperatureIndex] = convertedTemperature;
                            lastTemperatureIndex = (lastTemperatureIndex + 1) % MAX_TEMPERATURE_READINGS;

                            etimer_reset(&temperatureTimer);
                    	}
                    }
                }

    PROCESS_END();
}


