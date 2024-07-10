#pragma once
#include "Model.h"

class Sphere : public Model
{
public:
  // Set the indices and vertices
  Sphere()
  {
    const float radius = 1;
    const int numSubdivisions = 40;

    for (int lat = 0; lat <= numSubdivisions; ++lat)
    {
      float theta = lat * XM_PI / numSubdivisions;
      float sinTheta = sinf(theta);
      float cosTheta = cosf(theta);

      for (int lon = 0; lon <= numSubdivisions; ++lon)
      {
        float phi = lon * 2 * XM_PI / numSubdivisions;
        float sinPhi = sinf(phi);
        float cosPhi = cosf(phi);

        Vertex vertex;
        vertex.pos.x = radius * cosPhi * sinTheta;
        vertex.pos.y = radius * cosTheta;
        vertex.pos.z = radius * sinPhi * sinTheta;

        vertices.push_back(vertex);
      }
    }

    // Generate indices
    for (int lat = 0; lat < numSubdivisions; ++lat)
    {
      for (int lon = 0; lon < numSubdivisions; ++lon)
      {
        unsigned short v0 = lat * (numSubdivisions + 1) + lon;
        unsigned short v1 = v0 + 1;
        unsigned short v2 = (lat + 1) * (numSubdivisions + 1) + lon;
        unsigned short v3 = v2 + 1;

        indices.push_back(v0);
        indices.push_back(v1);
        indices.push_back(v2);

        indices.push_back(v1);
        indices.push_back(v3);
        indices.push_back(v2);
      }
    }

    // Calculate vertex normals
    for (size_t i = 0; i < vertices.size(); i++)
    {
        XMFLOAT3 vertexNormal(0.0f, 0.0f, 0.0f);

        // Find all faces that share this vertex
        for (size_t j = 0; j < indices.size(); j += 3)
        {
            if (indices[j] == i || indices[j + 1] == i || indices[j + 2] == i)
            {
                // This face shares this vertex
                XMFLOAT3 faceNormal = vertices[indices[j]].n;
                XMStoreFloat3(&faceNormal, XMVector3Normalize(XMLoadFloat3(&faceNormal)));

                vertexNormal.x += faceNormal.x;
                vertexNormal.y += faceNormal.y;
                vertexNormal.z += faceNormal.z;
            }
        }

        XMStoreFloat3(&vertexNormal, XMVector3Normalize(XMLoadFloat3(&vertexNormal)));
        vertices[i].n = vertexNormal;
    }
  }
  // Update
  void Update(float dt) {}
};
