#include "light_sensor.h"
#include <stdbool.h>

void LightSensor_Init(LightSensor *sensor)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = sensor->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

GPIO_PinState LightSensor_Read(LightSensor *sensor)
{
    return HAL_GPIO_ReadPin(GPIOB, sensor->pin);
}
