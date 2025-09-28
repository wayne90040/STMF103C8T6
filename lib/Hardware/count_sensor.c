#include "count_sensor.h"
#include "stm32f1xx_hal.h"

// 外部中斷
void CountSensor_Init()
{
    // __HAL_RCC_GPIOB_CLK_ENABLE();

    // 設定好 GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN; 
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 設定 AFIO，HAL 在設定 Mode 就已經處理好ㄌ

    // 設定 EXTI，HAL 在設定 Mode 就已經處理好ㄌ

    // 設定 NVIC
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// MARK: SPL 作法

// 啟用 GPIOA 和 AFIO 的時脈
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

// 設定 GPIO
// GPIO_InitTypeDef GPIO_InitStructure;
// // 設定 PA0 為輸入模式（浮空或上拉/下拉）
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
// GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IPU;
// GPIO_Init(GPIOA, &GPIO_InitStructure);

// EXTI 腳位映射
// GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

// EXTI 初始化
// EXTI_InitTypeDef EXTI_InitStructure;
// // 設定要配置的 EXTI 線路
// EXTI_InitStructure.EXTI_Line = EXTI_Line14;
// // 設定為中斷模式
// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// // 設定為下降沿觸發
// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
// // 啟用該中斷線
// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// EXTI_Init(&EXTI_InitStructure);

// 設定 NVIC
// NVIC_PriorityGroupConifg(NVIC_PriorityGroup_2); // 整個 CPU 只需要執行一次，NVIC_PriorityGroup_2：4 位元給搶佔優先級，4 位元給子優先級
// NVIC_InitTypeDef NVIC_InitStructure;
// NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_InitStructure.NVIC_IRQChannelPreeptionPriority = 1;
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// NVIC_Init(&NVIC_InitStructure);
