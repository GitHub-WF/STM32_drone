#include "Int_LED.h"

LED_Struct LED_1 = {.Port = LED1_GPIO_Port, .Pin = LED1_Pin, .Status = GPIO_PIN_RESET}; // 左上
LED_Struct LED_2 = {.Port = LED2_GPIO_Port, .Pin = LED2_Pin, .Status = GPIO_PIN_RESET}; // 右上
LED_Struct LED_3 = {.Port = LED3_GPIO_Port, .Pin = LED3_Pin, .Status = GPIO_PIN_RESET}; // 右下
LED_Struct LED_4 = {.Port = LED4_GPIO_Port, .Pin = LED4_Pin, .Status = GPIO_PIN_RESET}; // 左下

/**
 * @brief 打开LED
 *
 * @param led 指定的led结构体
 */
void Int_LED_Turn_On(LED_Struct *led)
{
  HAL_GPIO_WritePin(led->Port, led->Pin, GPIO_PIN_RESET);
}

/**
 * @brief 关闭LED
 *
 * @param led 指定的led结构体
 */
void Int_LED_Turn_Off(LED_Struct *led)
{
  HAL_GPIO_WritePin(led->Port, led->Pin, GPIO_PIN_SET);
}

/**
 * @brief 翻转LED
 *
 * @param led 指定的led结构体
 */
void Int_LED_Toggle(LED_Struct *led)
{
  HAL_GPIO_TogglePin(led->Port, led->Pin);
}
