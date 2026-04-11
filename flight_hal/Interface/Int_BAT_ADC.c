#include "Int_BAT_ADC.h"

// 启动ADC采样
void Int_BAT_ADC_Init(void)
{
  // 打开使能
  HAL_GPIO_WritePin(BAT_ADC_EN_GPIO_Port, BAT_ADC_EN_Pin, GPIO_PIN_RESET);
  // 启动ADC采样
  HAL_ADC_Start(&hadc1);
}

// 读取电池电压
float Int_BAT_ADC_Read(void)
{
  // 读取ADC值
  uint32_t adc_value = HAL_ADC_GetValue(&hadc1);
  // 计算电压
  float voltage = adc_value * 3.3f / 4096.0f;
  return voltage * 2.0f;
}
