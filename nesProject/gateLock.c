//
// Created by Raff on 10/11/2017.
//

#include "gateLock.h"

#include "contiki.h"
#include "dev/leds.h"

static char isGateLocked = 1;

void updateGateLockLEDs()
{
	if(isGateLocked)
	{
		leds_off(LEDS_GREEN);
		leds_on(LEDS_RED);
	}
	else
	{
		leds_on(LEDS_GREEN);
		leds_off(LEDS_RED);
	}
}

void setGateLock(char gateLock)
{
	isGateLocked = ( gateLock == GATE_UNLOCKED ) ? GATE_UNLOCKED : GATE_LOCKED;
	updateGateLockLEDs();
}

void toogleGateLock()
{
	isGateLocked = !isGateLocked;
	updateGateLockLEDs();
}
