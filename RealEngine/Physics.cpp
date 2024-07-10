#include "Physics.h"
#include "Object.h"
#include "Algorithms.h"
#include "Random.h"
#include "Keyboard.h"
#include "Transform.h"

Physics::Physics()
{
  velocity = { 0,0,0 };
  float spd = 8;
  //acceleration = { RandomRangeFloat(-spd,spd),RandomRangeFloat(-spd,spd),RandomRangeFloat(-spd,spd) };
  acceleration = { 0, 0, 0 };
}

void Physics::Update(float dt)
{
  // Apply friction to our objects based on acceleration
  velocity = ((acceleration * friction) * speed) * dt;

  // Test simulation
  if (KeyDown('G'))
  {
    float spd = 0.5;
    acceleration += { RandomRangeFloat(-spd,spd),RandomRangeFloat(-spd,spd),RandomRangeFloat(-spd,spd) };

  }

  // Move the object
  Transform* transform = parent->GetComponent<Transform>(parent->components);

  if (transform)
  {
    // Set old position for collision resolution
    transform->SetOldPosition(transform->GetPosition());

    // Set the current position
    transform->GetPosition() += velocity;
  }
}
