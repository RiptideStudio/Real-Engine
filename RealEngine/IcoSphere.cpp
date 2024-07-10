#include "IcoSphere.h"
#include <DirectXMath.h>

using namespace DirectX;

void IcoSphere::SubDivide(int recursionDepth)
{
    for (int r = 0; r < recursionDepth; r++)
    {
        // helper function to add a vertex to the mesh and return its index
        auto addVertex = [&](const DirectX::XMFLOAT3& pos) -> uint32_t
        {
            normalize(pos);
            vertices.push_back({ pos, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) });
            return static_cast<uint32_t>(vertices.size() - 1);
        };

        // helper function to calculate the midpoint of two vertices
        auto midpoint = [&](const Vertex& v1, const Vertex& v2) -> Vertex
        {
            DirectX::XMVECTOR pos1 = DirectX::XMLoadFloat3(&v1.pos);
            DirectX::XMVECTOR pos2 = DirectX::XMLoadFloat3(&v2.pos);
            DirectX::XMVECTOR midpoint = DirectX::XMVectorScale(DirectX::XMVectorAdd(pos1, pos2), 0.5f);

            Vertex result;
            DirectX::XMStoreFloat3(&result.pos, midpoint);
            return result;
        };

        // subdivide each triangle into four new triangles
        std::vector<unsigned short> newIndices;

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            Vertex v0 = vertices[indices[i]];
            Vertex v1 = vertices[indices[i + 1]];
            Vertex v2 = vertices[indices[i + 2]];

            Vertex v01 = midpoint(v0, v1);
            Vertex v12 = midpoint(v1, v2);
            Vertex v20 = midpoint(v2, v0);

            uint32_t i01 = addVertex(v01.pos);
            uint32_t i12 = addVertex(v12.pos);
            uint32_t i20 = addVertex(v20.pos);

            newIndices.push_back(indices[i]);
            newIndices.push_back(i01);
            newIndices.push_back(i20);

            newIndices.push_back(i01);
            newIndices.push_back(indices[i + 1]);
            newIndices.push_back(i12);

            newIndices.push_back(i20);
            newIndices.push_back(i12);
            newIndices.push_back(indices[i + 2]);

            newIndices.push_back(i01);
            newIndices.push_back(i12);
            newIndices.push_back(i20);
        }
        indices = newIndices;
    }

    ApplyLighting();
}
