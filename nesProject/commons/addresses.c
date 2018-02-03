//
// Created by enric on 29/10/2017.
//
#include <stdio.h>
#include "addresses.h"

linkaddr_t centralNodeAddress;
linkaddr_t doorNodeAddress;
linkaddr_t gateNodeAddress;
linkaddr_t htNodeAddress;
linkaddr_t rlNodeAddress;


void setNodesAddresses()
{
    centralNodeAddress.u8[0] = CENTRAL_UNIT_HIGH;
    centralNodeAddress.u8[1] = CENTRAL_UNIT_LOW;

    doorNodeAddress.u8[0] = DOOR_NODE_HIGH;
    doorNodeAddress.u8[1] = DOOR_NODE_LOW;

    gateNodeAddress.u8[0] = GATE_NODE_HIGH;
    gateNodeAddress.u8[1] = GATE_NODE_LOW;

    htNodeAddress.u8[0] = HT_NODE_HIGH;
    htNodeAddress.u8[1] = HT_NODE_LOW;

    rlNodeAddress.u8[0] = RL_NODE_HIGH;
    rlNodeAddress.u8[1] = RL_NODE_LOW;
}
