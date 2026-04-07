#ifndef __INT_LED_H__
#define __INT_LED_H__

#include "main.h"

typedef struct
{
  GPIO_TypeDef *Port;
  uint16_t Pin;
  GPIO_PinState Status;
} LED_Struct;

extern LED_Struct LED_1; // 左上
extern LED_Struct LED_2; // 右上
extern LED_Struct LED_3; // 右下
extern LED_Struct LED_4; // 左下

/**
 * @brief 打开LED
 *
 * @param led 指定的led结构体
 */
void Int_LED_Turn_On(LED_Struct *led);

/**
 * @brief 关闭LED
 *
 * @param led 指定的led结构体
 */
void Int_LED_Turn_Off(LED_Struct *led);

/**
 * @brief 翻转LED
 *
 * @param led 指定的led结构体
 */
void Int_LED_Toggle(LED_Struct *led);

#endif /* __INT_LED_H__ */
