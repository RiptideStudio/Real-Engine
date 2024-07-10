#include "Mouse.h"
#include <Windows.h>

std::pair<int, int> Input::Mouse::GetPos() const
{
  return { x,y };
}

int Input::Mouse::GetPosX() const
{
  return x;
}

int Input::Mouse::GetPosY() const
{
  return y;
}

bool Input::Mouse::LeftIsPressed() const
{
  return leftPressed;
}

bool Input::Mouse::RightIsPressed() const
{
  return rightPressed;
}

Input::Mouse::Event Input::Mouse::Read()
{
  if (buffer.size() > 0u)
  {
    Input::Mouse::Event e = buffer.front();
    buffer.pop();
    return e;
  }
  else
  {
    return Input::Mouse::Event();
  }
}

void Input::Mouse::Flush()
{
  buffer = std::queue<Event>();
}

void Input::Mouse::OnMouseMove(int newx, int newy)
{
  // Calculate the delta mouse position
  x = newx;
  y = newy;
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::Move, *this));
  TrimBuffer();
}

void Input::Mouse::OnLeftPressed(int x, int y)
{
  leftPressed = true;
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::LPress,*this));
  TrimBuffer();
}

void Input::Mouse::OnLeftReleased(int x, int y)
{
  leftPressed = false;
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::LRelease, *this));
  TrimBuffer();
}

void Input::Mouse::OnRightPressed(int x, int y)
{
  rightPressed = true;
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::RPress, *this));
  TrimBuffer();
}

void Input::Mouse::OnRightReleased(int x, int y)
{
  rightPressed = false;
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::RRelease, *this));
  TrimBuffer();
}

void Input::Mouse::OnWheelUp(int x, int y)
{
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::WheelUp, *this));
  TrimBuffer();
}

void Input::Mouse::OnWheelDown(int x, int y)
{
  buffer.push(Input::Mouse::Event(Input::Mouse::Event::Type::WheelDown, *this));
  TrimBuffer();
}

void Input::Mouse::TrimBuffer()
{
  while (buffer.size() > bufferSize)
  {
    buffer.pop();
  }
}

// Move the cursor to the center of the screen so it is locked like an FPS game
// also used as an update for things like making cursor dissapear
void Input::Mouse::SetDeltaPos(POINT newPos, POINT oldPos)
{
  ShowCursor(showCursor);

  deltaMousePos.x = newPos.x - oldPos.x;
  deltaMousePos.y = newPos.y - oldPos.y;

  if (!showCursor)
  {
    POINT center = { 1280 / 2, 720 / 2 };
    SetCursorPos(center.x, center.y);
  }
}

void Input::Mouse::ToggleCursor()
{
  if (showCursor)
  {
    showCursor = false;
  }
  else
  {
    showCursor = true;
  }
}

Vector3D Input::Mouse::GetDeltaPos()
{
  return deltaMousePos;
}

Vector3D GetMousePos()
{
  POINT cursorPos;
  GetCursorPos(&cursorPos);
  return Vector3D(cursorPos.x, cursorPos.y, 0);
}

POINT GetMousePosDelta(const POINT& prevPos, const POINT& currentPos)
{
  POINT delta;
  delta.x = currentPos.x - prevPos.x;
  delta.y = currentPos.y - prevPos.y;
  return delta;
}
