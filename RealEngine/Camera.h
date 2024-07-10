#pragma once
#include "Graphics.h"
#include "Vector.h"

namespace Game { class Window; }

// Camera class definition
class Camera
{
public:
  DirectX::XMMATRIX GetMatrix();
  void Update(float dt);
  XMFLOAT3 camDir = { 0,0,0 };

  Game::Window* wnd = nullptr;
private:
  float r = 20.f;
  float theta = 0.0f;
  float phi = 0.0f;
  float yaw = 0.0f;
  float roll = 0.0f;
  float pitch = 0.f;

  float sensitivity = 0.0025f;
  float fov = 0.5f;
  float spd = 22.1f;

  
  float viewDistance = 200.f;
  float maxCameraSpeed = 25;
  float camPosX = 0;
  float camPosY = 10;
  float camPosZ = 0;
};

