#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <Windows.h>
#include <optional>
#include <memory>

namespace Game
{
  class Window
  {
  private:
    class WindowClass
    {
    public:
      static const char* GetName() noexcept;
      static HINSTANCE GetInstance() noexcept;
    private:
      WindowClass() noexcept;
      ~WindowClass();
      WindowClass(const WindowClass&) = delete;
      WindowClass& operator=(const WindowClass&) = delete;
      static constexpr const char* wndClassName = "BIB Engine Window";
      static WindowClass wndClass;
      HINSTANCE hInst;
    };

  public:
    Window(int width, int height, const char* name);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    static std::optional<int>ProcessMessages();
  private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMsgThink(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lParam) noexcept;
    int width;
    int height;
    std::string title;
    HWND hwnd;
    std::unique_ptr<Graphics::Device> pGfx;
  public:
    HWND GetHandle();
    void SetTitle(std::string title);
    Graphics::Device& Gfx();
    Input::Keyboard input;
    Input::Mouse mouse;
  };
}