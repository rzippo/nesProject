//
// Created by enric on 27/10/2017.
//
#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "constants.h"
#include "net/rime/rime.h"
#include "addresses.h"
#include "dev/sht11/sht11-sensor.h"


static struct etimer alarmBlinkingTimer;
static struct etimer temperatureTimer;

static unsigned char isAlarmOn = 0;
static unsigned char alarmPreviousLEDStatus = 0;

static float temperatures[MAX_TEMPERATURE_READINGS];
static int lastTemperatureIndex = 0;

static process_event_t alarm_toggled_event;

void command_switch(int);

PROCESS(door_node_main, "Door Node Main Process");

PROCESS(alarm_blinking_process, "Alarm blinking process");

AUTOSTART_PROCESSES(&door_node_main, &alarm_blinking_process);


static void recv_runicast(struct runicast_conn *c, const linkaddr_t *from, uint8_t seqno)
{
    int receivedCommand = *((int*)packetbuf_dataptr());
    printf("runicast message received from %d.%d, seqno %d, message: %d\n", from->u8[0], from->u8[1], seqno, receivedCommand);

    command_switch(receivedCommand);
}

static void sent_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions){}


static void timedout_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions){}

static const struct runicast_callbacks runicast_calls = {recv_runicast, sent_runicast, timedout_runicast};
static struct runicast_conn runicast;

void command_switch(int command)
{
    switch(command)
    {
        case ALARM_TOGGLE_COMMAND: //Alarm toggle
            //TODO:alarm toggle
            process_post(&door_node_main, alarm_toggled_event, NULL);
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

void toggleAlarm()
{
    if(isAlarmOn == 0)
    {
		printf("Alarm Toggled: ON\n");
		
        isAlarmOn = 1;
        alarmPreviousLEDStatus = leds_get();
        leds_on(LEDS_ALL);
        
		etimer_set( &alarmBlinkingTimer, ALARM_LED_PERIOD * CLOCK_SECOND );
    }
    else
    {
		printf("Alarm Toggled: OFF\n");
		
        isAlarmOn = 0;
        leds_set(alarmPreviousLEDStatus);
        etimer_stop(&alarmBlinkingTimer);
    }
}


PROCESS_THREAD(door_node_main, ev, data)
{
    PROCESS_BEGIN();

                setNodesAddresses();

                SENSORS_ACTIVATE(sht11_sensor);

                etimer_set(&temperatureTimer, TEMPERATURE_MEASURE_PERIOD*CLOCK_SECOND);

                printf("My address is %d.%d\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);

                runicast_open(&runicast, 144, &runicast_calls);

                while(1) {

                    //printf("I wait for event\n");

                    PROCESS_WAIT_EVENT();

                    //printf("Event arrived\n");

                    if (ev == sensors_event && data == &button_sensor) {

                    } else if (ev == PROCESS_EVENT_TIMER) {
                        if (isAlarmOn && etimer_expired(&alarmBlinkingTimer)) {
                            leds_toggle(LEDS_ALL);
                            etimer_reset(&alarmBlinkingTimer);
                        }
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
                    } else if (ev == alarm_toggled_event)
                    {
                        toggleAlarm();
                    }
                }

    PROCESS_END();
}

PROCESS_THREAD(alarm_blinking_process, ev, data)
{
	PROCESS_BEGIN();
	
		while(1)
		{
			PROCESS_WAIT_EVENT_UNTIL(
					isAlarmOn &&
					ev == PROCESS_EVENT_TIMER &&
					etimer_expired(&alarmBlinkingTimer) );
			
			leds_toggle(LEDS_ALL);
			etimer_reset(&alarmBlinkingTimer);
		}
		
	PROCESS_END();
}
