#ifndef __APP_PROCESS_DATA_H__
#define __APP_PROCESS_DATA_H__

#include "Int_Joystick.h"
#include "Int_Key.h"
#include "Com_utils.h"

typedef struct
{
  uint16_t throttle;
  uint16_t yaw;
  uint16_t pitch;
  uint16_t roll;
  uint8_t shutdown; // 1 关机 0 不关机
  uint8_t fix_height;
} Remote_Data;

/**
 * @brief 保存按键数据
 * 
 */
void App_process_key_data(void);

/**
 * @brief 保存摇杆数据
 * 
 */
void App_process_joystick_data();

#endif /* __APP_PROCESS_DATA_H__ */
