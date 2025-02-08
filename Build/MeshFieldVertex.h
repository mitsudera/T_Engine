#pragma once
#include "Coreminimal.h"

class Renderer;

class MeshFieldVertex
{

public:
    MeshFieldVertex(Renderer* renderer);
    ~MeshFieldVertex();
    void SetVertexBuffer(void);

private:
    Renderer* pRenderer;
    ID3D11Buffer* vertexBuffer;

};

