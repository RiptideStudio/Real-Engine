#pragma once
#include <chrono>

class Timer
{
public:
  Timer();
  float DeltaTime() { return deltaTime; }
  float Peek() const;
  void Tick();
public:
  std::chrono::steady_clock::time_point last;
  float deltaTime = 0;
  int frameCount = 0;
  float totalTime = 0;
};

extern Timer gameTime;