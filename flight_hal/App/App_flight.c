#include "App_flight.h"

MPU6050_Data_t mpu6050_data = {0};
Euler_angle euler_angle = {0};

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
