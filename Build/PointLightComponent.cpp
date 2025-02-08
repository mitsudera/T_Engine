#include "PointLightComponent.h"
#include "gameobject.h"
#include "LightManager.h"
#include "GameEngine.h"
#include "World.h"
PointLightComponent::PointLightComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::Awake(void)
{
	Component::Awake();
	index= pWorld->GetLightmanager()->AddLight(this);

}

void PointLightComponent::Uninit(void)
{
	Component::Uninit();
	pWorld->GetLightmanager()->RemoveLight(this);
}

void PointLightComponent::Update(void)
{
	Component::Update();
}


POINT_LIGHT_PARAM& PointLightComponent::GetLightParam(void)
{
	return this->param;
}


