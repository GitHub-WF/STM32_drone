#ifndef __INT_JOYSTICK_H__
#define __INT_JOYSTICK_H__

#include "adc.h"

typedef struct
{
  int16_t throttle;
  int16_t yaw;
  int16_t pitch;
  int16_t roll;
} Joystick_t;

/**
 * @brief 初始化ADC遥控，打开ADC
 *
 */
void Int_Joystick_Init(void);

/**
 * @brief 读取ADC数据保存到结构体中
 *
 * @param joystick
 */
void Int_joystick_get(Joystick_t *joystick);

#endif /* __INT_JOYSTICK_H__ */
