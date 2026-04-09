#include "App_freeRTOS_Task.h"

// STM32F103C8T6的RAM为20KB，预留12KB给系统使用，剩余8KB给任务使用

// 电源管理任务
void power_task(void *arg);
#define POWER_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define POWER_TASK_PRIORITY 4 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t powerTaskHandle;
#define POWER_TASK_PERIOD_MS 10000 // 任务执行间隔，单位为毫秒，这里设置为10秒钟

// 飞行控制任务
void flight_task(void *arg);
#define FLIGHT_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define FLIGHT_TASK_PRIORITY 3 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t flightTaskHandle;
#define FLIGHT_TASK_PERIOD_MS 6 // 任务执行间隔，单位为毫秒，这里设置为6毫秒

// LED控制任务
void led_task(void *arg);
#define LED_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define LED_TASK_PRIORITY 1 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
TaskHandle_t ledTaskHandle;
#define LED_TASK_PERIOD_MS 100 // 任务执行间隔，单位为毫秒，这里设置为100毫秒

// 通讯任务
void comm_task(void *arg);
#define COMM_TASK_STACK_SIZE 128 // 任务栈大小，单位为字节，128字节对于这个简单的任务来说足够了
#define COMM_TASK_PRIORITY 2 // 任务优先级，数值越大优先级越高（不推荐使用优先级 0）
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
  xTaskCreate(flight_task, "FlightTask", FLIGHT_TASK_STACK_SIZE, NULL, FLIGHT_TASK_PRIORITY, &flightTaskHandle);
  xTaskCreate(led_task, "LEDTask", LED_TASK_STACK_SIZE, NULL, LED_TASK_PRIORITY, &ledTaskHandle);
  xTaskCreate(comm_task, "CommTask", COMM_TASK_STACK_SIZE, NULL, COMM_TASK_PRIORITY, &commTaskHandle);
  // 2.启动调度器
  vTaskStartScheduler();
}

void power_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    /* vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(POWER_TASK_PERIOD_MS)); // 10秒钟执行一次，激活电源，防止自动关机
    // 启动电源
    Int_IP5305T_Start(); */

    // 使用直接任务通知的接受方式实现10s处理一次
    // 一直等任务通知，收到通知res = 1, 超时res = 0
    uint32_t res = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(POWER_TASK_PERIOD_MS));
    if (res != 0)
    {
      Int_IP5305T_Stop();
    }
    else
    {
      // 10s内未收到通知，保持当前状态
      Int_IP5305T_Start();
    }
  }
}

void flight_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  // 初始化MPU6050
  Int_MPU6050_Init();
  while (1)
  {
    // 1.获取三轴加速度和角速度
    App_flight_get_euler_angle();

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(FLIGHT_TASK_PERIOD_MS)); // 6毫秒控制一次电机速度，保持电机可控运转
  }
}

void led_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间

  uint8_t count = 0;
  while (1)
  {
    // 判断连接状态
    if (remote_state == CONNECTED)
    {
      Int_LED_Turn_On(&LED_1);
      Int_LED_Turn_On(&LED_2);
    }
    else
    {
      Int_LED_Turn_Off(&LED_1);
      Int_LED_Turn_Off(&LED_2);
    }
    // 判断飞行状态
    if (flight_state == IDLE)
    {
      // 空闲慢闪 500ms
      if (count % 5 == 0)
      {
        Int_LED_Toggle(&LED_3);
        Int_LED_Toggle(&LED_4);
      }
    }
    else if (flight_state == NORMAL)
    {
      // 正常快闪 200ms
      if (count % 2 == 0)
      {
        Int_LED_Toggle(&LED_3);
        Int_LED_Toggle(&LED_4);
      }
    }
    else if (flight_state == FIX_HEIGHT)
    {
      // 定高常亮
      Int_LED_Turn_On(&LED_3);
      Int_LED_Turn_On(&LED_4);
    }
    else if (flight_state == FAIL)
    {
      // 故障常灭
      Int_LED_Turn_Off(&LED_3);
      Int_LED_Turn_Off(&LED_4);
    }

    if (count == 10) count = 0; // 防止溢出

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(LED_TASK_PERIOD_MS)); // 100ms处理一次led
  }
}

void comm_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    // 接收数据，并处理
    uint8_t res = App_receive_data();
    
    // 处理连接状态
    App_process_connect_data(res);

    // 处理关机指令
    if (remote_data.shutdown == 1)
    {
      // 关机（这种方式功能可以实现，但是项目结构很奇怪）
      // Int_IP5305T_Stop();
      // 使用直接任务通知，通知电源任务执行关机
      xTaskNotifyGive(powerTaskHandle);
    }

    // 处理飞机飞行状态
    App_process_flight_data();

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(COMM_TASK_PERIOD_MS)); // 6毫秒执行一次
  }
}
