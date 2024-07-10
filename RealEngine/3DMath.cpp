#include "3DMath.h"

typedef class Vector3D Vec3;
//************* 3D VECTORS ****************//
Vector3D::Vector3D(float xx, float yy, float zz)
  :
  x(xx),
  y(yy),
  z(zz)
{}

Vector3D Vector3D::operator-(const Vector3D& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

Vector3D Vector3D::operator+(const Vector3D& other)
{
  Vector3D vec = {this->x,this->y,this->z};
  vec.x += other.x;
  vec.y += other.y;
  vec.z += other.z;
  return vec;
}

Vector3D Vector3D::operator*(const float other)
{
  x *= other;
  y *= other;
  z *= other;
  return *this;
}

void Vector3D::operator+=(const Vector3D& other)
{
  x += other.x;
  y += other.y;
  z += other.z;
}

void Vector3D::operator-=(const Vector3D& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
}

void Vector3D::operator=(Vector3D vec)
{
  x = vec.x;
  y = vec.y;
  z = vec.z;
}

float DotProduct(Vector3D vec, Vector3D vec2)
{
  float dotP = 0;
  dotP += vec.x * vec.x;
  dotP += vec.y * vec.y;
  dotP += vec.z * vec.z;
  return dotP;
}

void VectorSet(Vector3D vec1, Vector3D vec2)
{
  vec1.x = vec2.x;
  vec1.y = vec2.y;
  vec1.z = vec2.z;
}

void VectorScale(Vector3D vec1, float mult)
{
  vec1.x *= mult;
  vec1.y *= mult;
  vec1.z *= mult;
}

