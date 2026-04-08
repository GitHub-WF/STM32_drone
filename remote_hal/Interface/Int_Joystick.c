#include "Int_Joystick.h"

uint16_t adc_buffer[4];

/**
 * @brief 初始化ADC遥控，打开ADC
 *
 */
void Int_Joystick_Init(void)
{
	// 使用HAL中的启动ADC方法
	// 16位数据地址值其实是32位，32位数据地址值也是32位
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 4);
}

/**
 * @brief 读取ADC数据保存到结构体中
 *
 * @param joystick
 */
void Int_joystick_get(Joystick_t *joystick)
{
	joystick->throttle = adc_buffer[0];
	joystick->yaw = adc_buffer[1];
	joystick->roll = adc_buffer[2];
	joystick->pitch = adc_buffer[3];
}
