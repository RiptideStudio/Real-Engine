#include "Engine.h"
#include "System.h"
#include "Settings.h"
#include "Object.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include  <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


bool Game::Engine::Init(Game::App* app)
{
  // Add systems
  Settings* settings = new Settings();
  AddSystem((System*)(settings));

  // Call system initializations
  for (auto system : systems)
  {
    system->Init();
    system->game = app;
  }
  return true;
}

void Game::Engine::Update(float dt)
{
  // Loop through systems and call each update
  for (auto system : systems)
  {
    system->Update(dt);
  }
}

void Game::Engine::Render()
{
  for (auto system : systems)
  {
    system->Render();
  }  
}

void Game::Engine::Exit()
{
  for (auto system : systems)
  {
    system->Exit();
  }
  systems.clear();

  for (auto object : objects)
  {
    delete object;
  }
  objects.clear();
}

// Add a new system to the list
void Game::Engine::AddSystem(System* system)
{
  systems.push_back(system);
}
