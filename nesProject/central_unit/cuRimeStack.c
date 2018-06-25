#include "cuRimeStack.h"
#include "commons/addresses.h"
#include "commons/constants.h"

#include "stdio.h"
#include "net/rime/rime.h"

extern void processDoorMessage(unsigned char* message, int payloadSize);
extern void processGateMessage(unsigned char* message, int payloadSize);
extern void processMboxMessage(unsigned char* message, int payloadSize);

extern void setNodesAddresses();

static void recv_runicast(struct runicast_conn *c, const linkaddr_t *from, uint8_t seqno)
{
    unsigned char* buffer = packetbuf_dataptr();

	int payloadSize = *( (int*)buffer);

    printf("runicast message received from %d.%d, seqno %d, message: %c\n",
           from->u8[0],
           from->u8[1],
           seqno,
           *(buffer+4));

	if(linkaddr_cmp(from, &doorNodeAddress))
	{
		processDoorMessage(buffer+4, payloadSize);
	}
	else if(linkaddr_cmp(from, &gateNodeAddress))
	{
		processGateMessage(buffer+4, payloadSize);
	}
	else if(linkaddr_cmp(from, &mboxNodeAddress))
	{
		processMboxMessage(buffer, payloadSize);
	}
	else
	{
		printf("Message from unexpected node: refused\n");
	}
}

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
static struct runicast_conn mboxRunicastConnection;

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from){}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};

static struct broadcast_conn alarmBroadcastConnection;
static struct broadcast_conn roomLightsBroadcastConnection;

void sendDoorNode(unsigned char* c, int bytes)
{
	packetbuf_copyfrom(c, bytes);
	runicast_send(&doorRunicastConnection, &doorNodeAddress, MAX_RETRANSMISSIONS);
}

void sendGateNode(unsigned char* c, int bytes)
{
	packetbuf_copyfrom(c, bytes);
	runicast_send(&gateRunicastConnection, &gateNodeAddress, MAX_RETRANSMISSIONS);
}

//todo: unreliable send to MBOX?

void broadcastDoorGate(char command)
{
	packetbuf_copyfrom(&command, sizeof(char));
	broadcast_send(&alarmBroadcastConnection);
	printf("Sent broadcast message to DOOR_GATE channel, message: %d\n", command);
};

void broadcastShutOff()
{
	char command = SHUT_OFF_LIGHTS_COMMAND;
	packetbuf_copyfrom(&command, sizeof(char));
	broadcast_send(&roomLightsBroadcastConnection);
	printf("Sent broadcast message to ROOMLIGHTS channel, message: %d\n", command);
};

void initCURimeStack()
{
	setNodesAddresses();
	
	printf("My address is %d.%d\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
	
	runicast_open(&doorRunicastConnection, CU_DOOR_CHANNEL, &runicast_calls);
	runicast_open(&gateRunicastConnection, CU_GATE_CHANNEL, &runicast_calls);
	
	//todo: should be unreliable?
	runicast_open(&mboxRunicastConnection, CU_MBOX_CHANNEL, &runicast_calls);

	broadcast_open(&alarmBroadcastConnection, GATE_DOOR_BROADCAST_CHANNEL, &broadcast_call);
	broadcast_open(&roomLightsBroadcastConnection, ROOMLIGHTS_BROADCAST_CHANNEL, &broadcast_call);
}
