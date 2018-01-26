//
// Created by enric on 29/10/2017.
//
#include <stdio.h>
#include "addresses.h"

linkaddr_t centralNodeAddress;
linkaddr_t doorNodeAddress;
linkaddr_t gateNodeAddress;
linkaddr_t mboxNodeAddress;

void setNodesAddresses()
{
    centralNodeAddress.u8[0] = CENTRAL_UNIT_HIGH;
    centralNodeAddress.u8[1] = CENTRAL_UNIT_LOW;

    doorNodeAddress.u8[0] = DOOR_NODE_HIGH;
    doorNodeAddress.u8[1] = DOOR_NODE_LOW;

    gateNodeAddress.u8[0] = GATE_NODE_HIGH;
    gateNodeAddress.u8[1] = GATE_NODE_LOW;

    mboxNodeAddress.u8[0] = MBOX_NODE_HIGH;
    mboxNodeAddress.u8[1] = MBOX_NODE_LOW;
}
