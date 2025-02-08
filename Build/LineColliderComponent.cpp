#include "LineColliderComponent.h"

LineColliderComponent::LineColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

LineColliderComponent::~LineColliderComponent()
{
}

void LineColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::Line;
}

void LineColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void LineColliderComponent::Update(void)
{
	ColliderComponent::Update();
	XMMATRIX worldMtx = GetWorldMtx();
	XMVECTOR sp = XMLoadFloat3(&startPoint);
	XMVECTOR ep = XMLoadFloat3(&endPoint);

	sp = XMVector3Transform(sp, worldMtx);
	ep = XMVector3Transform(ep, worldMtx);

	XMStoreFloat3(&startPoint, sp);
	XMStoreFloat3(&endPoint, ep);
}

void LineColliderComponent::SetLine(XMFLOAT3 start, XMFLOAT3 end)
{
	this->startPoint = start;
	this->endPoint = end;
	XMVECTOR sp = XMLoadFloat3(&startPoint);
	XMVECTOR ep = XMLoadFloat3(&endPoint);

	XMVECTOR len= XMVector3Length(ep - sp);

	XMStoreFloat(&checkRadius, len);


}

XMFLOAT3 LineColliderComponent::GetStart(void)
{
	return this->startPoint;
}

XMFLOAT3 LineColliderComponent::GetEnd(void)
{
	return this->endPoint;
}
