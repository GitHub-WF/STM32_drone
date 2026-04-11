#include "Com_pid.h"

// 后续需要进行PID调参，当前参数为默认值
// PID调参，先调内环，再调外环参数
PID_Struct out_pid_pitch = {.kp = -7.0, .ki = 0.0, .kd = 0.0};
PID_Struct in_pid_pitch = {.kp = 3.0, .ki = 0.0, .kd = 0.5};
PID_Struct out_pid_roll = {.kp = -7.0, .ki = 0.0, .kd = 0.0};
PID_Struct in_pid_roll = {.kp = 3.0, .ki = 0.0, .kd = 0.5};
PID_Struct out_pid_yaw = {.kp = -3.0, .ki = 0.0, .kd = 0.0};
PID_Struct in_pid_yaw = {.kp = -5.0, .ki = 0.0, .kd = 0.0};

PID_Struct out_pid_height = {.kp = -0.6, .ki = 0.0, .kd = -0.2};

// 单次PID计算
void Common_PID_Calc(PID_Struct *pid)
{
  // 1.计算当前误差值
  pid->err = pid->desire - pid->measure;
  // 2.计算积分误差
  pid->integral += pid->err;
  // 3.计算微分误差
  if (pid->err_last == 0) // 处理第一次last_err为0的情况
  {
    pid->err_last = pid->err;
  }
  float des = pid->err - pid->err_last;
  // 4.更新上一次误差值
  pid->err_last = pid->err;
  // 5.计算输出值
  pid->output = pid->kp * pid->err + pid->ki * pid->integral * PID_PERIOD + pid->kd * des / PID_PERIOD;
}

// 串级PID计算
void Common_PID_Calc_chain(PID_Struct *out_pid, PID_Struct *in_pid)
{
  // 先计算外环
  Common_PID_Calc(out_pid);
  // 外环输出作为内环目标值
  in_pid->desire = out_pid->output;
  // 再计算内环
  Common_PID_Calc(in_pid);
}

// 限制电机转速
float Com_Limit(float speed, float max, float min)
{
  if (speed > max)
  {
    return max;
  }
  else if (speed < min)
  {
    return min;
  }
  else
  {
    return speed;
  }
}
