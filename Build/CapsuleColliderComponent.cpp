#include "CapsuleColliderComponent.h"
#include "GameEngine.h"
#include "DebugUtility.h"
CapsuleColliderComponent::CapsuleColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

CapsuleColliderComponent::~CapsuleColliderComponent()
{
}

void CapsuleColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::Capsule;
}

void CapsuleColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void CapsuleColliderComponent::Update(void)
{
	ColliderComponent::Update();


}

void CapsuleColliderComponent::FixedUpdate(void)
{
	ColliderComponent::FixedUpdate();
	XMMATRIX worldMtx = GetWorldMtx();
	XMVECTOR sp = XMLoadFloat3(&lStartPoint);
	XMVECTOR ep = XMLoadFloat3(&lEndPoint);

	sp = XMVector3Transform(sp, worldMtx);
	ep = XMVector3Transform(ep, worldMtx);

	XMStoreFloat3(&startPoint, sp);
	XMStoreFloat3(&endPoint, ep);

}

void CapsuleColliderComponent::SetCapsule(XMFLOAT3 start, XMFLOAT3 end, float radius)
{
	this->lStartPoint = start;
	this->lEndPoint = end;
	XMVECTOR sp = XMLoadFloat3(&lStartPoint);
	XMVECTOR ep = XMLoadFloat3(&lEndPoint);

	XMVECTOR len = XMVector3Length(ep - sp);

	XMStoreFloat(&checkRadius, len);

	this->radius = radius;
}

XMFLOAT3 CapsuleColliderComponent::GetStart(void)
{
	return this->startPoint;
}

XMFLOAT3 CapsuleColliderComponent::GetEnd(void)
{
	return this->endPoint;
}

float CapsuleColliderComponent::GetRadius(void)
{
	return this->radius;
}
void CapsuleColliderComponent::DebugDraw(void)
{
	Component::DebugDraw();
	pGameEngine->GetDebugUtility()->DrawDebugCapsule(lStartPoint,lEndPoint,radius,GetWorldMtx());

}
