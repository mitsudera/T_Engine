#pragma once
#include "Coreminimal.h"

class Renderer;

class FullScreenQuadVertex
{
public:
    FullScreenQuadVertex(Renderer* renderer);
    ~FullScreenQuadVertex();
    void Draw(void);

private:
    Renderer* pRenderer;
    ID3D11VertexShader* vs;
    ID3D11Buffer* vertexBuffer;
    ID3D11InputLayout* VertexLayout;

};

