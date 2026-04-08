#include "Com_utils.h"

/**
 * @brief 限制最小最大值
 * 
 * @param value 
 * @param min 
 * @param max 
 */
void Com_utils_limit(int16_t *value, int16_t min, int16_t max)
{
  if (*value < min)
  {
    *value = min;
  }
  else if (*value > max)
  {
    *value = max;
  }
}
