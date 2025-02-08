#include "DebugLineShader.h"
#include "renderer.h"

DebugLineShader::DebugLineShader(Renderer* renderer)
{
	pRenderer = renderer;
	name = "DebugLineShader";

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

    };
    UINT numElements = ARRAYSIZE(layout);

    CreateCustomVS("shaders/DebugLineShader.hlsl", "VSmain", layout, numElements);
	CreatePS("shaders/DebugLineShader.hlsl", "PSmain");


}

DebugLineShader::~DebugLineShader()
{
}

