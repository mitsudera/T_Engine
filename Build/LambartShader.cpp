#include "LambartShader.h"
#include "renderer.h"

LambartShader::LambartShader(Renderer* renderer)
{
	pRenderer = renderer;
	name = "LambartShader";
	CreateVS("shaders/LambartShader.hlsl", "VSmain");
	CreatePS("shaders/LambartShader.hlsl", "PSmain");

    shaderIndex = ShaderIndex::Lambart;
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

LambartShader::~LambartShader()
{
	ShaderRelease();
    if (materialBuffer) materialBuffer->Release();
}

void LambartShader::SetMaterialCbuffer(LambartShader::MaterialCBuffer data)
{
	pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);
}
