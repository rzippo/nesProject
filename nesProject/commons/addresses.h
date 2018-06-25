#ifndef NESPROJECT_ADDRESSES_H

#include "net/linkaddr.h"

#define NESPROJECT_ADDRESSES_H

#define CENTRAL_UNIT_HIGH 3
#define CENTRAL_UNIT_LOW 0

#define DOOR_NODE_HIGH 1
#define DOOR_NODE_LOW 0

#define GATE_NODE_HIGH 2
#define GATE_NODE_LOW 0

#define MBOX_NODE_HIGH 4
#define MBOX_NODE_LOW 0

//Unicast channels
#define CU_DOOR_CHANNEL 144
#define CU_GATE_CHANNEL 154
#define CU_MBOX_CHANNEL 164

//Broadcast channels
#define GATE_DOOR_BROADCAST_CHANNEL 174
#define ROOMLIGHTS_BROADCAST_CHANNEL 184

extern linkaddr_t centralNodeAddress;
extern linkaddr_t doorNodeAddress;
extern linkaddr_t gateNodeAddress;
extern linkaddr_t mboxNodeAddress;

void setNodesAddresses();

#endif //NESPROJECT_ADDRESSES_H
