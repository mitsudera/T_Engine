#include "SpriteComponent.h"
#include "renderer.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "UIMaterial.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "AssetsManager.h"
#include "Scene.h"
#include "CBufferManager.h"

SpriteComponent::SpriteComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Awake(void)
{
	PrimitiveComponent::Awake();
	CreateVertexBuffer();
	this->material = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->GetMaterial("UIMaterial");
	texSlice.x = 1;
	texSlice.y = 1;
	texLRrev = FALSE;
	TypeName = typeid(SpriteComponent).name();;

}

void SpriteComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void SpriteComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
	this->vertexBuffer->Release();
}

void SpriteComponent::Draw(void)
{

	PrimitiveComponent::Draw();



	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


	pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	texture->SetShaderResourcePS(0);


	pRenderer->GetDeviceContext()->Draw(4, 0);

}

void SpriteComponent::CreateVertexBuffer(void)
{

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);

}

void SpriteComponent::SetSpriteCenter(string texPath, XMFLOAT3 pos, float width, float height)
{
	this->texture = pAssetsManager->LoadTexture(texPath);

	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
	float z = pos.z;

	float w = (width / screenHW.x) * 2.0f;
	float h = (height / screenHW.y) * 2.0f;
	float l = (pos.x / screenHW.x) - w * 0.5f;
	float t = (pos.y / screenHW.y) - h * 0.5f;





	vertexArray[0].Position = { l,t + h,z };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { l + w,t + h,z };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { l,t ,z };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { l + w ,t,z };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };





}

void SpriteComponent::SetSpriteLeftDown(string texPath, XMFLOAT3 pos, float width, float height)
{
	this->texture = pAssetsManager->LoadTexture(texPath);


	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
	float z = pos.z;

	float w = (width / screenHW.x) * 2.0f;
	float h = (height / screenHW.y) * 2.0f;
	float l = ((pos.x / screenHW.x) - 0.5f) * 2.0f;
	float t = ((pos.y / screenHW.y) - 0.5f) * 2.0f;





	vertexArray[0].Position = { l,t+h,z };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { l + w,t+h,z };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { l,t ,z };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { l+w ,t,z };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };


}
void SpriteComponent::SetSpriteCenter(XMFLOAT3 pos, float width, float height)
{

	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
	float z = pos.z;

	float w = (width / screenHW.x) * 2.0f;
	float h = (height / screenHW.y) * 2.0f;
	float l = (pos.x / screenHW.x) - w * 0.5f;
	float t = (pos.y / screenHW.y) - h * 0.5f;





	vertexArray[0].Position = { l,t + h,z };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { l + w,t + h,z };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { l,t ,z };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { l + w ,t,z };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };





}

void SpriteComponent::SetSpriteLeftDown(XMFLOAT3 pos, float width, float height)
{


	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
	float z = pos.z;

	float w = (width / screenHW.x) * 2.0f;
	float h = (height / screenHW.y) * 2.0f;
	float l = ((pos.x / screenHW.x) - 0.5f) * 2.0f;
	float t = ((pos.y / screenHW.y) - 0.5f) * 2.0f;





	vertexArray[0].Position = { l,t+h,z };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { l + w,t+h,z };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { l,t ,z };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { l+w ,t,z };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };


}

void SpriteComponent::SetFullScreen(string texPath)
{
	this->texture = pAssetsManager->LoadTexture(texPath);



	vertexArray[0].Position = { -1,1,0 };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { 1,1,0};
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { -1,-1 ,0 };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { 1 ,-1,0 };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

}

void SpriteComponent::SetTexSlice(XMINT2 slice)
{
	texSlice = slice;
	sliceMax = slice.x * slice.y;
}

void SpriteComponent::SetSliceTexIndex(int index)
{

	this->sliceIndex = index%sliceMax;
	

	XMINT2 pos;

	pos.x = index % texSlice.x;
	pos.y = index / texSlice.y;
	
	XMFLOAT2 uv;
	XMFLOAT2 uvScale;
	uvScale.x = 1.0f / (float)texSlice.x;
	uvScale.y = 1.0f / (float)texSlice.y;

	uv.x = uvScale.x * (float)pos.x;
	uv.y = uvScale.y * (float)pos.y;

	if (texLRrev)
	{
		vertexArray[1].TexCoord = { uv.x,uv.y };
		vertexArray[0].TexCoord = { uv.x + uvScale.x,uv.y };
		vertexArray[3].TexCoord = { uv.x,uv.y + uvScale.y };
		vertexArray[2].TexCoord = { uv.x + uvScale.x,uv.y + uvScale.y };

	}
	else
	{
		vertexArray[0].TexCoord = { uv.x,uv.y };
		vertexArray[1].TexCoord = { uv.x + uvScale.x,uv.y };
		vertexArray[2].TexCoord = { uv.x,uv.y + uvScale.y };
		vertexArray[3].TexCoord = { uv.x + uvScale.x,uv.y + uvScale.y };

	}




}

void SpriteComponent::SetColor(XMFLOAT4 diffuse)
{
	vertexArray[0].Diffuse = diffuse;
	vertexArray[1].Diffuse = diffuse;
	vertexArray[2].Diffuse = diffuse;
	vertexArray[3].Diffuse = diffuse;


}

void SpriteComponent::SetTexture(string path)
{
	this->texture = pAssetsManager->LoadTexture(path);

}

void SpriteComponent::SetLRRev(BOOL enable)
{
	texLRrev = enable;
}

