#include "Int_display.h"

extern Remote_Data remote_data;

#define LINE1_BEGIN 28
#define LINE2_BEGIN 5

#define LINE3_BEGIN 5
#define BAR1_BEGIN 35
#define BAR2_BEGIN 47

#define LINE3_BEGIN2 65
#define BAR1_BEGIN_2 95
#define BAR2_BEGIN_2 107

#define LINE4_BEGIN 5
#define BAR1_BEGIN 35
#define BAR2_BEGIN 47

#define LINE4_BEGIN2 65
#define BAR1_BEGIN_2 95
#define BAR2_BEGIN_2 107

#define Y0 0
#define Y1 14
#define Y2 26
#define Y3 38

void App_display_show_bar(uint8_t x, uint8_t y, uint8_t count)
{
  if (count < 13)
  {
    OLED_Show_CH(x, y, 12 + count, 12, 1);
  }
}

/**
 * @brief 初始化OLED
 *
 */
void Int_OLED_Init(void)
{
  OLED_Init();
}

/**
 * @brief 刷新屏幕
 *
 */
void App_OLED_refresh(void)
{
  // 标题
  for (uint8_t i = 0; i < 6; i++)
  {
    OLED_Show_CH(LINE1_BEGIN + 12 * i, Y0, i, 12, 1);
  }

  // 2.4g通讯信道
  uint8_t buff[4] = {0};
  sprintf((char *)buff, "%03d", RF_CH);
  OLED_ShowString(LINE2_BEGIN, Y1, buff, 12, 1);

  // 展示遥控数据 thr roll
  uint8_t count = 0;
  OLED_Show_CH_String(LINE3_BEGIN, Y2, "THR: ", 12, 1);
  if (remote_data.throttle > 500)
  {
    count = (remote_data.throttle - 500) / 41;
    App_display_show_bar(BAR1_BEGIN, Y2, 12);
    App_display_show_bar(BAR2_BEGIN, Y2, count); 
  }
  else
  {
    count = remote_data.throttle / 41;
    App_display_show_bar(BAR2_BEGIN, Y2, count); 
    App_display_show_bar(BAR2_BEGIN, Y2, 0); 
  }

  OLED_Show_CH_String(LINE3_BEGIN2, Y2, "ROL: ", 12, 1);
  if (remote_data.roll > 500)
  {
    count = (remote_data.roll - 500) / 41;
    App_display_show_bar(BAR1_BEGIN_2, Y2, 12);
    App_display_show_bar(BAR2_BEGIN_2, Y2, count); 
  }
  else
  {
    count = remote_data.roll / 41;
    App_display_show_bar(BAR2_BEGIN_2, Y2, count); 
    App_display_show_bar(BAR2_BEGIN_2, Y2, 0); 
  }

  // 展示 yaw pitch
  OLED_Show_CH_String(LINE4_BEGIN, Y3, "YAW: ", 12, 1);
  if (remote_data.yaw > 500)
  {
    count = (remote_data.yaw - 500) / 41;
    App_display_show_bar(BAR1_BEGIN, Y3, 12);
    App_display_show_bar(BAR2_BEGIN, Y3, count); 
  }
  else
  {
    count = remote_data.yaw / 41;
    App_display_show_bar(BAR2_BEGIN, Y3, count); 
    App_display_show_bar(BAR2_BEGIN, Y3, 0); 
  }

  OLED_Show_CH_String(LINE4_BEGIN2, Y3, "PIT: ", 12, 1);
  if (remote_data.pitch > 500)
  {
    count = (remote_data.pitch - 500) / 41;
    App_display_show_bar(BAR1_BEGIN_2, Y3, 12);
    App_display_show_bar(BAR2_BEGIN_2, Y3, count); 
  }
  else
  {
    count = remote_data.pitch / 41;
    App_display_show_bar(BAR2_BEGIN_2, Y3, count); 
    App_display_show_bar(BAR2_BEGIN_2, Y3, 0); 
  }
  
  OLED_Refresh_Gram();
}
