#include "buzzer.h"

void Buzzer_Init(Buzzer_t *buzzer)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = buzzer->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, buzzer->pin, GPIO_PIN_SET);
}

void Buzzer_On(Buzzer_t *buzzer)
{
    HAL_GPIO_WritePin(GPIOB, buzzer->pin, GPIO_PIN_RESET);
}

void Buzzer_Off(Buzzer_t *buzzer)
{
    HAL_GPIO_WritePin(GPIOB, buzzer->pin, GPIO_PIN_SET);
}

void Buzzer_Toggle(Buzzer_t *buzzer)
{
    HAL_GPIO_TogglePin(GPIOB, buzzer->pin);
}

// Buzzer Beep Ctrl

void BuzzerBeepCtrl_Update(BuzzerBeepCtrl_t *ctrl)
{
    if (ctrl->repeating)
    {
        uint32_t now = HAL_GetTick();
        if (now - ctrl->lastToggleTime >= 500)
        {
            Buzzer_Toggle(ctrl->buzzer);
            ctrl->lastToggleTime = now;
        }
    }
}
