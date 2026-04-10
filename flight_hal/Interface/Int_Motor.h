#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

typedef struct
{
  TIM_HandleTypeDef *htim; // 定时器句柄
  uint32_t channel;        // 定时器通道
  int16_t speed;               // 当前速度（占空比），传入的值是比较值（占空比），最大1000，默认200，最小0
} Motor_Struct;

extern Motor_Struct motor_left_0;
extern Motor_Struct motor_left_1;
extern Motor_Struct motor_right_0;
extern Motor_Struct motor_right_1;

/**
 * @brief 设置电机速度
 *
 * @param motor 指向要设置速度的电机结构体的指针
 */
void Int_Motor_Set_Speed(Motor_Struct *motor);

/**
 * @brief 启动电机
 *
 */
void Int_Motor_Start();

#endif /* __INT_MOTOR_H__ */
