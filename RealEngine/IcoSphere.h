#pragma once
#include "Model.h"
#include "Algorithms.h"
#include <cmath>

class IcoSphere : public Model
{
public:
    // Set the indices and vertices
    IcoSphere()
    {
        const float t = (1.0f + std::sqrt(5.0f)) / 4.0f;

        const DirectX::XMFLOAT3 positions[] =
        {
            {-1,  t,  0},
            { 1,  t,  0},
            {-1, -t,  0},
            { 1, -t,  0},
            { 0, -1,  t},
            { 0,  1,  t},
            { 0, -1, -t},
            { 0,  1, -t},
            { t,  0, -1},
            { t,  0,  1},
            {-t,  0, -1},
            {-t,  0,  1}
        };

        // Define the 20 faces of an icosahedron
        indices =
        {
            0, 11,  5,
            0,  5,  1,
            0,  1,  7,
            0,  7, 10,
            0, 10, 11,
            1,  5,  9,
            5, 11,  4,
            11, 10,  2,
            10,  7,  6,
            7,  1,  8,
            3,  9,  4,
            3,  4,  2,
            3,  2,  6,
            3,  6,  8,
            3,  8,  9,
            4,  5,  9,
            2,  4, 11, 
            6,  2, 10,
            8,  6,  7,
            9,  8,  1
        };

        // Assign the texture coordinates to the vertices
        std::vector<int> newIndices;
        for (int i = 0; i < 20; ++i)
        {
            vertices.push_back({ positions[indices[i * 3]], XMFLOAT3(0.0f, 0.0f, 0.0f) });
            vertices.push_back({ positions[indices[i * 3 + 1]], XMFLOAT3(0.0f, 0.0f, 0.0f) });
            vertices.push_back({ positions[indices[i * 3 + 2]], XMFLOAT3(0.0f, 0.0f, 1.0f) });
            newIndices.push_back(i * 3);
            newIndices.push_back(i * 3 + 2);
            newIndices.push_back(i * 3 + 1);
        }
        indices.insert(indices.end(), newIndices.begin(), newIndices.end());

        type = ModelType::sSpheric;

        ApplyLighting();
    }
    // Shader type
    // Subdivide the isosphere
    void SubDivide(int recursionDepth);
    // Update
    void Update(float dt) {}
};
