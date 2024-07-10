#pragma once
#include "3DMath.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "Camera.h"

using namespace DirectX;

namespace Game { class Object; class Window; }

namespace Graphics
{
  class Device
  {
  public:
    Device(HWND hwnd);
    void ToggleFullscreen();
    void ObjectRender(Game::Object* object);
    bool GetFullscreen();
    void EndFrame();
    void ClearBuffer(float red, float green, float blue);
    void SetCamera(DirectX::XMMATRIX cam);
    void SetProjection(DirectX::XMMATRIX proj);
    XMMATRIX GetCamera() const;
    XMMATRIX GetProjection() const;
    Game::Window* wnd;
    XMMATRIX projection;
    XMMATRIX camera;
    XMFLOAT3 cameraPos;
    float x = 0;
    float y = 0;
    float z = -5;
    Microsoft::WRL::ComPtr<ID3D11Device>pDevice = nullptr;
  private:
    bool fullscreen = false;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pTextureWorldPixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pWorldPixelShader = nullptr;

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain>pSwap = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>pTarget = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>pStencilView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>pInputLayout = nullptr;
  };
}