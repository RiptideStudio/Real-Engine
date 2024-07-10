#pragma once
#include "Model.h"

class Plane : public Model  
{
public:
  Plane()
  {
    // Vertices
    vertices.resize(4);
    float side = 1.f / 2.f;
    vertices[0].pos = { -side,-side,0.f };
    vertices[1].pos = { -side,side,0.f };
    vertices[2].pos = { side,side,0.f };
    vertices[3].pos = { side,-side,0.f };

    vertices[0].n = { 0.f, 0.f, 1.f };
    vertices[1].n = { 0.f, 0.f, 1.f };
    vertices[2].n = { 0.f, 0.f, 1.f };
    vertices[3].n = { 0.f, 0.f, 1.f };    

    // Indices
    indices = 
    {
        0, 1, 2,
        2, 3, 0,     
    };
    type = ModelType::sPlane;
    shaderType = ShaderType::sTexture;
  }
  // Get type
  void Update(float dt) {}
};
