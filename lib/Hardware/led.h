#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

typedef struct
{
    // GPIO_TypeDef *port;
    uint16_t pin;
} LED_t;

void LED_Init(LED_t *led);
void LED_On(LED_t *led);
void LED_Off(LED_t *led);
void LED_Toggle(LED_t *led);

#endif
