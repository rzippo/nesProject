//
// Created by enric on 29/10/2017.
//

#ifndef NESPROJECT_ADDRESSES_H

#include "net/linkaddr.h"

#define NESPROJECT_ADDRESSES_H

#define CENTRAL_UNIT_HIGH 3
#define CENTRAL_UNIT_LOW 0

#define DOOR_NODE_HIGH 1
#define DOOR_NODE_LOW 0

#define GATE_NODE_HIGH 2
#define GATE_NODe_LOW 0


linkaddr_t centralNodeAddress;
linkaddr_t doorNodeAddress;
linkaddr_t gateNodeAddress;

void setNodesAddresses();

#endif //NESPROJECT_ADDRESSES_H
