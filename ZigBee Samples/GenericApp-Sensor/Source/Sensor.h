#ifndef SENSOR_H_
#define SENSOR_H_

#include <hal_types.h>


// extern int8 readTemp(void);

extern void Sensor_Init(void);
extern uint8 GetTemperature(void);
void ADC_Init11(void);

#endif