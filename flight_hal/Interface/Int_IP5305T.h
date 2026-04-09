#ifndef __INT_IP5305T_H__
#define __INT_IP5305T_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief 启动电源，避免关机
 *
 */
void Int_IP5305T_Start(void);

/**
 * @brief 关机指令
 * 
 */
void Int_IP5305T_Stop(void);

#endif /* __INT_IP5305T_H__ */
