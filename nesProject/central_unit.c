#include <stdio.h>
#include "contiki.h"

PROCESS(central_unit_main, "Central Unit Main Process");

AUTOSTART_PROCESSES(&central_unit_main);

PROCESS_THREAD(central_unit_main, ev, data)
{
    PROCESS_BEGIN();

        printf("Hello World \n");

    PROCESS_END();
}