#ifndef __COM_IMU_H__
#define __COM_IMU_H__
#include "math.h"
#include "Com_config.h"
#include "Int_MPU6050.h"

/* 表示四元数的结构体 */
typedef struct
{
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion_Struct;

typedef struct
{
  float yaw;
  float pitch;
  float roll;
} Euler_angle;

extern float RtA;
extern float Gyro_G;
extern float Gyro_Gr;

void Common_IMU_GetEulerAngle(MPU6050_Data_t *gyroAccel, Euler_angle *eulerAngle, float dt);

float Common_IMU_GetNormAccZ(void);

#endif /* __COM_IMU_H__ */
