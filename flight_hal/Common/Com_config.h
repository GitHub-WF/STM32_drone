#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "main.h"

// 连接状态
typedef enum
{
  DISCONNECTED = 0,
  CONNECTED,
} Remote_state;
extern Remote_state remote_state;

// 飞行状态
typedef enum
{
  IDLE = 0,
  NORMAL,
  FIX_HEIGHT,
  FAIL,
} Flight_state;
extern Flight_state flight_state;

typedef struct
{
  uint16_t throttle;
  uint16_t yaw;
  uint16_t pitch;
  uint16_t roll;
  uint8_t shutdown; // 1 关机 0 不关机
  uint8_t fix_height; // 1 固定高度 0 不固定高度
} Remote_Data;
extern Remote_Data remote_data;

#endif /* __COM_CONFIG_H__ */
