#include "led.h"

void LED_Init(LED_t *led)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = led->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL; //  浮空 /上拉 / 下拉
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, led->pin, GPIO_PIN_SET);
}

void LED_On(LED_t *led)
{
    HAL_GPIO_WritePin(GPIOA, led->pin, GPIO_PIN_RESET);
}

void LED_Off(LED_t *led)
{
    HAL_GPIO_WritePin(GPIOA, led->pin, GPIO_PIN_SET);
}

void LED_Toggle(LED_t *led)
{
    HAL_GPIO_TogglePin(GPIOA, led->pin);
}