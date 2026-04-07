#include "App_freeRTOS_Task.h"

// STM32F103C8T6的RAM为20KB，预留12KB给系统使用，剩余8KB给任务使用

// 电源管理任务
void power_task(void *arg);
#define POWER_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define POWER_TASK_PRIORITY 4 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t powerTaskHandle;
#define POWER_TASK_PERIOD_MS 10000 // 任务执行间隔，单位为毫秒，这里设置为10秒钟

// 通讯任务
void comm_task(void *arg);
#define COMM_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define COMM_TASK_PRIORITY 3 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t commTaskHandle;
#define COMM_TASK_PERIOD_MS 6 // 任务执行间隔，单位为毫秒，这里设置为6毫秒

/**
 * @brief 初始化freeRTOS任务
 *
 */
void App_freeRTOS_Task_Init(void)
{
  // 1.创建任务
  xTaskCreate(power_task, "PowerTask", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &powerTaskHandle);
  xTaskCreate(comm_task, "CommTask", COMM_TASK_STACK_SIZE, NULL, COMM_TASK_PRIORITY, &commTaskHandle);
  // 2.启动调度器
  vTaskStartScheduler();
}

void power_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(POWER_TASK_PERIOD_MS)); // 10秒钟执行一次，激活电源，防止自动关机
    // 启动电源
    Int_IP5305T_Start();
  }
}

uint8_t txbuf[TX_PLOAD_WIDTH] = {0};
void comm_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    // 使用SI24R1发送数据
    // 1.进入TX模式
    Int_SI24R1_TX_Mode();
    // 2.发送数据
    txbuf[0] = 'h';
    txbuf[1] = 'e';
    txbuf[2] = 'l';
    txbuf[3] = 'l';
    txbuf[4] = 'o';
    Int_SI24R1_TxPacket(txbuf);
    // 3.恢复到接收模式
    Int_SI24R1_RX_Mode();

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(COMM_TASK_PERIOD_MS)); // 6毫秒执行一次
  }
}
