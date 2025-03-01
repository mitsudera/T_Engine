#pragma once
#include "MeshComponent.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "DX11Texture.h"
#include "gameobject.h"
#include "CBufferManager.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "transformcomponent.h"

#define TEXTURE_PATH	"data/TEXTURE/"


MeshComponent::MeshComponent(GameObject* gameObject)
{
	cullMode = CULL_MODE::CULL_MODE_FRONT;
	this->alphaTest = FALSE;

	pGameObject = gameObject;


}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Awake(void)
{
	PrimitiveComponent::Awake();

	attribute = Attribute::Primitive;

	cullMode = CULL_MODE::CULL_MODE_BACK;
	this->alphaTest = FALSE;
	TypeName = typeid(MeshComponent).name();;


	hasShadow = TRUE;


}

void MeshComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void MeshComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();



}


void MeshComponent::Draw(void)
{
	PrimitiveComponent::Draw();





	if (this->alphaTest==TRUE)
	{
		this->pRenderer->SetAlphaTestEnable(TRUE);
	}
	


	this->pRenderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();



	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	this->material->SetBufferMaterial();

	this->pRenderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	this->pRenderer->SetAlphaTestEnable(FALSE);

}

void MeshComponent::ShadowMapping(void)
{
	PrimitiveComponent::ShadowMapping();





	if (this->alphaTest == TRUE)
	{
		this->pRenderer->SetAlphaTestEnable(TRUE);
	}



	this->pRenderer->SetCullingMode((CULL_MODE)cullMode);
	meshData->BufferSetVertex();
	meshData->BufferSetIndex();



	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);


	this->shadowMaterial->SetBufferMaterial();

	this->pRenderer->GetDeviceContext()->DrawIndexed(meshData->GetIndexNum(), 0, 0);

	this->pRenderer->SetAlphaTestEnable(FALSE);

}



void MeshComponent::SetMeshData(MeshData* data)
{



	meshData = data;

	this->material = meshData->GetMaterial();
	this->shadowMaterial = meshData->GetShadowMaterial();

	this->GetTransFormComponent()->SetLocalMtx(meshData->GetLocalOffset());

	this->GetTransFormComponent()->UpdateMatrix();

	SetBoxMinMax(data->GetBoxMin(), data->GetBoxMax());
}

void MeshComponent::SetMeshData(string path, unsigned int index)
{
	string fileName = path;
	// 最後の '/' の位置を見つける
	size_t pos = fileName.rfind('/');

	// 最後の '/' が見つかった場合
	if (pos != string::npos) {
		// '/' より後の部分を切り出す
		fileName = fileName.substr(pos + 1);
	}

	this->meshData = pAssetsManager->LoadMeshFileFbx(fileName)->GetMeshData(index);
}

MeshData* MeshComponent::GetMeshData(void)
{
	return meshData;
}
