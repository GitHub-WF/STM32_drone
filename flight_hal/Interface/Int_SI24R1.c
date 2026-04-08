#include "Int_SI24R1.h"

// 定义静态的发送地址（接收地址和发送地址相同）
static const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x06, 0x0E, 0x01}; // 定义一个静态发送地址

// SPI读写一个字节（写入是传入参数，读取是返回值）
static uint8_t SPI_RW(uint8_t byte)
{
  uint8_t rx_byte;
  HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_byte, 1, 1000);
  return rx_byte;
}

/********************************************************
功能描述：写寄存器的值（单字节）
入参描述：reg:寄存器映射地址，格式：SI24R1_SI24R1_READ_REG|reg
          value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
uint8_t Int_SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
  uint8_t status;

  CS_LOW;
  status = SPI_RW(reg);
  SPI_RW(value);
  CS_HIGH;

  return status;
}

/********************************************************
功能描述：写寄存器的值（多字节）
入参描述：reg:寄存器映射地址，格式：SI24R1_WRITE_REG|reg
          pBuf:写入数据首地址
          size:写入数据字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t size)
{
  uint8_t status, byte_ctr;

  CS_LOW;
  status = SPI_RW(reg);
  for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
  {
    SPI_RW(*pBuf++);
  }
  CS_HIGH;

  return (status);
}

/********************************************************
功能描述：读取寄存器的值（单字节）
入参描述：reg:寄存器映射地址，格式：SI24R1_READ_REG|reg
返回  值：寄存器值
*********************************************************/
uint8_t Int_SI24R1_Read_Reg(uint8_t reg)
{
  uint8_t value;

  CS_LOW;
  SPI_RW(reg);
  value = SPI_RW(0);
  CS_HIGH;

  return value;
}

/********************************************************
功能描述：读取寄存器的值（多字节）
入参描述：reg:寄存器映射地址：SI24R1_READ_REG|reg
          pBuf:接收缓冲区数据首地址
          size:读取字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
  uint8_t status, byte_ctr;

  CS_LOW;
  status = SPI_RW(reg);
  for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
  {
    pBuf[byte_ctr] = SPI_RW(0); // 读取数据，低字节在前
  }
  CS_HIGH;

  return status;
}

/********************************************************
功能描述：SI24R1接收模式初始化
入参描述：无
返回  值：无
*********************************************************/
void Int_SI24R1_RX_Mode(void)
{
  EN_LOW;
  Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 接收设备接收通道0使用和发送设备相同的发送地址
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);                          // 使能接收通道0自动应答
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);                      // 使能接收通道0
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, 40);                            // 选择射频通道40
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);             // 接收通道0选择和发送通道相同的有效数据宽度
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);                       // 数据传输率1Mbps，发射功率4dBm
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0f);                         // CRC使能，16位CRC校验，上电，接收模式
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);                         // 清除所有的中断标志位
  EN_HIGH;                                                                       // 拉高CE，使能接收设备
}

/********************************************************
功能描述：SI24R1发送模式初始化
入参描述：无
返回  值：无
*********************************************************/
void Int_SI24R1_TX_Mode(void)
{
  EN_LOW;
  Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写入发送地址
  Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 为了应答接收设备，接收通道0地址和发送地址相同

  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);      // 使能接收通道0自动应答
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);  // 使能接收通道0
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0a); // 自动重发等待250us+86us，自动重发10次
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, 40);        // 选择射频通道0x40
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);   // 数据传输率1Mbps，发射功率4dBm
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0e);     // CRC使能，16位CRC校验，上电
  EN_HIGH;
}

/********************************************************
功能描述：接收一包数据，硬件直接接收到数据放到FIFO中，通过状态标志位判断队列中是否有数据
入参描述：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{
  uint8_t state;
  state = Int_SI24R1_Read_Reg(STATUS);                    // 读取状态寄存器的值
  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // 清除RX_DS中断标志

  if (state & RX_DR) // 接收到数据
  {
    Int_SI24R1_Read_Buf(SI24R1_RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); // 读取数据
    Int_SI24R1_Write_Reg(SI24R1_FLUSH_RX, 0xff);                    // 清除RX FIFO寄存器
    return 0;
  }
  return 1; // 没收到任何数据
}

/********************************************************
功能描述：发送一包数据包
入参描述：txbuf:要发送的数据
返回值：	0x10:达到最大重发次数，数据发送失败
          0x20:发送成功
          0xff:发送失败
*********************************************************/
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
  uint8_t state;
  EN_LOW;                                                          // CE拉高，使能发送
  Int_SI24R1_Write_Buf(SI24R1_WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // 写数据到TX FIFO,32个字节
  EN_HIGH;                                                         // CE拉低，使能SI24R1配置

  do
  {
    state = Int_SI24R1_Read_Reg(STATUS); // 读取状态寄存器的值
  } while ((state & TX_DS) != 1 && (state & MAX_RT) != 1);

  Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // 清除TX_DS或MAX_RT中断标志
  if (state & TX_DS)                                      // 发送完成
  {
    return TX_DS;
  }
  if (state & MAX_RT) // 达到最大重发次数
  {
    Int_SI24R1_Write_Reg(SI24R1_FLUSH_TX, 0xff); // 清除TX FIFO寄存器
    return MAX_RT;
  }
  return 0xff; // 发送失败
}

uint8_t Int_SI24R1_Chect()
{
  // 0.需要先读取一次保证SPI通讯正常
  Int_SI24R1_Read_Reg(STATUS);
  // 1.测试SPI通讯是否正常读取状态寄存器
  Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); // 写入发送地址
  // 2.读取发送地址
  uint8_t tx_addr[TX_ADR_WIDTH];
  Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, tx_addr, TX_ADR_WIDTH); // 读取发送地址

  uint8_t state = 1;
  for (uint8_t i = 0; i < TX_ADR_WIDTH; i++)
  {
    if (tx_addr[i] != TX_ADDRESS[i])
    {
      state = 0;
    }
  }
  return state;
}

// 硬件接口层的初始化
void Int_SI24R1_Init(void)
{
  HAL_Delay(200);
  // 1.检查SPI是否可以正常通信
  while (Int_SI24R1_Chect() == 1)
  {
    HAL_Delay(10); // 检查间隔10ms
  }
  // 2.设置默认接收模式
  Int_SI24R1_RX_Mode();
  log("SI24R1 init success");
}
