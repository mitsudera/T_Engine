#include "PointLightComponent.h"
#include "gameobject.h"
#include "LightManager.h"
#include "GameEngine.h"
#include "World.h"
#include "transformcomponent.h"
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
	TypeName = typeid(PointLightComponent).name();;

}

void PointLightComponent::Uninit(void)
{
	Component::Uninit();
	pWorld->GetLightmanager()->RemoveLight(this);
}

void PointLightComponent::Update(void)
{
	Component::Update();
	XMFLOAT3 pos = GetTransFormComponent()->GetWorldPos();
	this->param.m_Position = { pos.x,pos.y,pos.z,1.0f };
}


POINT_LIGHT_PARAM& PointLightComponent::GetLightParam(void)
{
	return this->param;
}

void PointLightComponent::SetLightParam(POINT_LIGHT_PARAM param)
{
	this->param = param;
}


