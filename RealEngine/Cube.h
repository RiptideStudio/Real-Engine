#pragma once
#include "Model.h"
#include "Transform.h"
#include <cmath>

class Cube : public Model
{
public:
  // Constructor
  Cube()
  {
    // Vertices
    vertices.resize(24);
    float side = 1.5f / 2.f;

    // Front face
    vertices[0].pos = { -side,-side, side };
    vertices[1].pos = { side,-side, side };
    vertices[2].pos = { side, side, side };
    vertices[3].pos = { -side, side, side };

    // Left face
    vertices[4].pos = { -side,-side,-side };
    vertices[5].pos = { -side,-side, side };
    vertices[6].pos = { -side, side, side };
    vertices[7].pos = { -side, side,-side };

    // Back face
    vertices[8].pos = { -side,-side,-side };
    vertices[9].pos = { -side, side,-side };
    vertices[10].pos = { side, side,-side };
    vertices[11].pos = { side,-side,-side };

    // Right face
    vertices[12].pos = { side,-side,-side };
    vertices[13].pos = { side, side,-side };
    vertices[14].pos = { side, side, side };
    vertices[15].pos = { side,-side, side };

    // Top face
    vertices[16].pos = { -side, side,-side };
    vertices[17].pos = { -side, side, side };
    vertices[18].pos = { side, side, side };
    vertices[19].pos = { side, side,-side };

    // Bottom face
    vertices[20].pos = { -side,-side,-side };
    vertices[21].pos = { side,-side,-side };
    vertices[22].pos = { side,-side, side };
    vertices[23].pos = { -side,-side, side };

    // Front face
    vertices[0].tex.coord = { 0.0f, 0.0f };
    vertices[1].tex.coord = { 1.0f, 0.0f };
    vertices[2].tex.coord = { 1.0f, 1.0f };
    vertices[3].tex.coord = { 0.0f, 1.0f };

    // Left face
    vertices[4].tex.coord = { 0.0f, 0.0f };
    vertices[5].tex.coord = { 1.0f, 0.0f };
    vertices[6].tex.coord = { 1.0f, 1.0f };
    vertices[7].tex.coord = { 0.0f, 1.0f };

    // Back face
    vertices[8].tex.coord = { 0.0f, 0.0f };
    vertices[9].tex.coord = { 1.0f, 0.0f };
    vertices[10].tex.coord = { 1.0f, 1.0f };
    vertices[11].tex.coord = { 0.0f, 1.0f };

    // Right face
    vertices[12].tex.coord = { 0.0f, 0.0f };
    vertices[13].tex.coord = { 1.0f, 0.0f };
    vertices[14].tex.coord = { 1.0f, 1.0f };
    vertices[15].tex.coord = { 0.0f, 1.0f };

    // Top face
    vertices[16].tex.coord = { 0.0f, 0.0f };
    vertices[17].tex.coord = { 1.0f, 0.0f };
    vertices[18].tex.coord = { 1.0f, 1.0f };
    vertices[19].tex.coord = { 0.0f, 1.0f };

    // Bottom face
    vertices[20].tex.coord = { 0.0f, 0.0f };
    vertices[21].tex.coord = { 1.0f, 0.0f };
    vertices[22].tex.coord = { 1.0f, 1.0f };
    vertices[23].tex.coord = { 0.0f, 1.0f };

    indices = 
    {
      0, 1, 2,
      0, 2, 3,
      4, 5, 6,
      4, 6, 7,
      8, 9, 10,
      8, 10, 11,
      12, 13, 14,
      12, 14, 15,
      16, 17, 18,
      16, 18, 19,
      20, 21, 22,
      20, 22, 23
    };

    ApplyLighting();
    type = ModelType::sCubic;
    // Apply texturing and lighting
    shaderType = ShaderType::sTexture;

  }
  // Update
  void Update(float dt) {}
};
