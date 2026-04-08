#include "App_transmit_data.h"

extern Remote_Data remote_data;

uint8_t txbuf[TX_PLOAD_WIDTH] = {0};

/**
 * @brief 自动切换SI24R1模式，将采集数据打包发送到飞机
 * 
 */
void App_transmit_data(void)
{
  // 1.进入TX模式
    Int_SI24R1_TX_Mode();
    // 2.发送数据（唯一性和可靠性）
    // 2.1 帧头校验指定发送给对应的设备 唯一性
    // 2.2 帧尾，添加校验（将发送的数据累加得到值添加到数据末尾）
    // 数据本身是10字节，帧头校验3字节，帧尾校验4字节，共17字节
    txbuf[0] = FRAME_HEADER_1;
    txbuf[1] = FRAME_HEADER_2;
    txbuf[2] = FRAME_HEADER_3;

    taskENTER_CRITICAL();   // 🔒 禁止任务切换
    txbuf[3] = (remote_data.throttle >> 8) & 0xff;
    txbuf[4] = remote_data.throttle & 0xff;
    txbuf[5] = (remote_data.yaw >> 8) & 0xff;
    txbuf[6] = remote_data.yaw & 0xff;
    txbuf[7] = (remote_data.pitch >> 8) & 0xff;
    txbuf[8] = remote_data.pitch & 0xff;
    txbuf[9] = (remote_data.roll >> 8) & 0xff;
    txbuf[10] = remote_data.roll & 0xff;
    txbuf[11] = remote_data.shutdown;
    remote_data.shutdown = 0;
    txbuf[12] = remote_data.fix_height;
    remote_data.fix_height = 0;
    taskEXIT_CRITICAL();   // 🔓 允许任务切换

    uint32_t sum_check = 0;
    for (int i = 3; i < 13; i++) sum_check += txbuf[i];
    txbuf[13] = (sum_check >> 24) & 0xff;
    txbuf[14] = (sum_check >> 16) & 0xff;
    txbuf[15] = (sum_check >> 8) & 0xff;
    txbuf[16] = sum_check & 0xff;

    Int_SI24R1_TxPacket(txbuf);
    // 3.恢复到接收模式
    Int_SI24R1_RX_Mode();
}
