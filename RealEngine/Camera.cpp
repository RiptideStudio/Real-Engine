#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Window.h"
#include "Algorithms.h"
#include "Timer.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix()
{
  // Calculate the direction vector based on the camera's current rotation
  dx::XMVECTOR direction = dx::XMVector3Rotate(
    dx::XMVectorSet(0, 0, 1, 0), // forward vector
    dx::XMQuaternionRotationRollPitchYaw(-pitch, yaw, roll)
  );

  // Get the viewing direction of camera and save it
  dx::XMVECTOR camDirect = direction;
  camDirect = dx::XMVector3Normalize(camDirect);
  dx::XMStoreFloat3(&camDir, camDirect);

  // Normalize the direction vector
  direction = dx::XMVector3Normalize(direction);

  dx::XMVECTOR up = dx::XMVectorSet(0, 1, 0, 0);
  dx::XMVECTOR right = dx::XMVector3Normalize(dx::XMVector3Cross(direction, up));
  up = dx::XMVector3Normalize(dx::XMVector3Cross(right, direction));

  if (KeyDown('W'))
  {
    // Move the camera position in the direction of the camera
    camPosX += dx::XMVectorGetX(direction) * spd * gameTime.deltaTime;
    camPosZ += dx::XMVectorGetZ(direction) * spd * gameTime.deltaTime;
  }

  if (KeyDown('S'))
  {
    camPosX -= dx::XMVectorGetX(direction) * spd * gameTime.deltaTime;
    camPosZ -= dx::XMVectorGetZ(direction) * spd * gameTime.deltaTime;
  }
   
  if (KeyDown('D'))
  {
    // Calculate the direction vector based on the camera's current rotation
    dx::XMVECTOR direction = dx::XMVector3Rotate(
      dx::XMVectorSet(1, 0, 0, 0), // right vector
      dx::XMQuaternionRotationRollPitchYaw(0.0f, yaw, 0.0f)
    );

    // Normalize the direction vector
    direction = dx::XMVector3Normalize(direction);

    // Move the camera position in the direction of the camera
    camPosX += dx::XMVectorGetX(direction) * spd * gameTime.deltaTime;
    camPosZ += dx::XMVectorGetZ(direction) * spd * gameTime.deltaTime;
  }  
  
  if (KeyDown('A'))
  {
    // Calculate the direction vector based on the camera's current rotation
    dx::XMVECTOR direction = dx::XMVector3Rotate(
      dx::XMVectorSet(1, 0, 0, 0), // right vector
      dx::XMQuaternionRotationRollPitchYaw(0.0f, yaw, 0.0f)
    );

    // Normalize the direction vector
    direction = dx::XMVector3Normalize(direction);

    // Move the camera position in the direction of the camera
    camPosX -= dx::XMVectorGetX(direction) * spd * gameTime.deltaTime;
    camPosZ -= dx::XMVectorGetZ(direction) * spd * gameTime.deltaTime;
  }

  // Calculate the new camera position
  dx::XMVECTOR position = dx::XMVectorSet(camPosX, camPosY, camPosZ, 1.0f);

  // Calculate the view matrix
  const auto pos = dx::XMVector3Transform(
    position,
    dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0)
  );
  return dx::XMMatrixLookToLH(
    position, direction, up
  );
}

void Camera::Update(float dt)
{
  // Calculate the delta mouse position
  float dx = wnd->mouse.GetDeltaPos().x;
  float dy = wnd->mouse.GetDeltaPos().y;

  yaw += dx * (sensitivity/dt)   *dt;
  pitch -= dy * (sensitivity/dt) *dt;

  pitch = clamp(pitch, -1.25, 1.25);

  if (KeyDown(VK_SHIFT))
  {
    camPosY -= spd * dt;
  }
  if (KeyDown(VK_SPACE))
  {
    camPosY += spd * dt;
  }
  wnd->Gfx().cameraPos.x = camPosX;
  wnd->Gfx().cameraPos.y = camPosY;
  wnd->Gfx().cameraPos.z = camPosZ;

  if (KeyDown('C'))
  {
      wnd->Gfx().SetProjection(XMMatrixPerspectiveLH(1.0f, 720.f / 1280.f, fov*4, viewDistance*4));
  }
  else
  {
      wnd->Gfx().SetProjection(XMMatrixPerspectiveLH(1.0f, 720.f / 1280.f, fov, viewDistance));
  }

  wnd->Gfx().SetCamera(GetMatrix());
}
