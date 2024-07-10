#pragma once
#include "Model.h"

class Cylinder : public Model
{
public:
    // Constructor
    Cylinder()
    {
        const float radius = 0.5f;        // radius of the cylinder
        const float height = 10.0f;        // height of the cylinder
        const int numSegments = 5;       // number of segments (slices) in the cylinder

        // Generate vertices
        const float angleStep = DirectX::XM_2PI / numSegments;
        for (int i = 0; i <= numSegments; i++)
        {
            float x = radius * cos(angleStep * i);
            float z = radius * sin(angleStep * i);

            // Top cap vertex
            vertices.push_back({ DirectX::XMFLOAT3(x, height / 2.0f, z), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) });

            // Bottom cap vertex
            vertices.push_back({ DirectX::XMFLOAT3(x, -height / 2.0f, z), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) });
        }

        // Generate indices for the sides
        for (int i = 0; i < numSegments; i++)
        {
            DirectX::XMFLOAT3 v0(vertices[i * 2].pos);
            DirectX::XMFLOAT3 v1(vertices[i * 2 + 1].pos);
            DirectX::XMFLOAT3 v2(vertices[i * 2 + 3].pos);

            DirectX::XMFLOAT3 normal1, normal2;
            CalculateTriangleNormals(v0, v1, v2, normal1, normal2);

            vertices[i * 2].n = normal1;
            vertices[i * 2 + 1].n = normal1;
            vertices[i * 2 + 3].n = normal2;

            indices.push_back(i * 2);
            indices.push_back(i * 2 + 1);
            indices.push_back(i * 2 + 3);

            indices.push_back(i * 2);
            indices.push_back(i * 2 + 3);
            indices.push_back(i * 2 + 2);
        }


        // Generate indices for the top and bottom caps
        for (int i = 0; i < numSegments; i++)
        {
            // Top cap
            indices.push_back(i * 2);
            indices.push_back((i + 1) * 2);
            indices.push_back(vertices.size() / 2 - 1);

            // Bottom cap
            indices.push_back((i + 1) * 2 + 1);
            indices.push_back(vertices.size() / 2);
            indices.push_back(i * 2 + 1);
        }

        ApplyLighting();
        shaderType = ShaderType::sTexture;
    }
    // Update
    void Update(float dt) {}
};
