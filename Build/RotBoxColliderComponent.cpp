#include "RotBoxColliderComponent.h"
#include "GameEngine.h"
#include "DebugUtility.h"
#include "CBufferManager.h"
RotBoxColliderComponent::RotBoxColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

RotBoxColliderComponent::~RotBoxColliderComponent()
{
}

void RotBoxColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::RotBox;
}

void RotBoxColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void RotBoxColliderComponent::Update(void)
{
	ColliderComponent::Update();

}
void RotBoxColliderComponent::FixedUpdate(void)
{
	ColliderComponent::FixedUpdate();
	
	XMMATRIX world = GetWorldMtx();



	// ワールドマトリックスから回転行列を抽出
	XMMATRIX rotationMatrix = world;
	rotationMatrix.r[3] = XMVectorSet(0, 0, 0, 1); // 平行移動成分をゼロに設定

	// 回転行列をクォータニオンに変換
	rotQton = XMQuaternionRotationMatrix(rotationMatrix);

}
void RotBoxColliderComponent::DebugDraw(void)
{
	Component::DebugDraw();
	XMMATRIX scl = XMMatrixScaling(size.x, size.y, size.z);
	
	XMMATRIX pos = XMMatrixTranslation(center.x, center.y, center.z);

	XMMATRIX world = scl * XMMatrixRotationQuaternion(rotQton) * pos;

	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);

	pGameEngine->GetDebugUtility()->DrawDebugBox();

}

void RotBoxColliderComponent::SetRotBox(XMFLOAT3 size)
{
	this->size = size;
	XMVECTOR len = XMVector3Length(XMLoadFloat3(&size));
	XMStoreFloat(&checkRadius, len / 2);


}

XMFLOAT3 RotBoxColliderComponent::GetSize(void)
{
	return this->size;
}

XMVECTOR RotBoxColliderComponent::GetRot(void)
{
	return rotQton;
}
