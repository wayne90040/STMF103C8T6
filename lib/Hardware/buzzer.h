#ifndef __Buzzer_H
#define __Buzzer_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
    uint16_t pin;
} Buzzer_t;

void Buzzer_Init(Buzzer_t *buzzer);
void Buzzer_On(Buzzer_t *buzzer);
void Buzzer_Off(Buzzer_t *buzzer);
void Buzzer_Toggle(Buzzer_t *buzzer);

typedef struct
{
    Buzzer_t *buzzer;
    bool repeating;
    uint32_t lastToggleTime;
} BuzzerBeepCtrl_t;

void BuzzerBeepCtrl_Update(BuzzerBeepCtrl_t *ctrl);

#endif
