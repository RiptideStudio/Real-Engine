#pragma once
#include "Vertex.h"
#include "TransformBuffer.h"
#include "Component.h"
#include "3DMath.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

enum class ShaderType
{
    sTexture,
    sDirectionalLight,
    sPointLight
};

enum class ModelType
{
    sInvalid = -1,
    sCubic,
    sSpheric,
    sPlane,
};

class Model : public Component
{
public:
  // Calculate face normals
  XMFLOAT3 normalize(XMFLOAT3 vec)
  {
    float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    vec.x /= length;
    vec.y /= length;
    vec.z /= length;
    return vec;
  }
  XMFLOAT3 subtract(const XMFLOAT3& a, const XMFLOAT3& b)
  {
    return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
  }
  XMFLOAT3 cross(const XMFLOAT3& a, const XMFLOAT3& b)
  {
    return XMFLOAT3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
  }
  XMFLOAT3 divide(XMFLOAT3 v, float s)
  {
      return XMFLOAT3(v.x / s, v.y / s, v.z / s);
  }
  XMFLOAT3 add(const XMFLOAT3& a, const XMFLOAT3& b)
  {
      return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
  }

  float dot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
  {
      DirectX::XMVECTOR dotProduct = DirectX::XMVector3Dot(
          DirectX::XMLoadFloat3(&v1),
          DirectX::XMLoadFloat3(&v2)
      );
      float result;
      DirectX::XMStoreFloat(&result, dotProduct);
      return result;
  }

  DirectX::XMFLOAT3 multiply(float s, const DirectX::XMFLOAT3& v)
  {
      DirectX::XMVECTOR scalar = DirectX::XMVectorReplicate(s);
      DirectX::XMVECTOR vector = DirectX::XMLoadFloat3(&v);
      DirectX::XMVECTOR product = DirectX::XMVectorMultiply(scalar, vector);
      DirectX::XMFLOAT3 result;
      DirectX::XMStoreFloat3(&result, product);
      return result;
  }
  void CalculateTriangleNormals(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, DirectX::XMFLOAT3& normal1, DirectX::XMFLOAT3& normal2)
  {
      // Calculate the two triangle normals
      DirectX::XMVECTOR vec1 = DirectX::XMLoadFloat3(&v1) - DirectX::XMLoadFloat3(&v0);
      DirectX::XMVECTOR vec2 = DirectX::XMLoadFloat3(&v2) - DirectX::XMLoadFloat3(&v0);
      DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vec2, vec1));

      DirectX::XMFLOAT3 normalFloat3;
      DirectX::XMStoreFloat3(&normalFloat3, normal);

      normal1 = normalFloat3;
      normal2 = normalFloat3;
  }
  // Model type
  ModelType type = ModelType::sInvalid;
  // Shader type
  ShaderType shaderType = ShaderType::sDirectionalLight;
  // Update
  void Update(float dt);
  // Vertices
  std::vector<Vertex>vertices = {};
  // Faces
  std::vector<Face>faces = {};
  // Indices
  std::vector<unsigned short>indices = {};
  // Face colors
  std::vector<Color>colors;
  // Apply lighting faces
  void ApplyLighting();
};
