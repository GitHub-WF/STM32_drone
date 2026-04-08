#ifndef __INT_KEY_H__
#define __INT_KEY_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
  KEY_NONE = 0,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_LEFT_X,
  KEY_RIGHT_X,
  KEY_RIGHT_X_LONG,
} Key_t;

/**
 * @brief 获取按键状态
 *
 * @return Key_type(KEY_NONE是没有按键按下)
 * */
Key_t Int_Key_Get(void);

#endif /* __INT_KEY_H__ */
