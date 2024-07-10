#pragma once

class Vector3D
{
public:
  float x, y, z;
  Vector3D(float xx = 0, float yy = 0, float zz = 0);
  Vector3D operator+(const Vector3D& other);
  Vector3D operator-(const Vector3D& other);
  Vector3D operator*(const float other);
  void operator+=(const Vector3D& other);
  void operator-=(const Vector3D& other);
  void operator=(Vector3D vec);
};

float DotProduct(Vector3D vec, Vector3D vec2);
void VectorSet(Vector3D vec1, Vector3D vec2);
void VectorScale(Vector3D vec1, float mult);
