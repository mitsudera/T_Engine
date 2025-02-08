#include "SkinMeshComputeShader.h"
#include "renderer.h"
#include "GameEngine.h"
#include "CBufferManager.h"
#include "SkinMeshPhongShader.h"

SkinMeshComputeShader::SkinMeshComputeShader(Renderer* renderer)
{
	this->pRenderer = renderer;
	this->pCBufferManager = renderer->GetGameEngine()->GetCBufferManager();



	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(XMMATRIX) * BONE_MAX;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pRenderer->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &boneBuffer);


	CreateCS("shaders/CSskinmesh.hlsl", "CSFunc");
}

SkinMeshComputeShader::~SkinMeshComputeShader()
{
}

void SkinMeshComputeShader::SetCS(void)
{
	ComputeShader::SetCS();
	pCBufferManager->SetCBufferCS(this->boneBuffer, (CBufferManager::BufferSlot)0);

}

void SkinMeshComputeShader::Destroy(void)
{
	ComputeShader::Destroy();

	if (boneBuffer) boneBuffer->Release();
}

void SkinMeshComputeShader::SetBoneBuffer(XMMATRIX* boneArray)
{
	pRenderer->GetDeviceContext()->UpdateSubresource(boneBuffer, 0, NULL, boneArray, 0, 0);
}
