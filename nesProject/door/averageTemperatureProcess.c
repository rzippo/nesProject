#include "averageTemperatureProcess.h"

#include "stdio.h"
#include "contiki.h"
#include "dev/sht11/sht11-sensor.h"

#include "commons/constants.h"

#if RANDOM_TEMPERATURE_PERTURBATION
	#include "lib/random.h"
#endif

double averageTemperature;

double average(double v[], int size)
{
	double sum = 0;
	int i;
	for (i = 0; i < size; ++i)
	{
		sum += v[i];
	}
	return sum/size;
}

PROCESS(averageTemperatureProcess, "Temperature measuring process");

PROCESS_THREAD(averageTemperatureProcess, ev, data)
{
	static struct etimer temperatureTimer;
	static double temperatureReadings[MAX_TEMPERATURE_READINGS];
	static int lastTemperatureIndex = 0;
	static int didIndexWrapAround = 0;
	
	PROCESS_BEGIN();
		#if RANDOM_TEMPERATURE_PERTURBATION
		random_init(150);
		#endif
		etimer_set(&temperatureTimer, TEMPERATURE_MEASURING_PERIOD * CLOCK_SECOND);
		
		while(1)
		{
			PROCESS_WAIT_UNTIL(ev == PROCESS_EVENT_TIMER && etimer_expired(&temperatureTimer));
			
			SENSORS_ACTIVATE(sht11_sensor);
				int rawTemperatureReading = sht11_sensor.value(SHT11_SENSOR_TEMP);
			SENSORS_DEACTIVATE(sht11_sensor);
			
			// Sht11 header file tells us that this is the conversion formula
			// Temperature in Celsius (t in 14 bits resolution at 3 Volts)
			// T = -39.60 + 0.01*t
			
			double measuredTemperature = -39.60 + 0.01*rawTemperatureReading;
			//printf("Temp measured %d\n", (int)measuredTemperature);
			
			#if RANDOM_TEMPERATURE_PERTURBATION
			//Generates a random number from -10 to +10, to be used as percentual perturbation
			int randomPerturbation = (random_rand() % 21) - 10;
			
			double multiplyingCoefficient = ((double)(100 + randomPerturbation))/100;
			double modifiedTemperature = measuredTemperature * multiplyingCoefficient;

			temperatureReadings[lastTemperatureIndex] = modifiedTemperature;
			#else
			temperatureReadings[lastTemperatureIndex] = measuredTemperature;
			#endif

			lastTemperatureIndex = (lastTemperatureIndex + 1) % MAX_TEMPERATURE_READINGS;
			
			if(lastTemperatureIndex == 0)
				didIndexWrapAround = 1;
			
			if(didIndexWrapAround)
				averageTemperature = average(temperatureReadings, MAX_TEMPERATURE_READINGS);
			else
				averageTemperature = average(temperatureReadings, lastTemperatureIndex);
			
			etimer_reset(&temperatureTimer);
		}
	
	PROCESS_END();
}
