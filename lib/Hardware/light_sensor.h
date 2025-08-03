#ifndef __LightSensor_H
#define __LightSensor_H

#include "stm32f1xx_hal.h"

typedef struct
{
    uint16_t pin;

} LightSensor;

void LightSensor_Init(LightSensor *sensor);

GPIO_PinState LightSensor_Read(LightSensor *sensor);

#endif
