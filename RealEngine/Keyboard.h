#pragma once

#include <queue>
#include <bitset>

namespace Input
{
  class Keyboard
  {
    friend class Window;

  public:
    class Event
    {
    public:
      enum class Type
      {
        Press,
        Release,
        Invalid
      };
    private:
      Type type;
      unsigned char code;
    public:
      Event() : type(Type::Invalid), code(0u) {}
      Event(Type type, unsigned char code) : type(type), code(code) {}
      bool IsPress()
      {
        return type == Type::Press;
      }
      bool IsRelease()
      {
        return type == Type::Release;
      }
      bool IsValid()
      {
        return type != Type::Invalid;
      }
      unsigned char GetCode()
      {
        return code;
      }
    };

  public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator =(const Keyboard&) = delete;

    bool KeyDown(unsigned char key);
    Event ReadKey();
    bool KeyIsEmpty();
    void FlushKey();
    char ReadChar();
    bool CharIsEmpty();
    void FlushChar();
    void Flush();
    bool KeyTriggered(unsigned char key);
    void ResetKeyTriggers();

    void EnableAutorepeat();
    void DisableAutorepeat();
    bool AutorepeatIsEnabled();
    void OnKeyPressed(unsigned char key);
    void OnKeyReleased(unsigned char key);
    void OnChar(char character);
    void ClearState();
    template<typename T>
    void TrimBuffer(std::queue<T>& buffer);

  private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autoRepeatEnabled = false;
    std::bitset<nKeys> keystates;
    std::bitset<nKeys> keyTriggers;
    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;
  };
}

// Global functions for getting keyboard Keyboard
// NOTE: These are potentially inaccurate, due to using asynchronous updates

// If a key is being held down
bool KeyDown(int virtualKeyCode);

// If a key was triggered
bool KeyTriggered(int virtualKeyCode);