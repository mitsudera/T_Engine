#include "TerrainColliderComponent.h"
#include "TerrainComponent.h"
TerrainColliderComponent::TerrainColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

TerrainColliderComponent::~TerrainColliderComponent()
{
}

void TerrainColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::Terrain;

}

void TerrainColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();

}

void TerrainColliderComponent::Update(void)
{
	ColliderComponent::Update();
}

void TerrainColliderComponent::SetTerrainComponent(TerrainComponent* com)
{
	this->terrain = com;
	this->center = GetWorldPos();
	this->size = com->GetHW();
	this->checkRadius = -1.0f;


}

float TerrainColliderComponent::GetHeight(XMFLOAT3 pos)
{
	return terrain->GetHeight(pos);
}

XMFLOAT2 TerrainColliderComponent::GetSize(void)
{
	return size;
}

