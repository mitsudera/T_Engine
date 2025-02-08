#include "StandardShader.h"
#include "renderer.h"

StandardShader::StandardShader(Renderer* renderer)
{
	pRenderer = renderer;
	name = "StandardShader";
	CreateVS("shaders/StandardShader.hlsl", "VSmain");
	CreatePS("shaders/StandardShader.hlsl", "PSmain");

    shaderIndex = ShaderIndex::Standard;
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

StandardShader::~StandardShader()
{
	ShaderRelease();
    if (materialBuffer) materialBuffer->Release();
}

void StandardShader::SetMaterialCbuffer(StandardShader::MaterialCBuffer data)
{
	pRenderer->GetDeviceContext()->UpdateSubresource(this->materialBuffer, 0, nullptr, &data, 0, 0);
}
