#include "App_process_data.h"

// 区分按键微调和摇杆控制
int16_t key_pitch_offset = 0; // 向前为正
int16_t key_roll_offset = 0; // 向右为正

// 摇杆数据
Joystick_t joystick = {0};

//记录摇杆数据
int16_t joystick_cel_data[4] = {0};
// 校准摇杆函数
void App_joystick_calibrate(void)
{
  // 情况按键微调值
  key_pitch_offset = 0;
  key_roll_offset = 0;
  // 1.校准摇杆：THR 0, PITCH、ROLL、YAW 500
  // 测量十次平均值
  uint16_t ThrSum = 0;
  uint16_t YawSum = 0;
  uint16_t RollSum = 0;
  uint16_t PitchSum = 0;
  for (uint8_t i = 0; i < 10; i++)
  {
    App_process_joystick_data();
    ThrSum += joystick.throttle - 0;
    YawSum += joystick.yaw - 500;
    RollSum += joystick.roll - 500;
    PitchSum += joystick.pitch - 500;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  // 2.计算校准值
  ThrSum /= 10;
  YawSum /= 10;
  RollSum /= 10;
  PitchSum /= 10;
  // 3.校准摇杆
  joystick_cel_data[0] += ThrSum;
  joystick_cel_data[1] += YawSum;
  joystick_cel_data[2] += RollSum;
  joystick_cel_data[3] += PitchSum;
}

Remote_Data remote_data = {0};

// 这里会有线程安全问题，按键和摇杆数据访问了同一个数据，需要在处理按键数据时加锁
// 1.使用临界区解决
// 2.使用信号量解决

/**
 * @brief 保存按键数据
 *
 */
void App_process_key_data(void)
{
  // 获取按键状态
  Key_t key = Int_Key_Get();
  // 如果进行校准，将按键微调值改为0
  if (key != KEY_NONE)
  {
    // 处理按键事件
    switch (key)
    {
    case KEY_UP:
      // 处理UP键按下事件
      key_pitch_offset += 10; // 俯仰角
      break;
    case KEY_DOWN:
      // 处理DOWN键按下事件
      key_pitch_offset -= 10;
      break;
    case KEY_LEFT:
      // 处理LEFT键按下事件
      key_roll_offset -= 10; // 横滚角
      break;
    case KEY_RIGHT:
      // 处理RIGHT键按下事件
      key_roll_offset += 10;
      break;
    case KEY_LEFT_X:
      // 处理LEFT_X键按下事件（关机）
      remote_data.shutdown = 1;
      break;
    case KEY_RIGHT_X:
      // 处理RIGHT_X键按下事件（定高）
      remote_data.fix_height = 1;
      break;
    case KEY_RIGHT_X_LONG:
      // 处理RIGHT_X_LONG键按下事件（校准摇杆：THR 0, PITCH、ROLL、YAW 500）
      App_joystick_calibrate();
      break;
    }
  }
}

/**
 * @brief 保存摇杆数据
 *
 */
void App_process_joystick_data()
{
  // 解决线程安全问题
  // 进入临界区
  taskENTER_CRITICAL();

  // 1.读取摇杆数据ADC值
  Int_joystick_get(&joystick);

  // 2.处理范围和极性（使用范围是0 - 1000 => ADC范围是0 - 4095）
  joystick.throttle = 1000 - joystick.throttle / 4095 * 1000;
  joystick.yaw = 1000 - joystick.yaw / 4095 * 1000;
  joystick.roll = 1000 - joystick.roll / 4095 * 1000;
  joystick.pitch = 1000 - joystick.pitch / 4095 * 1000;

  // 3.零点便宜校准
  joystick.throttle -= joystick_cel_data[0];
  joystick.yaw -= joystick_cel_data[1];
  joystick.roll -= joystick_cel_data[2];
  joystick.pitch -= joystick_cel_data[3];

  // 4.加上按键微调值
  joystick.throttle += key_pitch_offset;
  joystick.roll += key_roll_offset;

  // 5.可能会超出范围（限制0 - 1000）
  Com_utils_limit(&joystick.throttle, 0, 1000);
  Com_utils_limit(&joystick.yaw, 0, 1000);
  Com_utils_limit(&joystick.roll, 0, 1000);
  Com_utils_limit(&joystick.pitch, 0, 1000);

  remote_data.throttle = joystick.throttle;
  remote_data.yaw = joystick.yaw;
  remote_data.roll = joystick.roll;
  remote_data.pitch = joystick.pitch;

  log("joystick:%d,%d, %d,%d\n", joystick.throttle, joystick.yaw, joystick.roll, joystick.pitch);

  // 退出临界区
  taskEXIT_CRITICAL();
}
