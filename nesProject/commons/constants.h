#ifndef NESPROJECT_CONSTANTS_H
#define NESPROJECT_CONSTANTS_H

//Debug flag
#define DEBUG 0

//Command input
#define COMMAND_TIMEOUT 4
#define COMMANDS_NUMBER 6

//Command codes. All codes except ALARM_TOGGLE are used in transmission too
#define ALARM_TOGGLE_COMMAND 1
#define GATELOCK_TOGGLE_COMMAND 2
#define DOORS_OPEN_COMMAND 3
#define AVERAGE_TEMPERATURE_COMMAND 4
#define LIGHT_VALUE_COMMAND 5
#define SHUT_OFF_LIGHTS_COMMAND 6

//Alarm 
#define ALARM_ON_COMMAND 1
#define ALARM_OFF_COMMAND 0

#define ALARM_LED_PERIOD 2

//Mailbox
#define MBOX_EMPTY 1
#define MBOX_FULL 2

#define MBOX_PULL_PERIOD_MINUTES 20

//Auto-opening
#define AUTO_OPENING_LED_PERIOD 2
#define AUTO_OPENING_BLINKINGS 16
#define DOOR_AUTO_OPENING_DELAY 14

//Temperature reading
#define TEMPERATURE_MEASURING_PERIOD 10
#define MAX_TEMPERATURE_READINGS 5
#define RANDOM_TEMPERATURE_PERTURBATION 1

//Lights adjustment
#define LIGHT_ADJUSTMENT_PERIOD_SECONDS 4
#define LIGHT_ADJUSTMENT_RESPONSE_TIME_SECONDS 20
#define LIGHT_TARGET_INTENSITY 300

//Runicast
#define MAX_RETRANSMISSIONS 5

#endif //NESPROJECT_CONSTANTS_H
