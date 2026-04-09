#include "Int_IP5305T.h"

/**
 * @brief 启动电源，避免关机
 *
 */
void Int_IP5305T_Start(void)
{
  HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET); // 设置电源键为高电平，启动电源
  vTaskDelay(100); // 延时100毫秒，确保电源稳定
  HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET); // 设置电源键为低电平，完成启动
}

/**
 * @brief 关机指令
 * 
 */
void Int_IP5305T_Stop(void)
{
  // 双击两次关机（1s之内短按两次）
  HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
  vTaskDelay(100); // 延时100毫秒，确保电源稳定
  HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
  vTaskDelay(200); // 延时100毫秒，确保电源稳定
  HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
  vTaskDelay(100); // 延时100毫秒，确保电源稳定
  HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}
