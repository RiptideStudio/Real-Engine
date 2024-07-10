#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class TransformBuffer
{
public:
  XMMATRIX model;
  XMMATRIX modelViewProj;
  XMMATRIX projection;
  XMMATRIX view;
};

struct Color
{
  float r;
  float g;
  float b;
  float a;
};
