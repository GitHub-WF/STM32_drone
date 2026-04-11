#ifndef __COM_PID_H__
#define __COM_PID_H__

#define PID_PERIOD 0.006 // PID计算周期，单位：秒

// 定义PID结构体
// kp,ki,kd需要再初始化设置好，目标值和测量值需要再计算时传递
typedef struct
{
  float kp; // 比例：值越大，响应越快，超调越严重
  float ki; // 积分：解决稳态误差，处理稳态误差 = 无人机积分一般不使用
  float kd; // 微分：值越大，抑制超调越明显
  float err; // 当前误差 = 目标值 - 测量值
  float desire; // 目标值
  float measure; // 测量值
  float err_last; // 上一次误差值
  float integral; // 积分累计
  float output; // 输出值
} PID_Struct;

// 定义PID变量
extern PID_Struct out_pid_pitch;
extern PID_Struct in_pid_pitch;
extern PID_Struct out_pid_roll;
extern PID_Struct in_pid_roll;
extern PID_Struct out_pid_yaw;
extern PID_Struct in_pid_yaw;

extern PID_Struct out_pid_height;

// 单次PID计算
void Common_PID_Calc(PID_Struct *pid);

// 串级PID计算
void Common_PID_Calc_chain(PID_Struct *out_pid, PID_Struct *in_pid);

// 限制电机转速
float Com_Limit(float speed, float max, float min);

#endif /* __COM_PID_H__ */
