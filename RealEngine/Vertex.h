#pragma once
#include "Vector.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT3 n;

    struct
    {
        float r, g, b, a;
    } col;

    struct
    {
        XMFLOAT2 coord;
    } tex;
};

struct Face
{
    unsigned short v1, v2, v3;
};