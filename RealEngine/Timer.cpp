#include "Timer.h"
using namespace std::chrono;

Timer gameTime;

Timer::Timer()
{
  last = steady_clock::now();
}

float Timer::Peek() const
{
  return duration<float>(steady_clock::now() - last).count();
}

void Timer::Tick()
{
  const auto now = std::chrono::steady_clock::now();
  const std::chrono::duration<float> frameTime = now - last;
  deltaTime = frameTime.count();
  last = now;
  frameCount++;
  totalTime += deltaTime;
}
