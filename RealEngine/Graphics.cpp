#include "Graphics.h"
#include "3DMath.h"
#include "Window.h"
#include "Object.h"
#include "Camera.h"
#include "Mouse.h"
#include "Transform.h"
#include "Texture.h"
#include <cmath>
#include <d3dcompiler.h>
#include  <string>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

//--- Constants ------------------------------------//
const UINT stride = sizeof(Vertex);
const UINT offset = 0u;
//--------------------------------------------------//

Graphics::Device::Device(HWND hwnd)
{
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 0;
  sd.BufferDesc.RefreshRate.Denominator = 0;
  sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = 1;
  sd.OutputWindow = hwnd;
  sd.Windowed = true;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = 0;
  
  D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    0,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &sd,
    &pSwap,
    &pDevice,
    nullptr,
    &pContext
  );

  // Get the backbuffer 
  Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
  pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
  pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);

  // Create a rasterizer state description
  D3D11_RASTERIZER_DESC rsDesc;
  ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
  rsDesc.FillMode = D3D11_FILL_SOLID;
  rsDesc.CullMode = D3D11_CULL_NONE;
  rsDesc.FrontCounterClockwise = true;
  rsDesc.DepthBias = 0;
  rsDesc.DepthBiasClamp = 0.0f;
  rsDesc.SlopeScaledDepthBias = 0.0f;
  rsDesc.DepthClipEnable = true;
  rsDesc.ScissorEnable = false;
  rsDesc.MultisampleEnable = false;
  rsDesc.AntialiasedLineEnable = false;

  // Set the rasterizer state in the graphics pipeline
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRSState;
  pDevice->CreateRasterizerState(&rsDesc, &pRSState);

  // Bind rasterizer state
  pContext->RSSetState(pRSState.Get());

  // Depth stencil
  D3D11_DEPTH_STENCIL_DESC sDesc;
  sDesc.DepthEnable = true;
  sDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  sDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pSState;
  pDevice->CreateDepthStencilState(&sDesc, &pSState);

  // Bind depth state
  pContext->OMSetDepthStencilState(pSState.Get(), 1u);

  // Depth stencil texture
  Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
  D3D11_TEXTURE2D_DESC descDepth = {};
  descDepth.Width = 1280u;
  descDepth.Height = 720u;
  descDepth.MipLevels = 1u;
  descDepth.ArraySize = 1u;
  descDepth.Format = DXGI_FORMAT_D32_FLOAT;
  descDepth.SampleDesc.Count = 1u;
  descDepth.SampleDesc.Quality = 0u;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

  // View of stencil texture
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
  descDSV.Format = DXGI_FORMAT_D32_FLOAT;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  descDSV.Texture2D.MipSlice = 0u;
  pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pStencilView);

  // Set topology (primitive type)
  pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Bind stencil view
  pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pStencilView.Get());

  // Shaders
  // World light shader
  D3DReadFileToBlob(L"PSPointLight.cso", &pBlob);
  pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
  
  D3DReadFileToBlob(L"PSWorldLight.cso", &pBlob);
  pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pWorldPixelShader);

  // Texture world light shader
  D3DReadFileToBlob(L"PSTexture.cso", &pBlob);
  pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pTextureWorldPixelShader);

  // Set vertex shaders
  D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
  pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

  // Create a sampler state
  D3D11_SAMPLER_DESC samplerDesc = {};
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  ComPtr<ID3D11SamplerState> sampler;
  pDevice->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());

  // Set the sampler state
  pContext->PSSetSamplers(0, 1, sampler.GetAddressOf());

  // Bind vertex shader
  pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

  // Input vertex layout
  const D3D11_INPUT_ELEMENT_DESC ied[] =
  {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };

  pDevice->CreateInputLayout(
    ied, (UINT)std::size(ied),
    pBlob->GetBufferPointer(),
    pBlob->GetBufferSize(),
    &pInputLayout
  );

  // Bind input
  pContext->IASetInputLayout(pInputLayout.Get());

  // Configure viewport
  D3D11_VIEWPORT vp = {};
  vp.Width = 1280;
  vp.Height = 720;
  vp.MinDepth = 0;
  vp.MaxDepth = 1;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  pContext->RSSetViewports(1u, &vp);
}

