#include "component.h"
#include "transformcomponent.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "input.h"
#include "Scene.h"
#include "World.h"



Component::Component()
{
	attribute = Attribute::Component;
}
Component::Component(GameObject* gameObject)
{
	this->pGameObject = gameObject;

}

Component::~Component()
{
}


void Component::Awake(void)
{
	this->pGameEngine = pGameObject->GetScene()->GetGameEngine();
	this->pGameObject->GetScene()->AddSceneComponent(this);
	this->pScene = pGameObject->GetScene();
	this->pWorld = pGameObject->GetWorld();
	this->attribute = Attribute::Component;
	this->input = pGameEngine->GetInput();
	TypeName = typeid(Component).name();;
}

void Component::Init(void)
{
	this->SetActive(TRUE);

}

void Component::Uninit(void)
{
	this->SetActive(FALSE);
	this->pGameObject->GetScene()->RemoveSceneComponent(this);
}

void Component::FixedUpdate(void)
{
}

void Component::FixedLateUpdate(void)
{
}

void Component::Update(void)
{
	if (!isActive) return;
}

void Component::LateUpdate(void)
{
	if (!isActive) return;

}

void Component::Draw(void)
{
	if (!isActive) return;

}

void Component::DebugDraw(void)
{
	if (!isActive) return;

}

void Component::OnEnable(void)
{
}

void Component::OnDisable(void)
{
}

GameObject* Component::GetGameObject(void)
{
	return this->pGameObject;
}

Component::Attribute Component::GetAttribute(void)
{
	return this->attribute;
}

void Component::SetActive(BOOL b)
{
	if (isActive==b)
	{
		return;
	}
	else if (isActive == FALSE)
	{
		isActive = TRUE;
		this->OnEnable();
	}
	else if (isActive == TRUE)
	{
		isActive = FALSE;
		this->OnDisable();
	}

}

BOOL Component::GetActive(void)
{
	if (!GetGameObject()->GetActive())
	{
		return FALSE;
	}

	return this->isActive;
}

XMMATRIX Component::GetWorldMtx(XMMATRIX world)
{
	return this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
}

XMMATRIX Component::GetWorldMtx(void)
{
	return this->pGameObject->GetTransFormComponent()->GetWorldMtx();
}

XMFLOAT3 Component::GetWorldPos()
{
	return this->pGameObject->GetTransFormComponent()->GetWorldPos();
}

TransformComponent* Component::GetTransFormComponent(void)
{
	return this->pGameObject->GetTransFormComponent();
}

string Component::GetTypeName(void)
{
	return this->TypeName;
}


