#include "App_flight.h"

MPU6050_Data_t mpu6050_data = {0};
Euler_angle euler_angle = {0};

/**
 * @brief mpu6050初始化和启动电机
 * 
 */
void App_flight_init(void)
{
  // 1. 初始化MPU6050
  Int_MPU6050_Init();
  // 2. 启动电机
  Int_Motor_Start();
}

/**
 * @brief 根据陀螺仪数据计算欧拉角
 * 
 */
void App_flight_get_euler_angle(void)
{
  // 1. 获取陀螺仪数据
  Int_MPU6050_Get_Data(&mpu6050_data);
  // 2. 姿态解算

  // 方式1：使用互补解算（加速度解算，角速度积分）计算欧拉角：
  // 加速度解算（俯仰角和横滚角）
  /* euler_angle.pitch = atan2(mpu6050_data.accel.ay * 1.0, mpu6050_data.accel.az) / 3.14 * 180;
  euler_angle.roll = atan2(mpu6050_data.accel.ax * 1.0, mpu6050_data.accel.az) / 3.14 * 180;
  // 角速度积分（偏航角）
  // 16位ADC值转换成角度制 量程是±2000°/s
  euler_angle.yaw += (mpu6050_data.gyro.gz / 32768.0 * 2000.0) * 0.006; */

  // 方式2：使用四元数解算计算欧拉角
  Common_IMU_GetEulerAngle(&mpu6050_data, &euler_angle, 0.006);

  log_printf("euler:%.2f,%.2f,%.2f\n", euler_angle.yaw, euler_angle.pitch, euler_angle.roll);
}

/**
 * @brief 计算PID输出
 * 
 */
void App_flight_pid_calc()
{
  // 1. 更新外环PID测量值
  // 偏航角
  out_pid_yaw.desire = (remote_data.yaw - 500) / 50.0; // 遥控偏航数据在 0 - 1000，转换成 ±500，角度范围在 ±10°
  out_pid_yaw.measure = euler_angle.yaw; // 外环的测量值是欧拉角的偏航角（通过四元数解算获得）
  in_pid_yaw.measure = mpu6050_data.gyro.gz / 32768.0 * 2000.0; // 内环的测量值是陀螺仪的角速度（切换单位统一为度）
  // 计算串级PID输出
  Common_PID_Calc_chain(&out_pid_yaw, &in_pid_yaw);
  // 俯仰角
  out_pid_pitch.desire = (remote_data.pitch - 500.0) / 50.0; // 遥控俯仰数据在 0 - 1000，转换成 ±500，角度范围在 ±10°
  out_pid_pitch.measure = euler_angle.pitch; // 外环的测量值是欧拉角的俯仰角（通过四元数解算获得）
  in_pid_pitch.measure = mpu6050_data.gyro.gy / 32768.0 * 2000.0; // 内环的测量值是陀螺仪的角速度（切换单位统一为度）
  // 计算串级PID输出
  Common_PID_Calc_chain(&out_pid_pitch, &in_pid_pitch);
  // 横滚角
  out_pid_roll.desire = (remote_data.roll - 500.0) / 50.0; // 遥控横滚数据在 0 - 1000，转换成 ±500，角度范围在 ±10°
  out_pid_roll.measure = euler_angle.roll; // 外环的测量值是欧拉角的横滚角（通过四元数解算获得）
  in_pid_roll.measure = mpu6050_data.gyro.gx / 32768.0 * 2000.0; // 内环的测量值是陀螺仪的角速度（切换单位统一为度）
  // 计算串级PID输出
  Common_PID_Calc_chain(&out_pid_roll, &in_pid_roll);
}

/**
 * @brief 根据PID输出更新电机速度
 * 
 */
void App_flight_update_motor_speed(void)
{
  // 1.判断当前的飞行状态
  switch (flight_state)
  {
  case IDLE:
    // 空闲状态
    // 所有电机速度设为0
    motor_left_0.speed = 0;
    motor_left_1.speed = 0;
    motor_right_0.speed = 0;
    motor_right_1.speed = 0;
    break;
  case NORMAL:
    // 正常状态
    // 根据PID输出更新电机速度 => 向前飞角速度是正误差（+10°），需要PID向后抑制误差，所以前方两个电机PID大，后方两个电机PID小
    motor_left_0.speed = remote_data.throttle + in_pid_pitch.output - in_pid_roll.output + in_pid_yaw.output;
    motor_left_1.speed = remote_data.throttle - in_pid_pitch.output - in_pid_roll.output - in_pid_yaw.output;
    motor_right_0.speed = remote_data.throttle + in_pid_pitch.output + in_pid_roll.output - in_pid_yaw.output;
    motor_right_1.speed = remote_data.throttle - in_pid_pitch.output + in_pid_roll.output + in_pid_yaw.output;
    break;
  case FIX_HEIGHT:
    /* code */
    break;
  case FAIL:
    /* code */
    break;
  }

  // 限制电机速度上限
  motor_left_0.speed = Com_Limit(motor_left_0.speed, 700, 0);
  motor_left_1.speed = Com_Limit(motor_left_1.speed, 700, 0);
  motor_right_0.speed = Com_Limit(motor_right_0.speed, 700, 0);
  motor_right_1.speed = Com_Limit(motor_right_1.speed, 700, 0);

  // 安全设置，油门 < 50，所有电机速度设为0
  if (remote_data.throttle < 50)
  {
    motor_left_0.speed = 0;
    motor_left_1.speed = 0;
    motor_right_0.speed = 0;
    motor_right_1.speed = 0;
  }

  // 设置电机速度
  Int_Motor_Set_Speed(&motor_left_0);
  Int_Motor_Set_Speed(&motor_left_1);
  Int_Motor_Set_Speed(&motor_right_0);
  Int_Motor_Set_Speed(&motor_right_1);
}
