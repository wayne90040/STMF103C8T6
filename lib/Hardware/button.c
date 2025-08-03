#include "button.h"

#define DEBOUNCE_TIME 50    // 去抖動時間(ms)
#define LONGPRESS_TIME 1000 // 長按時間(ms)

void Button_Init(Button_t *button)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = button->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    button->last_level = HAL_GPIO_ReadPin(GPIOB, button->pin);
    button->last_tick = HAL_GetTick();
    button->is_pressed = false;
    button->longpress_detected = false;
}

ButtonEvent_t Button_Update(Button_t *button)
{
    uint8_t level = HAL_GPIO_ReadPin(GPIOB, button->pin);
    uint32_t now = HAL_GetTick();
    ButtonEvent_t event = BUTTON_STATE_IDLE;

    if (level != button->last_level)
    {
        button->last_tick = now; // 狀態改變，重置去抖動計時
    }

    // 狀態已穩定一段時間，進行事件判斷
    if (now - button->last_tick >= DEBOUNCE_TIME)
    {
        if (level == GPIO_PIN_RESET && !button->is_pressed)
        {
            // 按下事件
            button->is_pressed = true;
            button->longpress_detected = false;
            event = BUTTON_STATE_PRESSED;
        }

        if (level == GPIO_PIN_SET && button->is_pressed)
        {
            // 鬆開事件，長按事件不應該觸發短按的鬆開事件
            if (!button->longpress_detected)
            {
                event = BUTTON_STATE_RELEASES;
            }
            button->is_pressed = false;
            button->longpress_detected = false;
        }

        if (level == GPIO_PIN_RESET && button->is_pressed && !button->longpress_detected)
        {
            if (now - button->last_tick >= LONGPRESS_TIME)
            {
                button->longpress_detected = true;
                event = BUTTON_STATE_LONGPRESS;
            }
        }
    }

    button->last_level = level;
    return event;
}