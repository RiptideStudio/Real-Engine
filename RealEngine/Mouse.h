#pragma once
#include <queue>
#include "Vector.h"
#include <Windows.h>

namespace Input
{
  class Mouse
  {
    friend class Window;
  public:
    class Event
    {
    public:
      enum class Type
      {
        LPress,
        LRelease,
        RPress,
        RRelease,
        WheelUp,
        WheelDown,
        Move,
        Invalid
      };
    private:
      Type type;
      int x;
      int y;
      bool leftPressed = false;
      bool rightPressed = false;
    public:
      Event()
        :
        type(Type::Invalid),
        leftPressed(false),
        rightPressed(false),
        x(0),
        y(0)
      {}
      Event(Type type, const Mouse& parent)
        :
        type(type),
        leftPressed(parent.leftPressed),
        rightPressed(parent.rightPressed),
        x(parent.x),
        y(parent.y)
      {}
      bool IsValid() const
      {
        return type != Type::Invalid;
      }
      Type GetType() const
      {
        return type;
      }
      std::pair<int, int> GetPos() const
      {
        return { x,y };
      }
      int GetPosX() const
      {
        return x;
      }
      int GetPosY() const
      {
        return y;
      }
      bool LeftIsPressed() const
      {
        return leftPressed;
      }
      bool RightIsPressed() const
      {
        return rightPressed;
      }
    };
  public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator = (const Mouse&) = delete;
    std::pair<int, int> GetPos() const;
    int GetPosX() const;
    int GetPosY() const;
    bool LeftIsPressed() const;
    bool RightIsPressed() const;
    Mouse::Event Read();
    bool IsEmpty() const
    {
      return buffer.empty();
    }
    void Flush();
    void OnMouseMove(int x, int y);
    void OnLeftPressed(int x, int y);
    void OnLeftReleased(int x, int y);
    void OnRightPressed(int x, int y);
    void OnRightReleased(int x, int y);
    void OnWheelUp(int x, int y);
    void OnWheelDown(int x, int y);
    void TrimBuffer();
    void SetDeltaPos(POINT newPos, POINT oldPos);
    void ToggleCursor();
    bool CursorIsVisibile() { return showCursor; }
    Vector3D GetDeltaPos();
    Vector3D deltaMousePos;
  private:
    static constexpr unsigned int bufferSize = 16u;
    std::queue<Event> buffer;
    int x;
    int y;
    bool leftPressed = false;
    bool rightPressed = false;
    bool showCursor = false;
  };
}

Vector3D GetMousePos();

