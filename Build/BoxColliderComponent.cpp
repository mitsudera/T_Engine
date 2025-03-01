#include "BoxColliderComponent.h"
#include "GameEngine.h"
#include "DebugUtility.h"
#include "CBufferManager.h"
BoxColliderComponent::BoxColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

BoxColliderComponent::~BoxColliderComponent()
{
}

void BoxColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::Box;
	TypeName = typeid(BoxColliderComponent).name();;

}

void BoxColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void BoxColliderComponent::Update(void)
{
	ColliderComponent::Update();

}
void BoxColliderComponent::DebugDraw(void)
{
	Component::DebugDraw();
	XMMATRIX world = XMMatrixIdentity();

	XMVECTOR wposv = XMVector3Transform(pivot, GetWorldMtx());

	XMMATRIX pos = XMMatrixTranslationFromVector(wposv);
	XMMATRIX scl = XMMatrixScaling(size.x, size.y, size.z);


	world = XMMatrixMultiply(scl, pos);







	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);

	pGameEngine->GetDebugUtility()->DrawDebugBox();

}

void BoxColliderComponent::SetBox(XMFLOAT3 size)
{
	this->size = size;
	XMVECTOR len = XMVector3Length(XMLoadFloat3(&size));
	XMStoreFloat(&checkRadius, len / 2);


}

void BoxColliderComponent::SetBox(float x,float y,float z)
{
	this->size = XMFLOAT3(x, y, z);
	XMVECTOR len = XMVector3Length(XMLoadFloat3(&size));
	XMStoreFloat(&checkRadius, len / 2);


}


XMFLOAT3 BoxColliderComponent::GetSize(void)
{
	return this->size;
}
