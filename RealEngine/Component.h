#pragma once

namespace Game { class Object; }

class Component
{
private:
  enum ComponentType
  {
    sInvalid = -1,
    sTransform,
    sPhysics,
    sMesh
  };
public:
  virtual void Update(float dt) = 0;
  Game::Object* GetParent() { return parent; }
  void SetParent(Game::Object* newParent) { parent = newParent; }
protected:
  Game::Object* parent = nullptr;
};
