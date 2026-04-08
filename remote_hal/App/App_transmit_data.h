#ifndef __APP_TRANSMIT_DATA_H__
#define __APP_TRANSMIT_DATA_H__

#include "Int_SI24R1.h"
#include "App_process_data.h"
#include "FreeRTOS.h"
#include "task.h"

// 定义帧头校验值
#define FRAME_HEADER_1 '6'
#define FRAME_HEADER_2 '6'
#define FRAME_HEADER_3 '6'

/**
 * @brief 自动切换SI24R1模式，将采集数据打包发送到飞机
 * 
 */
void App_transmit_data(void);

#endif /* __APP_TRANSMIT_DATA_H__ */
