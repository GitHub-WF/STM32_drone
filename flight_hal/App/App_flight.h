#ifndef __APP_FLIGHT_H__
#define __APP_FLIGHT_H__

#include "Int_MPU6050.h"
#include "Com_imu.h"

/**
 * @brief 根据陀螺仪数据计算欧拉角
 * 
 */
void App_flight_get_euler_angle(void);

#endif /* __APP_FLIGHT_H__ */
