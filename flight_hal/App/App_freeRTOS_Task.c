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
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(POWER_TASK_PERIOD_MS)); // 10秒钟执行一次，激活电源，防止自动关机
    // 启动电源
    Int_IP5305T_Start();
  }
}

void flight_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  // 启动电机
  Int_Motor_Start(&motor_left_0);
  Int_Motor_Start(&motor_left_1);
  Int_Motor_Start(&motor_right_0);
  Int_Motor_Start(&motor_right_1);
  while (1)
  {
    motor_left_0.speed = 500;
    motor_left_1.speed = 600;
    motor_right_0.speed = 700;
    motor_right_1.speed = 800;
    Int_Motor_Set_Speed(&motor_left_0);
    Int_Motor_Set_Speed(&motor_left_1);
    Int_Motor_Set_Speed(&motor_right_0);
    Int_Motor_Set_Speed(&motor_right_1);
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

uint8_t rxbuf[TX_PLOAD_WIDTH] = {0};
void comm_task(void *arg)
{
  TickType_t lastWakeTime = xTaskGetTickCount(); // 获取当前系统时间
  while (1)
  {
    // 使用SI24R1接收数据
    uint8_t res = Int_SI24R1_RxPacket(rxbuf);
    if (res == 0)
    {
      log("rece data: %s", rxbuf);
    }

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(COMM_TASK_PERIOD_MS)); // 6毫秒执行一次
  }
}
