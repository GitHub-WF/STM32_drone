#ifndef __APP_FREERTOS_TASK_H__
#define __APP_FREERTOS_TASK_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Int_IP5305T.h"
#include "Int_SI24R1.h"
#include "App_process_data.h"
#include "App_transmit_data.h"

/**
 * @brief 启动freeRTOS
 *
 */
void App_freeRTOS_Task_Init(void);

#endif /* __APP_FREERTOS_TASK_H__ */
