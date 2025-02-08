#pragma once
#include "PrimitiveComponent.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "UIMaterial.h"
#include "GameEngine.h"
#include "Material.h"
#include "AssetsManager.h"
#include "renderer.h"
#include "gameobject.h"
#include "Scene.h"
#include "ShadowMap.h"
#include "transformcomponent.h"
#include "World.h"
PrimitiveComponent::PrimitiveComponent()
{
	
}

PrimitiveComponent::PrimitiveComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;

}

PrimitiveComponent::~PrimitiveComponent()
{
	
}

void PrimitiveComponent::Awake(void)
{
	Component::Awake();
	this->pRenderer = pGameEngine->GetRenderer();
	this->pCBufferManager = pGameEngine->GetCBufferManager();
	this->pAssetsManager = pGameEngine->GetAssetsManager();

	this->pGameObject->GetScene()->AddScenePrimitiveComponent(this);
	attribute = Attribute::Primitive;
	alphaTest = FALSE;
	hasShadow = TRUE;
	drawShadow = TRUE;

	isFrustumCulling = FALSE;
}

void PrimitiveComponent::Update(void)
{
	Component::Update();
}

void PrimitiveComponent::Uninit(void)
{
	Component::Uninit();
	this->pGameObject->GetScene()->RemoveScenePrimitiveComponent(this);

}



void PrimitiveComponent::Draw(void)
{
	Component::Draw();



	pRenderer->SetAlphaTestEnable(this->alphaTest);
	if (this->alphaTest)
	{
		pRenderer->SetBlendState(BLEND_MODE::BLEND_MODE_ALPHABLEND);

	}
	if (drawShadow)
	{
		pWorld->GetShadowMap()->SetEnable(TRUE);
	}
	else
	{
		pWorld->GetShadowMap()->SetEnable(FALSE);

	}
}

void PrimitiveComponent::ShadowMapping(void)
{
	if (!hasShadow)
		return;
}

Material* PrimitiveComponent::GetMaterial(void)
{
	return this->material;
}

void PrimitiveComponent::SetMaterial(Material* matelial)
{
	this->material = matelial;
}

Material* PrimitiveComponent::LoadMaterial(Material* material)
{

	this->material = pGameEngine->GetAssetsManager()->LoadMaterial(material);
	return this->material;
}

Material* PrimitiveComponent::GetShadowMaterial(void)
{
	return this->shadowMaterial;
}

void PrimitiveComponent::SetCullingMode(int cullMode)
{
	this->cullMode = cullMode;
}

int PrimitiveComponent::GetCullingMode(void)
{
	return this->cullMode;
}

void PrimitiveComponent::SetAlphaTest(BOOL enable)
{
	this->alphaTest = enable;
}

BOOL PrimitiveComponent::GetAlphaTest(void)
{
	return this->alphaTest;
}

BOOL PrimitiveComponent::GetIsFrustumCulling(void)
{
	return isFrustumCulling;
}

void PrimitiveComponent::SetIsFrustumCulling(BOOL b)
{
	this->isFrustumCulling = b;
}

void PrimitiveComponent::SetBoxMinMax(XMVECTOR min, XMVECTOR max)
{
	isFrustumCulling = TRUE;
	boxMin = min;
	boxMax = max;
}

XMVECTOR PrimitiveComponent::GetBoxMin(void)
{
	return XMVector3Transform(boxMin,GetWorldMtx());
}

XMVECTOR PrimitiveComponent::GetBoxMax(void)
{
	return XMVector3Transform(boxMax, GetWorldMtx());
}





BOOL PrimitiveComponent::GetHasShadow(void)
{
	return hasShadow;
}

void PrimitiveComponent::SetHasShadow(BOOL b)
{
	hasShadow = b;
}

BOOL PrimitiveComponent::GetDrawShadow(void)
{
	return drawShadow;
}

void PrimitiveComponent::SetDrawShadow(BOOL b)
{
	drawShadow = b;
}
