#include "Int_Key.h"

/**
 * @brief 获取按键状态
 *
 * @return Key_t(KEY_NONE是没有按键按下)
 * */
Key_t Int_Key_Get(void)
{
	// 1.电弧抖动，需要消抖
	vTaskDelay(pdMS_TO_TICKS(5));
  if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
	{
		// 2.人按下时间长，避免多次判断，等待按起再返回
		while(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET) vTaskDelay(pdMS_TO_TICKS(1));
		return KEY_UP;
	}
	else if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
	{
		while(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET) vTaskDelay(pdMS_TO_TICKS(1)); // 为了在while阻塞时，释放CPU资源
		return KEY_DOWN;
	}
	else if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
	{
		while(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET) vTaskDelay(pdMS_TO_TICKS(1));
		return KEY_LEFT;
	}
	else if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
	{
		while(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET) vTaskDelay(pdMS_TO_TICKS(1));
		return KEY_RIGHT;
	}
	else if (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
	{
		while(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET) vTaskDelay(pdMS_TO_TICKS(1));
		return KEY_LEFT_X;
	}
	else if (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
	{
		// 开始计时
		TickType_t start_time = xTaskGetTickCount();
		while(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET) vTaskDelay(pdMS_TO_TICKS(1));
		// 计算按键按下时间
		TickType_t end_time = xTaskGetTickCount();
		if (end_time - start_time > 1000)
		{
			return KEY_RIGHT_X_LONG;
		}
		return KEY_RIGHT_X;
	}
	else
	{
		return KEY_NONE;
	}
}
