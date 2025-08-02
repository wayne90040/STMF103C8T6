#ifndef __OLED_H
#define __OLED_h

// https://github.com/olikraus/u8g2

#include "stm32f1xx_hal.h"

// #define U8X8_WITH_USER_PTR 
#include "u8g2.h"

#define I2C_TIMEOUT 1000
#define DATA_BUFFER_SIZE 128

typedef struct
{
    u8g2_t u8g2;
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
} OLED_t;

HAL_StatusTypeDef OLED_Init(OLED_t *oled);
void OLED_Clear(OLED_t *oled);
void OLED_DrawText(OLED_t *oled, uint8_t x, uint8_t y, const char *text);

#endif