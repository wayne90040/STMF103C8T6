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
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 上拉（未按下時為高電位）
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    button->last_level = HAL_GPIO_ReadPin(GPIOB, button->pin);
    button->last_tick = HAL_GetTick();
    button->is_pressed = 0;
    button->longpress_detected = 0;
}

// 低電位是按下
ButtonEvent_t Button_Update(Button_t *button)
{
    uint8_t level = HAL_GPIO_ReadPin(GPIOB, button->pin);
    uint32_t now = HAL_GetTick();
    ButtonEvent_t event = BUTTON_STATE_IDLE;

    if (level != button->last_level)
    {
        button->last_tick = now; // 狀態改變，重置去抖動計時
    }
    else
    {
        if (now - button->last_tick >= DEBOUNCE_TIME) // 去抖動判斷，大於 Debounce time
        {
            // 按下事件
            button->is_pressed = 1;
            button->longpress_detected = 0;
            event = BUTTON_STATE_PRESSED;
        }
        else if (button->last_level == GPIO_PIN_RESET && level == GPIO_PIN_SET)
        {
            // 前一次是低電位代表按下，這次是高電位 -> 放開事件
            // 如果之前有按下，並且 沒觸發過長按，那就把這次事件視為「短按放開」。
            if (button->is_pressed)
            {
                if (!button->longpress_detected)
                {
                    event = BUTTON_STATE_RELEASES;
                }
                // 放開就重置狀態
                button->is_pressed = 0;
                button->longpress_detected = 0;
            }
        }
        else if (button->is_pressed && !button->longpress_detected)
        {
            // 長按判斷
            if (now - button->last_tick >= LONGPRESS_TIME)
            {
                button->longpress_detected = 1;
                event = BUTTON_STATE_LONGPRESS;
            }
        }
    }
    button->last_level = level;
    return event;
}

// ButtonEvent_t Button_Update(Button_t *button)
// {
//     uint8_t level = HAL_GPIO_ReadPin(button->port, button->pin);
//     uint32_t now = HAL_GetTick();
//     ButtonEvent_t event = BUTTON_STATE_IDLE;

//     if(level != button->last_level)
//     {
//         // 狀態改變，重置去抖動計時
//         button->last_tick = now;
//     }
//     else
//     {
//         // 去抖動判斷
//         if(now - button->last_tick >= DEBOUNCE_TIME)
//         {
//             if(button->last_level == GPIO_PIN_SET && level == GPIO_PIN_RESET)
//             {
//                 // 按下事件
//                 button->is_pressed = 1;
//                 button->longpress_detected = 0;
//                 event = BUTTON_STATE_PRESSED;
//             }
//             else if(button->last_level == GPIO_PIN_RESET && level == GPIO_PIN_SET)
//             {
//                 // 放開事件
//                 if(button->is_pressed)
//                 {
//                     if(!button->longpress_detected)
//                         event = BUTTON_STATE_RELEASED; // 短按放開
//                     button->is_pressed = 0;
//                     button->longpress_detected = 0;
//                 }
//             }
//             else if(button->is_pressed && !button->longpress_detected)
//             {
//                 // 長按判斷
//                 if(now - button->last_tick >= LONGPRESS_TIME)
//                 {
//                     button->longpress_detected = 1;
//                     event = BUTTON_STATE_LONGPRESS;
//                 }
//             }
//         }
//     }

//     button->last_level = level;
//     return event;
// }

// uint8_t Button_WasPressed(Button_t *button)
// {
//     uint8_t state = 0;
//     uint8_t current_state = HAL_GPIO_ReadPin(GPIOB, button->pin);
//     if (current_state == 0)
//     {
//         HAL_Delay(20);
//         while (HAL_GPIO_ReadPin(GPIOB, button->pin) == 0)
//             ;
//         HAL_Delay(20);
//         state = 1;
//     }
//     return state;
// }

// uint8_t Button_WasPressed2(Button_t *button)
// {
//     uint8_t current_state = HAL_GPIO_ReadPin(button->port, button->pin);
//     uint32_t now = HAL_GetTick();

//     // 上次狀態是高、現在是低 → 有機會是按下
//     if (button->last_state == GPIO_PIN_SET && current_state == GPIO_PIN_RESET)
//     {
//         if (now - button->last_tick > DEBOUNCE_DELAY)
//         {
//             button->last_tick = now;
//             button->last_state = current_state;
//             return 1; // 確認是按下（狀態轉換 + 過了 debounce）
//         }
//     }

//     // 沒變化，更新狀態
//     if (current_state != button->last_state)
//     {
//         button->last_tick = now;
//         button->last_state = current_state;
//     }

//     return 0; // 沒按下
// }