#include "platform/sky/dev/light-sensor.h"

double getExternalLight()
{
	SENSORS_ACTIVATE(light_sensor);
	
	//TODO: capire quale dei due usare
	
	#if 1 //PHOTOSYNTHETIC
		int rawReading = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
		double externalLight = 10 * rawReading / 7;
	#else //TOTAL_SOLAR
		int rawReading = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
		double externalLight = 46 * rawReading / 10;
	#endif
	
	SENSORS_DEACTIVATE(light_sensor);
	
	return externalLight;
}