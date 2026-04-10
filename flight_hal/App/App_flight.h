#ifndef __APP_FLIGHT_H__
#define __APP_FLIGHT_H__

#include "Int_MPU6050.h"
#include "Com_imu.h"
#include "Com_pid.h"
#include "App_receive_data.h"
#include "Com_config.h"
#include "Int_motor.h"

typedef enum
{
  YAW_PID = 0,
  PITCH_PID,
  ROLL_PID,
} PID_Type;

/**
 * @brief mpu6050初始化和启动电机
 * 
 */
void App_flight_init(void);

/**
 * @brief 根据陀螺仪数据计算欧拉角
 * 
 */
void App_flight_get_euler_angle(void);

/**
 * @brief 计算PID输出
 * 
 */
void App_flight_pid_calc(void);

/**
 * @brief 根据PID输出更新电机速度
 * 
 */
void App_flight_update_motor_speed(void);

#endif /* __APP_FLIGHT_H__ */
