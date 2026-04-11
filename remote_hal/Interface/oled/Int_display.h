#ifndef __INT_DISPLAY_H__
#define __INT_DISPLAY_H__

#include "main.h"
#include "Inf_OLED.h"
#include "Int_SI24R1.h"
#include "App_process_data.h"

/**
 * @brief 初始化OLED
 *
 */
void Int_OLED_Init(void);

/**
 * @brief 刷新屏幕
 *
 */
void App_OLED_refresh(void);

#endif /* __INT_DISPLAY_H__ */
