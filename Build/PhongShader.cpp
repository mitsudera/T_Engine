#include "PhongShader.h"
#include"renderer.h"

PhongShader::PhongShader(Renderer* renderer)
{
    pRenderer = renderer;
    this->CreateVS("shaders/PhongShader.hlsl", "VSmain");
    this->CreatePS("shaders/PhongShader.hlsl", "PSmain");

    shaderIndex = ShaderIndex::Phong;

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

PhongShader::~PhongShader()
{
    ShaderRelease();
    if (materialBuffer) materialBuffer->Release();
}

void PhongShader::SetMaterialCbuffer(MaterialCBuffer data)
{
    pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);

}
