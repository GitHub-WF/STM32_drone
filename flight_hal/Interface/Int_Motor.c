#include "Int_Motor.h"

// 内存管理 C语言中的结构体通常保存在堆中，不会自动垃圾回收 => 建议使用同一个结构体，不断循环使用
Motor_Struct motor_left_0 = {.htim = &htim3, .channel = TIM_CHANNEL_1, .speed = 200};
Motor_Struct motor_left_1 = {.htim = &htim4, .channel = TIM_CHANNEL_4, .speed = 200};
Motor_Struct motor_right_0 = {.htim = &htim2, .channel = TIM_CHANNEL_2, .speed = 200};
Motor_Struct motor_right_1 = {.htim = &htim1, .channel = TIM_CHANNEL_3, .speed = 200};

/**
 * @brief 设置电机速度
 *
 * @param speed 传入的值是比较值（占空比），最大1000，默认200，最小0
 */
void Int_Motor_Set_Speed(Motor_Struct *motor)
{
  if (motor->speed > 1000)
  {
    log_printf("speed is so fast");
    return;
  }
  __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, motor->speed); // 设置指定电机的比较值，控制电机速度
}

/**
 * @brief 启动电机
 *
 * @param motor 指向要启动的电机结构体的指针
 */
void Int_Motor_Start(Motor_Struct *motor)
{
  HAL_TIM_PWM_Start(motor->htim, motor->channel); // 启动指定电机的PWM输出
}
