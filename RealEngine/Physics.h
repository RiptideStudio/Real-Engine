#pragma once
#include "Component.h"
#include "Vector.h"

class Physics : public Component
{
public:
  Physics();
  void Update(float dt);
  void SetVelocity(Vector3D vel) { velocity = vel; }
  Vector3D& GetVelocity() { return velocity; }
  void SetAcceleration(Vector3D acc) { acceleration = acc; }
  Vector3D& GetAcceleration() { return acceleration; }
private:
  Vector3D velocity;
  Vector3D acceleration;
  float speed = 10.f;
  float friction = 0.975f; // This value should be lower than 1
};