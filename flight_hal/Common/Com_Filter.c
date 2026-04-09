#include "Com_Filter.h"

// 一阶低通滤波：对精度要求不高的情况，但是计算要快，值越小, 过滤效果越明显
#define ALPHA 0.15 /* 一阶低通滤波 指数加权系数 */
/**
 * @description: 一阶低通滤波
 *  是一种常用的滤波器，用于去除高频噪声或高频成分，保留信号中的低频成分。
 *  在单片机应用中，一种简单且常见的低通滤波器是一阶无限脉冲响应（IIR）低通滤波器，
 *  通常实现为指数加权移动平均滤波器。
 * @param {int16_t} newValue 需要滤波的值
 * @param {int16_t} preFilteredValue 上一次滤波过的值
 * @return {*}
 */
int16_t Com_Filter_LowPass(int16_t newValue, int16_t preFilteredValue)
{
    return ALPHA * newValue + (1 - ALPHA) * preFilteredValue;
}

// 对于波动比较大的情况，使用卡尔曼滤波
// 均值和期望：求平均值
// 方差和标准差：一组数据在平均值附近波动的幅度，方差越大，波动越强
// 准度：计算输出值和真实值越接近
// 精度：计算输出值统计方差低，波动小

// 卡尔曼滤波实现：
// 1. 初始化：
//    P_kx = x_k
//    P_k = Q_k
// 2. 迭代更新：
//    P_kx = P_kx + Q_k
//    P_k = P_kx / (P_kx + R_k)
//    x_k = x_k + P_k * (z_k - x_k)

// 卡尔曼滤波实现：
// (初始化)记录一组初始化状态值
// (测量)获取当前一次的加速度=>方差(平均值)
// (使用初始化的K值)得到当前一次的输出值 =>记录结果的方差
// (下一次测量之前)更新状态=>重新计算K增益系数
// (测量)获取下一次的加速度=>方差(平均值)
// (计算下一次的输出值)得到下一次的输出值=>记录结果的方差
// 循环...


/* 卡尔曼滤波 https://www.mwrf.net/tech/basic/2023/30081.html */
/* 卡尔曼滤波参数 */
KalmanFilter_Struct kfs[3] = {
    {0.02, 0, 0, 0, 0.001, 0.543},
    {0.02, 0, 0, 0, 0.001, 0.543},
    {0.02, 0, 0, 0, 0.001, 0.543}};
double Com_Filter_KalmanFilter(KalmanFilter_Struct *kf, double input)
{
    kf->Now_P = kf->LastP + kf->Q;
    kf->Kg    = kf->Now_P / (kf->Now_P + kf->R);
    kf->out   = kf->out + kf->Kg * (input - kf->out);
    kf->LastP = (1 - kf->Kg) * kf->Now_P;
    return kf->out;
}
