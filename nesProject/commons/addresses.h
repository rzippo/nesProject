#ifndef NESPROJECT_ADDRESSES_H

#include "net/linkaddr.h"

#define NESPROJECT_ADDRESSES_H

#define CENTRAL_UNIT_HIGH 3
#define CENTRAL_UNIT_LOW 0

#define DOOR_NODE_HIGH 1
#define DOOR_NODE_LOW 0

#define GATE_NODE_HIGH 2
#define GATE_NODE_LOW 0

#define HT_NODE_HIGH 4
#define HT_NODE_LOW 0

#define RL_NODE_HIGH 5
#define RL_NODE_LOW 0

#define CU_DOOR_CHANNEL 144
#define CU_GATE_CHANNEL 154
#define HT_RL_CHANNEL 164

extern linkaddr_t centralNodeAddress;
extern linkaddr_t doorNodeAddress;
extern linkaddr_t gateNodeAddress;
extern linkaddr_t htNodeAddress;
extern linkaddr_t rlNodeAddress;

void setNodesAddresses();

#endif //NESPROJECT_ADDRESSES_H
