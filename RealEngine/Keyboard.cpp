#include "Keyboard.h"
#include <Windows.h>

// Returns if a key is held down
bool KeyDown(int virtualKeyCode)
{
  SHORT keyState = GetAsyncKeyState(virtualKeyCode);
  return (keyState & 0x8000) != 0;
}

// Returns if a key was pressed once
bool KeyTriggered(int virtualKeyCode)
{
  SHORT keyState = GetAsyncKeyState(virtualKeyCode);

  bool isPressed = (keyState & 0x8000) != 0;

  static bool previousState[256] = { false }; 
  bool wasTriggered = !previousState[virtualKeyCode] && isPressed;

  previousState[virtualKeyCode] = isPressed;

  return wasTriggered;
}

void Input::Keyboard::OnKeyPressed(unsigned char key)
{
  if (!keystates[key])
  {
    keyTriggers[key] = true;
  }

  keystates[key] = true;
  keybuffer.push(Input::Keyboard::Event(Input::Keyboard::Event::Type::Press, key));
  TrimBuffer(keybuffer);
}

template<typename T>
void Input::Keyboard::TrimBuffer(std::queue<T>& buffer)
{
  while (buffer.size() > bufferSize)
  {
    buffer.pop();
  }
}

void Input::Keyboard::OnKeyReleased(unsigned char key)
{
  keystates[key] = false;
  keybuffer.push(Input::Keyboard::Event(Input::Keyboard::Event::Type::Release, key));
  TrimBuffer(keybuffer);
}

void Input::Keyboard::OnChar(char character)
{
  charbuffer.push(character);
  TrimBuffer(charbuffer);
}

void Input::Keyboard::ClearState()
{
  keystates.reset();
}

bool Input::Keyboard::KeyDown(unsigned char key)
{
  return keystates[key];
}

bool Input::Keyboard::KeyTriggered(unsigned char key)
{
  return keyTriggers[key];
}

Input::Keyboard::Event Input::Keyboard::ReadKey()
{
  if (keybuffer.size() > 0u)
  {
    Input::Keyboard::Event e = keybuffer.front();
    keybuffer.pop();
    return e;
  }
  else
  {
    return Input::Keyboard::Event();
  }
}

bool Input::Keyboard::KeyIsEmpty()
{
  return keybuffer.empty();
}

char Input::Keyboard::ReadChar()
{
  if (charbuffer.size() > 0u)
  {
    unsigned char charcode = charbuffer.front();
    charbuffer.pop();
    return charcode;
  }
  else
  {
    return 0;
  }
}

bool Input::Keyboard::CharIsEmpty()
{
  return charbuffer.empty();
}

void Input::Keyboard::FlushKey()
{
  keybuffer = std::queue<Event>();
}

void Input::Keyboard::FlushChar()
{
  charbuffer = std::queue<char>();
}

void Input::Keyboard::Flush()
{
  FlushKey();
  FlushChar();
}

void Input::Keyboard::ResetKeyTriggers()
{
  keyTriggers.reset();
}

void Input::Keyboard::EnableAutorepeat()
{
  autoRepeatEnabled = true;
}

void Input::Keyboard::DisableAutorepeat()
{
  autoRepeatEnabled = false;
}

bool Input::Keyboard::AutorepeatIsEnabled()
{
  return autoRepeatEnabled;
}
