#include "Window.h"
#include <Windows.h>
#include <sstream>

Game::Window::WindowClass Game::Window::WindowClass::wndClass;

Game::Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
  WNDCLASSEX wc = { 0 };
  wc.cbSize = sizeof(wc);
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = HandleMsgSetup;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetInstance();
  wc.hIcon = nullptr;
  wc.hCursor = nullptr;
  wc.hbrBackground = nullptr;
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = GetName();
  wc.hIconSm = nullptr;

  // Set the icon
  HICON icon = (HICON)LoadImage(wc.hInstance, "windowicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
  wc.hIcon = icon;

  RegisterClassEx(&wc);

}

Game::Window::WindowClass::~WindowClass()
{
  UnregisterClass(wndClassName, GetInstance());
}

const char* Game::Window::WindowClass::GetName() noexcept
{
  return wndClassName;
}

HINSTANCE Game::Window::WindowClass::GetInstance() noexcept
{
  return wndClass.hInst;
}

Game::Window::Window(int width, int height, const char* name) : width(1280), height(720)
{
  RECT wr = { 0 };
  wr.left = 100;
  wr.right = width + wr.left;
  wr.top = 100;
  wr.bottom = height + wr.top;
  AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZE | WS_SYSMENU, FALSE);
  hwnd = CreateWindow(
    WindowClass::GetName(), name,
    WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
    CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
    nullptr, nullptr, WindowClass::GetInstance(), this
  );
  ShowWindow(hwnd, SW_SHOWDEFAULT);
  // create graphics 
  pGfx = std::make_unique<Graphics::Device>(hwnd);
}

Game::Window::~Window()
{
  DestroyWindow(hwnd);
}

std::optional<int> Game::Window::ProcessMessages()
{
  MSG msg;

  while (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
  {
    if (msg.message == WM_QUIT)
    {
      return msg.wParam;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return {};
}

LRESULT Game::Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_NCCREATE)
  {
    const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
    Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Game::Window::HandleMsgThink));
    return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Game::Window::HandleMsgThink(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}

LRESULT Game::Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
  switch (msg)
  {
    //****************** KEYBOARD MESSAGE HANDLING ******************//
  case WM_KILLFOCUS:
    input.ClearState();
    mouse.Flush();
    break;

  case WM_CLOSE:
    // If we are in fullscreen, toggle off before closing
    PostQuitMessage(0);
    return 0;

  case WM_KEYDOWN:
    if (!(lParam & 0x40000000) || input.AutorepeatIsEnabled())
    {
      input.OnKeyPressed(static_cast<unsigned char>(wParam));
    }
    break;

  case WM_KEYUP:
    input.OnKeyReleased(static_cast<unsigned char>(wParam));
    break;

  case WM_CHAR:
    input.OnChar(static_cast<unsigned char>(wParam));
    break;

    //****************** MOUSE MESSAGE HANDLING ********************//
  case WM_MOUSEMOVE:
  {
    POINTS pt = MAKEPOINTS(lParam);
    mouse.OnMouseMove(pt.x, pt.y);
    break;
  }
  case WM_LBUTTONDOWN:
  {
    const POINTS pt = MAKEPOINTS(lParam);
    mouse.OnLeftPressed(pt.x, pt.y);
    break;
  }

  case WM_RBUTTONDOWN:
  {
    const POINTS pt = MAKEPOINTS(lParam);
    mouse.OnRightPressed(pt.x, pt.y);
    break;
  }  
  
  case WM_RBUTTONUP:
  {
    const POINTS pt = MAKEPOINTS(lParam);
    mouse.OnRightReleased(pt.x, pt.y);
    break;
  }  
  
  case WM_LBUTTONUP:
  {
    const POINTS pt = MAKEPOINTS(lParam);
    mouse.OnLeftReleased(pt.x, pt.y);
    break;
  }

  case WM_MOUSEHWHEEL:
  {
    const POINTS pt = MAKEPOINTS(lParam);
    if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
    {
      mouse.OnWheelUp(pt.x, pt.y);
    }
    if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
    {
      mouse.OnWheelDown(pt.x, pt.y);
    }
    break;
  }
  //****************** WINDOW HANDLING ********************//
  case WM_SIZE:
    if (wParam == SIZE_MINIMIZED)
    {
      // If the window is minimized
    }
    else if (wParam != SIZE_MAXIMIZED)
    {

    }
    break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND Game::Window::GetHandle()
{
  return hwnd;
}

void Game::Window::SetTitle(std::string title)
{
  SetWindowText(hwnd, title.c_str());
}

Graphics::Device& Game::Window::Gfx()
{
  return *pGfx;
}
