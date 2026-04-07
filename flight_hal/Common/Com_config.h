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

#endif /* __COM_CONFIG_H__ */
