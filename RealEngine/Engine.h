#pragma once
#include "System.h"
#include <vector>
#include <memory>

namespace Game
{
  class Engine
  {
  private:
    std::vector<System*> systems;
  public:
    void Update(float dt);
    void Render();
    bool Init(Game::App* app);
    void Exit();
    void AddSystem(System* system);
  };
}

