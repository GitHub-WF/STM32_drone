#ifndef __APP_FREERTOS_TASK_H__
#define __APP_FREERTOS_TASK_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Int_IP5305T.h"
#include "Int_Motor.h"

/**
 * @brief 启动freeRTOS
 *
 */
void App_freeRTOS_Task_Init(void);

#endif /* __APP_FREERTOS_TASK_H__ */
