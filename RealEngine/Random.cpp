#include "Random.h"
#include <random>
#include <iostream>
#include <cstring>
#include <cstdarg>
#include <ctime>

float RandomRangeFloat(float min, float max)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

int RandomRange(int min, int max)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(min, max);
  return dis(gen);
}

