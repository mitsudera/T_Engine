#include "SkinMeshPhongShader.h"
#include"renderer.h"
#include "CBufferManager.h"
#include "GameEngine.h"
SkinMeshPhongShader::SkinMeshPhongShader(Renderer* renderer)
{
    pRenderer = renderer;
    pCBufferManager = renderer->GetGameEngine()->GetCBufferManager();
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

    this->CreateCustomVS("shaders/SkinMeshPhongShader.hlsl", "VSmain", layout, numElements);
    this->CreatePS("shaders/SkinMeshPhongShader.hlsl", "PSmain");

    shaderIndex = ShaderIndex::SkinMeshPhong;

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



    bufferDesc = {};

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(XMMATRIX) * BONE_MAX;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    pRenderer->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &boneBuffer);

    pCBufferManager->SetCBufferVS(this->boneBuffer, CBufferManager::BufferSlot::Bone);



}

SkinMeshPhongShader::~SkinMeshPhongShader()
{
    ShaderRelease();
    if (materialBuffer) materialBuffer->Release();
}

void SkinMeshPhongShader::Uninit(void)
{
    ShaderSet::Uninit();
    if (boneBuffer) boneBuffer->Release();
}

void SkinMeshPhongShader::SetMaterialCbuffer(MaterialCBuffer data)
{
    pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);

}

void SkinMeshPhongShader::SetBoneBuffer(XMMATRIX* bone)
{
    pRenderer->GetDeviceContext()->UpdateSubresource(this->boneBuffer, 0, nullptr, bone, 0, 0);

}
