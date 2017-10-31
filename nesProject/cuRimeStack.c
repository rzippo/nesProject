//
// Created by Raff on 31/10/2017.
//
#include "cuRimeStack.h"
#include "addresses.h"
#include "constants.h"

#include "stdio.h"
#include "net/rime/rime.h"


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
static struct runicast_conn doorRunicastConnection;
static struct runicast_conn gateRunicastConnection;

void sendDoorNode(unsigned char c)
{
	packetbuf_copyfrom(&c,1);
	runicast_send(&doorRunicastConnection, &doorNodeAddress, MAX_RETRANSMISSIONS);
}

void sendGateNode(unsigned char c)
{
	packetbuf_copyfrom(&c,1);
	runicast_send(&gateRunicastConnection, &gateNodeAddress, MAX_RETRANSMISSIONS);
}

void initCURimeStack()
{
	setNodesAddresses();
	
	printf("My address is %d.%d\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
	
	runicast_open(&doorRunicastConnection, CU_DOOR_CHANNEL, &runicast_calls);
	runicast_open(&gateRunicastConnection, CU_GATE_CHANNEL, &runicast_calls);
}
