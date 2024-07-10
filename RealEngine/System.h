#pragma once

namespace Game
{
  class App;

  class System
  {
  public:
    System() {}
    virtual void Update(float dt) = 0;
    virtual void Render() const = 0;
    virtual void Init() = 0;
    virtual void Exit() = 0;
    virtual void Unload() = 0;
    Game::App* game = nullptr;
  };
}

