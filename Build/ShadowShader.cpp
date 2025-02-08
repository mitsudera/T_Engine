#include "ShadowShader.h"
#include "renderer.h"

ShadowShader::ShadowShader(Renderer* renderer)
{
    pRenderer = renderer;
    name = "ShadowShader";
    CreateVS("shaders/ShadowShader.hlsl", "VS_SM");
    CreatePS("shaders/ShadowShader.hlsl", "PS_SM");

    shadowShaderIndex = ShadowShaderIndex::StandardShadow;


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

ShadowShader::~ShadowShader()
{
    ShaderRelease();
    if (materialBuffer) materialBuffer->Release();

}

void ShadowShader::SetMaterialCbuffer(MaterialCBuffer data)
{
    pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);

}
