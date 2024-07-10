#pragma once
#include "Component.h"
#include "TransformBuffer.h"
#include "3DMath.h"
#include "Algorithms.h"
#include <vector>

class Transform : public Component
{
public:
  // Virtual overrides
  void Update(float dt);

  // Methods
  Transform();
  void SetPosition(Vector3D pos) { position = pos; }
  Vector3D& GetPosition() { return position; }    
  void SetOffset(Vector3D off) { offset = off; }
  Vector3D& GetOffset() { return offset; }  
  void SetOldPosition(Vector3D oldPos) { oldPosition = oldPos; }
  Vector3D GetOldPosition() { return oldPosition; }
  void SetScale(Vector3D newScale) { scale = newScale; }
  Vector3D& GetScale() { return scale; }
  void SetAngle(Vector3D ang) { angle = ang; }
  Vector3D& GetAngle() { return  angle; } 
  void SetDirection(Vector3D dir) { direction = dir; }
  Vector3D& GetDirection() { return direction; }
private:
  bool isDirty = true;
  Vector3D position;
  Vector3D angle = { ToRad(90), 0, 0 };
  Vector3D oldPosition;
  Vector3D direction;
  Vector3D offset; // This is the offset from a composite object
  Vector3D scale = { 1,1,1 };
public:
  // Transform matrix
  TransformBuffer transformMatrix;
};

