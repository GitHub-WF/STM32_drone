#include "App_freeRTOS_Task.h"

// STM32F103C8T6的RAM为20KB，预留12KB给系统使用，剩余8KB给任务使用

// 电源管理任务
void power_task(void *arg);
#define POWER_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define POWER_TASK_PRIORITY 4 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t powerTaskHandle;

/**
 * @brief 初始化freeRTOS任务
 *
 */
void App_freeRTOS_Task_Init(void)
{
  // 1.创建任务
  xTaskCreate(power_task, "PowerTask", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &powerTaskHandle);
  // 2.启动调度器
  vTaskStartScheduler();
}

void power_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10000)); // 10秒钟执行一次，激活电源，防止自动关机
    // 启动电源
    Int_IP5305T_Start();
  }
}
