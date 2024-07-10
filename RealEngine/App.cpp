//------------------------------------------------------------------------------
//
// File Name:	App.cpp
// Author(s): Callen Betts
// Course:		GAM 200
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "App.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Random.h"
#include "Player.h"
#include "Object.h"
#include "Cube.h"
#include "Sphere.h"
#include "IcoSphere.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Physics.h"
#include <sstream>
#include "Algorithms.h"
#include <iomanip>

// Creates a new window
Game::App::App() : wnd(1280, 720, "BIB Window")
{

}

// Initializes the game, run once when window is launched
bool Game::App::Init()
{
  engine.Init(this);

  // Give the graphics and camera component a pointer to the window
  wnd.Gfx().wnd = &wnd;
  cam.wnd = &wnd;

  // Fun stuff, level design!
  float width = 150;
  int treeNum = RandomRange(75, 100);

  CreateTree({ 0,15,0 },&wnd.Gfx(),1.5f);

  Object plane = CreateObject(Vector3D(0,5,0), new Plane(), &wnd.Gfx(), L"grass.png");
  plane.SetScale(Vector3D(width*2, 1, width*2));
  plane.SetAngle(ToRad(90.f));
  plane.SetColor(0.2, 0.2, 0.2);

  Object pillar = CreateObject({ 20,50,0 },new Cube(), & wnd.Gfx(), L"rock.png");
  pillar.SetScale({ 10,100,10 });
  pillar.SetColor(0.3f, 0.3f, 0.4f);

  for (int i = 0; i < treeNum; i++)
  {
    // Trees & Rocks
    float leaveSize = RandomRangeFloat(6.f, 7.f);
    float stoneSize = RandomRangeFloat(1.5f, 3.f);
    float stoneSize2 = RandomRangeFloat(1.5f, 3.f);
    float stoneSize3 = RandomRangeFloat(1.5f, 3.f);

    Object tree = CreateObject(Vector3D(RandomRangeFloat(-width, width), 10, RandomRangeFloat(-width, width)), new Cube(), &wnd.Gfx(), L"wood.png");
    tree.SetScale(Vector3D(1,10,1));
    tree.SetColor(0.4, 0.25, 0.25);

    IcoSphere* mod = new IcoSphere();
    mod->shaderType = ShaderType::sTexture;
    mod->SubDivide(1);
    Object leaves = CreateObject(Vector3D(tree.GetPosition().x, tree.GetPosition().y+8, tree.GetPosition().z), mod, &wnd.Gfx(), L"leaves.png");

    IcoSphere* rock = new IcoSphere();
    if (RandomRange(0,5) == 1) rock->SubDivide(1);

    rock->shaderType = ShaderType::sTexture;
    Object stone = CreateObject(Vector3D(RandomRangeFloat(-width, width), 5, RandomRangeFloat(-width, width)), rock, &wnd.Gfx(), L"rock.png");
    stone.SetScale(Vector3D(stoneSize, stoneSize2, stoneSize3));
    if (RandomRange(0,3) == 1) stone.SetScale(Vector3D(stoneSize*8, stoneSize2 * 8, stoneSize3 * 8));

    stone.SetAngle(ToRad(RandomRangeFloat(0,360)));
    stone.SetColor(0.3f, 0.3f, 0.4f);

    leaves.SetVelocity({ 0,0,0 });
    leaves.SetAngle({RandomRangeFloat(0,360),RandomRangeFloat(0,360),RandomRangeFloat(0,360)});
    leaves.SetScale(Vector3D(leaveSize, leaveSize, leaveSize));
    leaves.SetColor(0.3f, 0.45f, 0.2f);
    leaves.SetVelocity({ 0.f,0.f,0.f });
  }
  // Sun
  Object sun = CreateObject(Vector3D(-33, 100, -100), new Sphere());
  sun.SetScale(Vector3D(10.f, 10.f, 10.f));
  sun.SetColor(1.f, 0.7f, 0.3f);

  return true;
}

// Renders all systems and objects
void Game::App::Render(float dt)
{
  // Clear frame
  wnd.Gfx().ClearBuffer(0.2,0.2,0.3);

  // Render all objects
  for (auto object : objects)
  {
    object->Render(wnd.Gfx());
  }

  // Render systems
  engine.Render();

  // End the frame (draw this current finished frame)
  wnd.Gfx().EndFrame();
}

// Updates all systems and objects, and returns windows encode message which handles
//  windows callbacks
// Params: None
// Returns: Windows wParam, 0 (false)
int Game::App::Update(float dt)
{
  // Processes all windows callbacks
  if (const auto encode = Window::ProcessMessages())
  {
    running = false;
    return *encode; // Return encode for quit is 0
  }

  // Test shooting
  XMFLOAT3 p = wnd.Gfx().cameraPos;
  XMFLOAT3 dir = cam.camDir;

  if (KeyDown('Q'))
  {
      Object o = CreateObject(Vector3D(p.x,p.y,p.z), new Cube());
      o.SetVelocity({ dir.x * 10,dir.y * 10,dir.z *10});
      o.SetColor(RandomRangeFloat(0, 1), RandomRangeFloat(0, 1), RandomRangeFloat(0, 1));
  }

  // Update engine 
  engine.Update(dt);
  cam.Update(dt);

  // Update objects
  for (auto object : objects)
  {
    object->Update(dt);
  }

  // Reset input triggers & flush
  wnd.input.ResetKeyTriggers();
  return 0;
}

// Unloads all game resources
void Game::App::Exit()
{
  running = false;
  ClipCursor(NULL);
  ShowCursor(true);
}

