#include "App_receive_data.h"

Remote_Data remote_data = {.throttle = 0, .yaw = 500, .pitch = 500, .roll = 500, .shutdown = 0, .fix_height = 0};

Thr_state thr_state = FREE;

/**
 * @brief 接收遥控器发送的数据，解析为结构体
 *
 * @return uint8_t 0 校验通过 1 校验失败\未接收数据
 */
uint8_t rxbuf[TX_PLOAD_WIDTH + 1] = {0};
uint8_t App_receive_data(void)
{
  // 清空缓冲区
  memset(rxbuf, 0, TX_PLOAD_WIDTH);
  // 使用SI24R1接收数据
  uint8_t res = Int_SI24R1_RxPacket(rxbuf);
  if (res == 0)
  {
    log_printf("rece data: %s", rxbuf);
    // 校验帧头
    if (rxbuf[0] != FRAME_HEADER_1 || rxbuf[1] != FRAME_HEADER_2 || rxbuf[2] != FRAME_HEADER_3)
    {
      log_printf("rece data error");
      return 1;
    }
    // 校验帧尾
    uint32_t sum_check = 0;
    for (int i = 3; i < 13; i++)
    {
      sum_check += rxbuf[i];
    }
    if (((sum_check >> 24) & 0xff) != rxbuf[13] || ((sum_check >> 16) & 0xff) != rxbuf[14] || ((sum_check >> 8) & 0xff) != rxbuf[15] || (sum_check & 0xff) != rxbuf[16])
    {
      log_printf("rece data error");
      return 1;
    }
    // 解析数据
    remote_data.throttle = rxbuf[3] << 8 | rxbuf[4];
    remote_data.yaw = rxbuf[5] << 8 | rxbuf[6];
    remote_data.pitch = rxbuf[7] << 8 | rxbuf[8];
    remote_data.roll = rxbuf[9] << 8 | rxbuf[10];
    remote_data.shutdown = rxbuf[11];
    remote_data.fix_height = rxbuf[12];

    return 0;
  }
  else
  {
    log_printf("rece data error");
    return 1;
  }
}

/**
 * @brief 处理连接状态数据
 *
 * @param res
 */
uint8_t retry_count = 0;
void App_process_connect_data(uint8_t res)
{
  // 接收数据成功一次，则说明连接成功
  if (res == 0)
  {
    remote_state = CONNECTED;
    retry_count = 0;
  }
  else if (res == 1)
  {
    // 数据接收失败，且重试超过10次，则连接失败
    if (retry_count++ >= MAX_RETRY_COUNT)
    {
      remote_state = DISCONNECTED;
    }
  }
}

/**
 * @brief 处理解锁逻辑
 * 
 * @return uint8_t 0 解锁成功 1 解锁失败
 */
TickType_t enter_free_time = 0;
TickType_t enter_leave_time = 0;
static uint8_t App_process_unlock(void)
{
  // 1.考虑安全问题，解锁：油门 > 900 1s => 油门 < 100 1s => 解锁成功
  switch (thr_state)
  {
  case FREE:
    if (remote_data.throttle > 900)
    {
      thr_state = MAX;
      enter_free_time = xTaskGetTickCount();
    }
    break;
  case MAX:
    if (remote_data.throttle < 900)
    {
      if (xTaskGetTickCount() - enter_free_time >= pdMS_TO_TICKS(1000))
      {
        thr_state = LEAVE_MAX;
      }
      else
      {
        thr_state = FREE;
      }
    }
    break;
  case LEAVE_MAX:
    if (remote_data.throttle < 100)
    {
      thr_state = MIN;
      enter_leave_time = xTaskGetTickCount();
    }
    else
    {
      thr_state = FREE;
    }
    break;
  case MIN:
    if (remote_data.throttle >= 100) {
      thr_state = FREE;
    } else if (xTaskGetTickCount() - enter_leave_time >= pdMS_TO_TICKS(1000)) {
      thr_state = UNLOCK;
    }
    break;
  case UNLOCK:
    return 0;
  }

  return 1;
}

/**
 * @brief 处理飞机的飞行状态
 * 
 */
void App_process_flight_data(void)
{
  // 使用状态机实现
  // 1.轮训调用判断当前所处的状态
  switch (flight_state)
  {
  case IDLE:
    // 判断解锁状态
    if (App_process_unlock() == 0)
    {
      flight_state = NORMAL;
      thr_state = FREE;
    }
    break;
  case NORMAL:
    // 判断定高状态
    if (remote_data.fix_height == 1)
    {
      flight_state = FIX_HEIGHT;
      remote_data.fix_height = 0;
      // 进入定高状态时，记录当前高度1
      out_pid_height.desire = Int_VL53L1_RdDistance();
    }
    // 判断失联状态
    if (remote_state == DISCONNECTED)
    {
      flight_state = FAIL;
    }
    break;
  case FIX_HEIGHT:
    // 判断取消定高
    if (remote_data.fix_height == 1)
    {
      flight_state = NORMAL;
      remote_data.fix_height = 0;
    }
    // 判断失联状态
    if (remote_state == DISCONNECTED)
    {
      flight_state = FAIL;
    }
    break;
  case FAIL:
    // 等待故障处理完成进入空闲状态
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 一直等待故障处理完成

    flight_state = IDLE;
    break;
  }
}
