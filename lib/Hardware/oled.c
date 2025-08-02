#include "oled.h"

extern I2C_HandleTypeDef hi2c1;

// HAL I2C bridge
// https://github.com/harebit/STM32CubeMX_and_u8g2/blob/master/u8x8_stm32_HAL.h
static uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8,
                                uint8_t msg,
                                uint8_t arg_int,
                                void *arg_ptr)
{

    OLED_t *oled = (OLED_t *)u8x8_GetUserPtr(u8x8);
    uint8_t *ptr;
    static uint8_t buffer_count;
    static uint8_t buffer[DATA_BUFFER_SIZE + 1];

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
    {
        ptr = arg_ptr;
        for (int i = 1; i <= arg_int; i++)
        {
            buffer[buffer_count] = *(ptr++); // 將 ptr 指向的資料放入 buffer，ptr 往後移動一個 byte
            buffer_count++;
        }
    }
    break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        buffer_count = 0;
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        // Send all at once
        HAL_I2C_Master_Transmit(oled->hi2c, oled->address, (uint8_t *)buffer, buffer_count, I2C_TIMEOUT);
        break;
    default:
        return 0;
    }

    return 1;
}

// u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr
// uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
static uint8_t u8x8_stm32_gpio_and_delay_cb(u8x8_t *u8x8,
                                            uint8_t msg,
                                            uint8_t arg_int,
                                            void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT: // called once during init phase of u8g2/u8x8 -> Make with HAL_init
        break;
    case U8X8_MSG_DELAY_NANO: // delay arg_int * 1 nano second
        break;
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI:
        HAL_Delay(arg_int);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
                             // for (uint16_t n = 0; n < (160*arg_int); n++)
                             //  		{
                             //		 __NOP();
                             //	}
        break;               // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_D0:   // D0 or SPI clock pin: Output level in arg_int
                             // case U8X8_MSG_GPIO_SPI_CLOCK:
        break;
    case U8X8_MSG_GPIO_D1: // D1 or SPI data pin: Output level in arg_int
                           // case U8X8_MSG_GPIO_SPI_DATA:
        break;
    case U8X8_MSG_GPIO_D2: // D2 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D3: // D3 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D4: // D4 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D5: // D5 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D6: // D6 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_D7: // D7 pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_E: // E/WR pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_CS: // CS (chip select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_DC: // DC (data/cmd, A0, register select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_RESET: // Reset pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_CS1: // CS1 (chip select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_CS2: // CS2 (chip select) pin: Output level in arg_int
        break;
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        break;
    }
    return 1;
}

HAL_StatusTypeDef OLED_Init(OLED_t *oled)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &oled->u8g2,
        U8G2_R0,                       // 顯示方向（旋轉 0 度)
        u8x8_byte_hw_i2c,              // byte callback，用來實作 I2C 傳輸
        u8x8_stm32_gpio_and_delay_cb); // GPIO 與延遲 callback，用來控制延遲與 GPIO 腳位。

    u8g2_SetUserPtr(&oled->u8g2, oled);
    u8g2_InitDisplay(&oled->u8g2);
    u8g2_SetPowerSave(&oled->u8g2, 0); // 關閉省電模式，讓 OLED 顯示器開始正常顯示畫面。 0: 關閉 1: 開啟
    u8g2_SetFont(&oled->u8g2, u8g2_font_6x10_tr);

    return HAL_OK;
}

void OLED_Clear(OLED_t *oled)
{
    u8g2_ClearBuffer(&oled->u8g2); // 清空 u8g2 的畫面緩衝區（buffer）
    u8g2_SendBuffer(&oled->u8g2);  // 把剛剛清空的 buffer 資料真正送到 OLED 硬體
}

void OLED_DrawText(OLED_t *oled, uint8_t x, uint8_t y, const char *text)
{
    u8g2_ClearBuffer(&oled->u8g2);
    u8g2_DrawStr(&oled->u8g2, x, y, text);
    u8g2_SendBuffer(&oled->u8g2);
}