void Graphics::Device::ToggleFullscreen()
{
  if (fullscreen)
  {
    fullscreen = false;
    pSwap->SetFullscreenState(false, nullptr);
  }
  else
  {
    fullscreen = true;
    pSwap->SetFullscreenState(true, nullptr);
  }
}

void Graphics::Device::ObjectRender(Game::Object* object)
{
  namespace wrl = Microsoft::WRL;

  // Get the transform component
  Transform* transform = object->GetComponent<Transform>(object->components);
  Texture* texture = object->GetComponent<Texture>(object->components);
  Model* model = object->GetComponent<Model>(object->components);
  TransformBuffer* cb = &transform->transformMatrix;

  Vector3D position = transform->GetPosition();
  Vector3D scale = transform->GetScale();
  Vector3D angle = transform->GetAngle();
  
  XMMATRIX rotationMatrix = 
    XMMatrixRotationX(angle.x) *
    XMMatrixRotationY(angle.y) *
    XMMatrixRotationZ(angle.z);

    XMMATRIX transformMatrix = rotationMatrix * XMMatrixScaling(scale.x, scale.y, scale.z) *
    XMMatrixTranslation(position.x, position.y, position.z);

  //cb->model = XMMatrixTranspose(transformMatrix);
    float camOff = gameTime.Peek();

  cb->model = XMMatrixTranspose(transformMatrix);
  cb->modelViewProj = XMMatrixTranspose(transformMatrix*camera * projection);

  // Vertex loop
  float tileFactor = 0.5;
  for (auto& vertex : model->vertices)
  {
      DirectX::XMFLOAT3& pos = vertex.pos;
      float u = 0.0f, v = 0.0f;

      if (model->type == ModelType::sSpheric)
      {
          // Spherical coordinates
          float theta = std::atan2f(pos.z, pos.x);
          if (theta < 0.0f)
          {
              theta += 2.0f * XM_PI;
          }
          float phi = std::acosf(pos.y);
          u = theta / (2.0f * XM_PI) * scale.x;
          v = phi / XM_PI * scale.y;
          vertex.tex.coord = { u * tileFactor, v * tileFactor };
      }
      else if (model->type == ModelType::sCubic)
      {
          tileFactor = 0.25;
          // Bottom Face
          model->vertices[0].tex.coord = { 0.0f, 0.0f };
          model->vertices[1].tex.coord = { tileFactor * scale.z, 0.0f };
          model->vertices[2].tex.coord = { tileFactor * scale.z, tileFactor * scale.x };
          model->vertices[3].tex.coord = { 0.0f, tileFactor * scale.x };

          // Left face - Good
          model->vertices[4].tex.coord = { 0.0f, 0.0f };
          model->vertices[5].tex.coord = { tileFactor * scale.y, 0.0f };
          model->vertices[6].tex.coord = { tileFactor * scale.y, tileFactor * scale.z };
          model->vertices[7].tex.coord = { 0.0f, tileFactor * scale.z };

          // Top face
          model->vertices[8].tex.coord = { 0.0f, tileFactor * scale.x };
          model->vertices[9].tex.coord = { tileFactor * scale.z, tileFactor * scale.x };
          model->vertices[10].tex.coord = { tileFactor * scale.z, 0.0f };
          model->vertices[11].tex.coord = { 0.0f, 0.0f };

          // Right face - Good
          model->vertices[12].tex.coord = { 0.0f, 0.0f };
          model->vertices[13].tex.coord = { tileFactor * scale.x, 0.0f };
          model->vertices[14].tex.coord = { tileFactor * scale.x, tileFactor * scale.y };
          model->vertices[15].tex.coord = { 0.0f, tileFactor * scale.y };

          // Front face - Good
          model->vertices[16].tex.coord = { 0.0f, 0.0f };
          model->vertices[17].tex.coord = { tileFactor * scale.y, 0.0f };
          model->vertices[18].tex.coord = { tileFactor * scale.y, tileFactor * scale.z };
          model->vertices[19].tex.coord = { 0.0f, tileFactor * scale.z };

          // Back face - Good
          model->vertices[20].tex.coord = { tileFactor * scale.y, 0.0f };
          model->vertices[21].tex.coord = { tileFactor * scale.y, tileFactor * scale.z };
          model->vertices[22].tex.coord = { 0.0f, tileFactor * scale.z };
          model->vertices[23].tex.coord = { 0.0f, 0.0f };
      }
      else if (model->type == ModelType::sPlane)
      {
          float scaleAvg = 20;
          float tilingFactor = scaleAvg;
          model->vertices[0].tex.coord = { 0, tilingFactor };
          model->vertices[1].tex.coord = { 0, 0 };
          model->vertices[2].tex.coord = { tilingFactor, 0 };
          model->vertices[3].tex.coord = { tilingFactor, tilingFactor };
      }
  }


  // Set the model's shader
  switch (model->shaderType)
  {
  case ShaderType::sDirectionalLight:
      pContext->PSSetShader(pWorldPixelShader.Get(), nullptr, 0u);
      break;      
  case ShaderType::sPointLight:
      pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
      break;    
  case ShaderType::sTexture:
      pContext->PSSetShader(pTextureWorldPixelShader.Get(), nullptr, 0u);
      break;  
  }

  // Set the texture shader resource view
  if (texture)
  {
      ID3D11ShaderResourceView* srv = texture->GetResourceView();
      pContext->GenerateMips(srv);
      pContext->PSSetShaderResources(0, 1, &srv);
  }

  // Vertices of objects
  wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
  D3D11_BUFFER_DESC bd = {};
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.CPUAccessFlags = 0u;
  bd.MiscFlags = 0u;
  bd.ByteWidth = sizeof(Vertex) * model->vertices.size();
  bd.StructureByteStride = sizeof(Vertex);

  D3D11_SUBRESOURCE_DATA sd = {};
  sd.pSysMem = &(model->vertices[0]);
  pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
  pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

  // Transform matrix constant view buffer
  wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
  D3D11_BUFFER_DESC cbd;
  cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbd.Usage = D3D11_USAGE_DYNAMIC;
  cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cbd.MiscFlags = 0u;
  cbd.ByteWidth = sizeof(*cb);
  cbd.StructureByteStride = 0u;
  D3D11_SUBRESOURCE_DATA csd = {};
  csd.pSysMem = cb;
  pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

  // Bind constant buffer (Transform matrix)
  pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
  pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


  // Indices
  wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
  D3D11_BUFFER_DESC ibd = {};
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.Usage = D3D11_USAGE_DEFAULT;
  ibd.CPUAccessFlags = 0u;
  ibd.MiscFlags = 0u;
  ibd.ByteWidth = sizeof(short unsigned) * model->indices.size();
  ibd.StructureByteStride = sizeof(unsigned short);
  D3D11_SUBRESOURCE_DATA isd = {};
  isd.pSysMem = &(model->indices[0]);
  pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
  pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

  // Configure viewport
  D3D11_VIEWPORT vp = {};
  vp.Width = 1280;
  vp.Height = 720;
  vp.MinDepth = 0;
  vp.MaxDepth = 1;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  pContext->RSSetViewports(1u, &vp);


  // Draw
  pContext->DrawIndexed((UINT)(model->indices.size()), 0u, 0u);
}

void Graphics::Device::ClearBuffer(float red, float green, float blue)
{
  const float color[] = { red, green, blue, .1f };
  pContext->ClearRenderTargetView(pTarget.Get(), color);
  pContext->ClearDepthStencilView(pStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
}

void Graphics::Device::SetCamera(DirectX::XMMATRIX cam)
{
  camera = cam;
}

void Graphics::Device::SetProjection(DirectX::XMMATRIX proj)
{
  projection = proj;
}

XMMATRIX Graphics::Device::GetCamera() const
{
  return camera;
}

XMMATRIX Graphics::Device::GetProjection() const
{
  return projection;
}

bool Graphics::Device::GetFullscreen()
{
  return fullscreen;
}

void Graphics::Device::EndFrame()
{
  pSwap->Present(1u, 0u);
}
