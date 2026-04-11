#ifndef __APP_RECEIVE_DATA_H__
#define __APP_RECEIVE_DATA_H__

#include "Int_SI24R1.h"
#include "Com_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Int_VL53L1.h"
#include "Com_pid.h"

// 定义帧头校验值
#define FRAME_HEADER_1 '6'
#define FRAME_HEADER_2 '6'
#define FRAME_HEADER_3 '6'

// 最大重试次数
#define MAX_RETRY_COUNT 10

extern Remote_Data remote_data;
extern uint8_t voltage_buf[TX_PLOAD_WIDTH];

/**
 * @brief 接收遥控器发送的数据，解析为结构体
 * 
 * @return uint8_t 0 校验通过 1 校验失败
 */
uint8_t App_receive_data(void);

/**
 * @brief 处理连接状态数据
 * 
 * @param res 
 */
void App_process_connect_data(uint8_t res);

/**
 * @brief 处理飞机的飞行状态
 * 
 */
void App_process_flight_data(void);

#endif /* __APP_RECEIVE_DATA_H__ */
