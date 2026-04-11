#ifndef __INT_VL53L1_H__
#define __INT_VL53L1_H__

#include "main.h"
#include "vl53l1_platform.h"
#include "VL53L1X_api.h"
#include "VL53L1X_calibration.h"
#include "FreeRTOS.h"
#include "task.h"

// 初始化激光测距仪
void Int_VL53L1_Init(void);

// 读取距离数据
uint8_t Int_VL53L1_RdDistance(void);

#endif /* __INT_VL53L1_H__ */
