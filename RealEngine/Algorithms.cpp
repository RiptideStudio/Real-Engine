#include "Algorithms.h"

float clamp(float value, float minVal, float maxVal)
{
  if (value < minVal) {
    return minVal;
  }
  else if (value > maxVal) {
    return maxVal;
  }
  else {
    return value;
  }
}

int sign(float val)
{
  if (val < 0) return -1;
  if (val > 0) return 1;
  return 0;
}

float ToRad(float deg)
{
  return (myPi / 180.f) * deg;
}
