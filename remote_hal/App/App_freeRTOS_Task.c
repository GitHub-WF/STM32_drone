#include "App_freeRTOS_Task.h"

// STM32F103C8T6的RAM为20KB，预留12KB给系统使用，剩余8KB给任务使用

// 电源管理任务
void power_task(void *arg);
#define POWER_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define POWER_TASK_PRIORITY 4 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t powerTaskHandle;
#define POWER_TASK_PERIOD_MS 10000 // 任务执行间隔，单位为毫秒，这里设置为10秒钟

// 按键任务
void key_task(void *arg);
#define KEY_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define KEY_TASK_PRIORITY 2 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t keyTaskHandle;
#define KEY_TASK_PERIOD_MS 20 // 任务执行间隔，单位为毫秒，这里设置为20毫秒

// 摇杆任务
void joystick_task(void *arg);
#define JOYSTICK_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define JOYSTICK_TASK_PRIORITY 2 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t joystickTaskHandle;
#define JOYSTICK_TASK_PERIOD_MS 20 // 任务执行间隔，单位为毫秒，这里设置为20毫秒

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
  xTaskCreate(key_task, "KeyTask", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &keyTaskHandle);
  xTaskCreate(joystick_task, "JoystickTask", JOYSTICK_TASK_STACK_SIZE, NULL, JOYSTICK_TASK_PRIORITY, &joystickTaskHandle);
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

void key_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    // 处理按键数据
    App_process_key_data();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(KEY_TASK_PERIOD_MS)); // 20毫秒执行一次
  }
}

void joystick_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  // 1.初始化摇杆
  Int_Joystick_Init();
  while (1)
  {
    // 处理摇杆数据
    App_process_joystick_data();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(JOYSTICK_TASK_PERIOD_MS)); // 20毫秒执行一次
  }
}

void comm_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    // 使用SI24R1发送数据
    App_transmit_data();

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(COMM_TASK_PERIOD_MS)); // 6毫秒执行一次
  }
}
