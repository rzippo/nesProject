#include "lock.h"

#include "contiki.h"
#include "dev/leds.h"

static char isLocked = 1;

void updateLockLEDs()
{
	if(isLocked)
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

void setLock(char lockedState)
{
	isLocked = ( lockedState == UNLOCKED ) ? UNLOCKED : LOCKED;
	updateLockLEDs();
}

void toogleLock()
{
	isLocked = !isLocked;
	updateLockLEDs();
}
