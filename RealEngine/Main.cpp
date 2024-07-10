//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s): Callen Betts
// Course:		GAM 200
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BIB.h"
#include "App.h"
#include "Engine.h"
#include "Window.h"
#include "Mouse.h"
#include <Windows.h>
#include <string>
#include <sstream>

// Static variables
POINT cursorPos;    // Old position
POINT newCursorPos; // New position

// Main windows callback for running a window - hInstance contains the exec data of the window
//  hPrevInstance should be NULL in win32, determines if another window is running
//  lpCmdLine contains the parameters the window was launched with
// 
// Params: hInstance, hPrevInstance, lpCmdLine, nCmdShow
// Returns: Integer-value
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  // Initialize a new game
  static Game::App game;

  if (!game.Init()) return 0;

  Timer* timer = game.GetTimer();

  // Main game loop
  while (game.IsRunning())
  {
    // Update delta times and tick timer
    gameTime.Tick();

    GetCursorPos(&cursorPos);

    // Update game
    game.Update(gameTime.deltaTime);

    // Render game
    game.Render(gameTime.deltaTime);

    // Calculate mouse delta
    GetCursorPos(&newCursorPos);
    game.GetWindow().mouse.SetDeltaPos(newCursorPos, cursorPos);
  }

  // Exit
  game.Exit();
}
