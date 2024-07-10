#pragma once
#include "System.h"
#include "App.h"

class Settings : public Game::System
{
public:
  void Update(float dt);
  void Render() const {}
  void Exit() {}
  void Init() {}
  void Unload() {}
};