//
// Created by Raff on 31/10/2017.
//

#include "door/doorRimeStack.h"
#include "commons/addresses.h"
#include "commons/constants.h"

#include "stdio.h"
#include "net/rime/rime.h"

extern void processCUCommand(unsigned char command);
extern void setNodesAddresses();

static void recv_runicast(struct runicast_conn *c, const linkaddr_t *from, uint8_t seqno)
{
	unsigned char receivedCommand = *( (unsigned char*)packetbuf_dataptr() );
	
	printf("runicast message received from %d.%d, seqno %d, message: %c\n",
		   from->u8[0],
		   from->u8[1],
		   seqno,
		   receivedCommand);
	
	if( linkaddr_cmp(from, &centralNodeAddress))
	{
		processCUCommand(receivedCommand);
	}
	else
	{
		printf("Message from unexpected node: refused\n");
	}
}

static void sent_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions){}

static void timedout_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions){}

static const struct runicast_callbacks runicast_calls = {recv_runicast, sent_runicast, timedout_runicast};
static struct runicast_conn cuRunicastConnection;

void initDoorRimeStack()
{
	setNodesAddresses();
	
	printf("My address is %d.%d\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
	
	runicast_open(&cuRunicastConnection, CU_DOOR_CHANNEL, &runicast_calls);
}

void sendFromDoorToCentralUnit(unsigned char *cmd, int bytes)
{
	packetbuf_copyfrom(cmd, bytes);
	runicast_send(&cuRunicastConnection, &centralNodeAddress, MAX_RETRANSMISSIONS);
}