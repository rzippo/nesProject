#include <net/linkaddr.h>
#include "stdio.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "constants.h"
#include "net/rime/rime.h"

#include "addresses.h"

static void recv_runicast(struct runicast_conn *c, const linkaddr_t *from, uint8_t seqno){}

static void sent_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions)
{
    printf("runicast message sent to %d.%d, retransmissions %d\n", to->u8[0], to->u8[1], retransmissions);
}

static void timedout_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions)
{
    printf("runicast message timed out when sending to %d.%d, retransmissions %d\n", to->u8[0], to->u8[1], retransmissions);
}

static const struct runicast_callbacks runicast_calls = {recv_runicast, sent_runicast, timedout_runicast};
static struct runicast_conn runicast;

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

                setNodesAddresses();

                printf("My address is %d.%d\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
                runicast_open(&runicast, 144, &runicast_calls);

                //int prova = 1;
                //packetbuf_copyfrom(&prova,4);
                //runicast_send(&runicast, &doorNodeAddress, MAX_RETRANSMISSIONS);

                //printf("Send to %d.%d\n", doorNodeAddress.u8[0], doorNodeAddress.u8[1]);

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
