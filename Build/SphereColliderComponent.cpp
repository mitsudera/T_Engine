#include "SphereColliderComponent.h"
#include "DebugUtility.h"
#include "GameEngine.h"
#include "CBufferManager.h"
SphereColliderComponent::SphereColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

SphereColliderComponent::~SphereColliderComponent()
{
}

void SphereColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::Sphere;
	TypeName = typeid(SphereColliderComponent).name();

}

void SphereColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void SphereColliderComponent::Update(void)
{
	ColliderComponent::Update();

}

void SphereColliderComponent::DebugDraw(void)
{
	ColliderComponent::DebugDraw();

	XMVECTOR pos = XMVector3Transform(pivot, GetWorldMtx());

	XMMATRIX world = XMMatrixScaling(checkRadius, checkRadius, checkRadius) * XMMatrixTranslationFromVector(pos);

	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);

	pGameEngine->GetDebugUtility()->DrawDebugSphere();

}

void SphereColliderComponent::SetRadius(float r)
{
	this->checkRadius = r;
}
