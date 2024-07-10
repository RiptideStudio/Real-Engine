//------------------------------------------------------------------------------
//
// File Name:	App.h
// Author(s): Callen Betts
// Course:		GAM 200
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Window.h"
#include "Timer.h"
#include "Engine.h"

// Game instances (apps) have a window and engine
// The window has a graphics instance, which handles rendering
// Only one game instance should be active at once
namespace Game
{
  class App
  {
  // Public functions
  public:
    App();
    int Update(float dt);
    bool Init();
    bool IsRunning() { return running; }
    void Render(float dt);
    void Exit();

    App& GetInstance() { return *this; }
    Timer* GetTimer() { return &timer; }
    Window& GetWindow() { return wnd; }
    Graphics::Device& Gfx() { return wnd.Gfx(); }
    Engine& GetEngine() { return engine; }
    HWND GetWindowHandle() { return wnd.GetHandle(); }
    Camera cam;
  // Private variables
  private:
    bool running = true;
    Window wnd;
    Engine engine;
    Timer timer;

  // Public variables
  public:
    int fps = 0;
  };
}