#include "Model.h"
#include "Transform.h"
#include "Object.h"
#include <cmath>

void Model::Update(float dt)
{

}

void Model::ApplyLighting()
{
    // Normalize the vertices to make them lie on the unit sphere
    for (auto& vertex : vertices)
    {
        DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.pos);
        pos = DirectX::XMVector3Normalize(pos);
        DirectX::XMStoreFloat3(&vertex.pos, pos);
    }

    // Calculate the center 
    DirectX::XMFLOAT3 center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    for (const auto& vertex : vertices) {
        center.x += vertex.pos.x;
        center.y += vertex.pos.y;
        center.z += vertex.pos.z;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();

    for (int i = 0; i < indices.size(); i += 3)
    {
        // Get the vertex indices for this face
        unsigned short i0 = indices[i];
        unsigned short i1 = indices[i + 1];
        unsigned short i2 = indices[i + 2];

        // Calculate the edges of this face
        DirectX::XMFLOAT3 edge0 = subtract(vertices[i2].pos, vertices[i0].pos);
        DirectX::XMFLOAT3 edge1 = subtract(vertices[i1].pos, vertices[i0].pos);

        // Calculate the face normal
        DirectX::XMFLOAT3 faceNormal = normalize(cross(edge1, edge0));

        // Check if the normal is facing outward
        DirectX::XMFLOAT3 vecToVertex = subtract(vertices[i0].pos, center);
        if (dot(faceNormal, vecToVertex) < 0.0f)
        {
            faceNormal = multiply(-1, faceNormal);
        }

        // Add the face normal to the normals of the vertices
        vertices[i0].n = faceNormal;
        vertices[i1].n = faceNormal;
        vertices[i2].n = faceNormal;
    }
}
