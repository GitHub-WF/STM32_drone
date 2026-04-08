#include "App_receive_data.h"

Remote_Data remote_data = {0};

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
    log("rece data: %s", rxbuf);
    // 校验帧头
    if (rxbuf[0] != FRAME_HEADER_1 || rxbuf[1] != FRAME_HEADER_2 || rxbuf[2] != FRAME_HEADER_3)
    {
      log("rece data error");
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
      log("rece data error");
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
    log("rece data error");
    return 1;
  }
}

/**
 * @brief 处理连接状态数据
 *
 * @param res
 */
uint8_t retry_count = 0;
void App_process_connect_data(uint8_t *res)
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
