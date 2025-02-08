#include "QuadComponent.h"
#include "ShaderSet.h"
#include "renderer.h"
#include "gameobject.h"
#include "Material.h"
#include "transformcomponent.h"
#include "GameEngine.h"
#include "CBufferManager.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "CameraComponent.h"
#include "World.h"
QuadComponent::QuadComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}
void QuadComponent::Awake(void)
{
	PrimitiveComponent::Awake();

}

void QuadComponent::Init(void)
{
	PrimitiveComponent::Init();
	VERTEX_3D vertexArray[4];

	vertexArray[0].Position = { -0.5f,0.5f,0.0f };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };

	vertexArray[1].Position = { 0.5f,0.5f,0.0f };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { -0.5f,-0.5f,0.0f };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { 0.5f,-0.5f,0.0f };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if(!this->vertexBuffer) this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	pRenderer->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->pRenderer->GetDeviceContext()->Unmap(this->vertexBuffer, 0);

	this->material = pAssetsManager->GetMaterial("StandardMaterial");

}

void QuadComponent::Update(void)
{
	PrimitiveComponent::Update();

}

void QuadComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
	if (this->vertexBuffer)vertexBuffer->Release();
}

void QuadComponent::Draw(void)
{
	PrimitiveComponent::Draw();
	pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);
	XMMATRIX world = XMMatrixIdentity();


	if (isBillBorad)
	{
		pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_NONE);

		XMFLOAT3 scale = GetTransFormComponent()->GetScale();
		XMVECTOR opos = XMLoadFloat3(&GetWorldPos());
		XMMATRIX camW = pWorld->GetMainCamera()->GetWorldMtx();
		XMVECTOR camX = pWorld->GetMainCamera()->GetTransFormComponent()->GetAxisX();
		XMVECTOR camY = pWorld->GetMainCamera()->GetTransFormComponent()->GetAxisY();
		XMVECTOR camZ = pWorld->GetMainCamera()->GetTransFormComponent()->GetAxisZ();
		camX = XMVector3TransformNormal(camX, camW) * scale.x;
		camY = XMVector3TransformNormal(camY, camW) * scale.y;
		camZ = XMVector3TransformNormal(camZ, camW) * scale.z;


		XMVECTOR pos0 = opos + (camX * -0.5 + camY * 0.5) + camZ;
		XMVECTOR pos1 = opos + (camX * 0.5 + camY * 0.5) + camZ;
		XMVECTOR pos2 = opos + (camX * -0.5 + camY * -0.5) + camZ;
		XMVECTOR pos3 = opos + (camX * 0.5 + camY * -0.5) + camZ;


		VERTEX_3D vertexArray[4];

		XMStoreFloat3(&vertexArray[0].Position, pos0);
		vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		vertexArray[0].TexCoord = { 0.0f,0.0f };

		XMStoreFloat3(&vertexArray[1].Position, pos1);
		vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		vertexArray[1].TexCoord = { 1.0f,0.0f };

		XMStoreFloat3(&vertexArray[2].Position, pos2);
		vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		vertexArray[2].TexCoord = { 0.0f,1.0f };

		XMStoreFloat3(&vertexArray[3].Position, pos3);
		vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		vertexArray[3].TexCoord = { 1.0f,1.0f };

		//頂点バッファの中身を埋める

		D3D11_MAPPED_SUBRESOURCE msr;
		pRenderer->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

		this->pRenderer->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


	}
	else
	{
		world = GetWorldMtx();
	}


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);



	texture->SetShaderResourcePS(0);

	pRenderer->GetDeviceContext()->Draw(4, 0);

}

void QuadComponent::LoadTexture(string path)
{
	this->texture = pAssetsManager->LoadTexture(path);

}

void QuadComponent::SetBillBorad(BOOL b)
{
	isBillBorad = b;
}
