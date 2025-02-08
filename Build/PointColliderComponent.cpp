#include "PointColliderComponent.h"

PointColliderComponent::PointColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

PointColliderComponent::~PointColliderComponent()
{
}

void PointColliderComponent::Awake(void)
{
	ColliderComponent::Awake();
	this->shape = Shape::Point;
}

void PointColliderComponent::Uninit(void)
{
	ColliderComponent::Uninit();
}

void PointColliderComponent::Update(void)
{
	ColliderComponent::Update();

}
