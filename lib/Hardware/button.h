#ifndef __Button_H
#define __Button_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef enum
{
    BUTTON_STATE_IDLE = 0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASES,
    BUTTON_STATE_LONGPRESS,
} ButtonEvent_t;

typedef struct
{
    // 角位
    uint16_t pin;
    //  儲存上一次讀取的電平狀態。GPIO_PIN_SET 或 GPIO_PIN_RESET
    GPIO_PinState last_level;
    // 儲存最後一次狀態改變的時間。用來處理長短按以及去抖動
    uint32_t last_tick;
    bool is_pressed;
    // Bool 是否已經偵測過一次長按事件，避免長按多次觸發。
    bool longpress_detected;
} Button_t;

void Button_Init(Button_t *button);
ButtonEvent_t Button_Update(Button_t *button);

#endif