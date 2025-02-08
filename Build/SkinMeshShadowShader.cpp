#include "SkinMeshShadowShader.h"
#include "renderer.h"

SkinMeshShadowShader::SkinMeshShadowShader(Renderer* renderer)
{
    pRenderer = renderer;
    name = "SkinMeshShadowShader";

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONEWEIGHT",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONEINDEX",    0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

    };
    UINT numElements = ARRAYSIZE(layout);



    CreateCustomVS("shaders/SkinMeshShadowShader.hlsl", "VSmain", layout, numElements);
    CreatePS("shaders/SkinMeshShadowShader.hlsl", "PS_SM");

    shadowShaderIndex = ShadowShaderIndex::SkinMeshShadow;


    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.ByteWidth = sizeof(MaterialCBuffer);
    bufferDesc.StructureByteStride = sizeof(float);

    HRESULT hr = pRenderer->GetDevice()->CreateBuffer(&bufferDesc, NULL, &materialBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create buffer");
    }

}

SkinMeshShadowShader::~SkinMeshShadowShader()
{
    ShaderRelease();
    if (materialBuffer) materialBuffer->Release();

}

void SkinMeshShadowShader::SetMaterialCbuffer(MaterialCBuffer data)
{
    pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);

}
