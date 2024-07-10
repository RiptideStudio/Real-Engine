#include "Settings.h"
#include "App.h"
#include "Object.h"
#include <string>

void Settings::Update(float dt)
{
  int fps = game->fps;
  Timer* timer = &gameTime;

  if (timer->totalTime > 1.0f)
  {
    game->fps = (int)(timer->frameCount / timer->totalTime);
    timer->frameCount = 0;
    timer->totalTime = 0.0f;
  }

  // Exit
  if (KeyTriggered(VK_ESCAPE))
  {
    game->Exit();
  }

  // Debug check
  if (game->GetWindow().mouse.LeftIsPressed())
  {
    float width = 8;
    //Game::Object o = CreateObject(Vector3D(RandomRangeFloat(-width, width), RandomRangeFloat(-width, width), RandomRangeFloat(1, width * 2)));
  }


  std::string fpsText = "Frame Rate: " + std::to_string(static_cast<int>(game->fps)) + " fps, " + "Objects: " + std::to_string(objects.size());
  SetWindowText(game->GetWindowHandle(), fpsText.c_str());

  // Fullscreen toggle
  if (KeyTriggered(VK_TAB))
  {
    game->Gfx().ToggleFullscreen();
  }

  // Temporary restart code
  if (KeyTriggered('R'))
  {
    game->GetEngine().Exit();
    game->Init();
  }
}

