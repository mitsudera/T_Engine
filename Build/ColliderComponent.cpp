#include "ColliderComponent.h"
#include "gameobject.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "GameEngine.h"
#include "RigidBodyComponent.h"
#include "World.h"
ColliderComponent::ColliderComponent()
{
	this->attribute = Component::Attribute::Collider;

}
ColliderComponent::ColliderComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	this->attribute = Component::Attribute::Collider;
	
}

ColliderComponent::~ColliderComponent()
{

}

void ColliderComponent::Awake(void)
{
	Component::Awake();
	
	this->attribute = Attribute::Collider;
	result.hitObject.clear();
	enable = FALSE;
	this->checkRadius = 1.0f;
	isRigid = FALSE;
	TypeName = typeid(ColliderComponent).name();;

}

void ColliderComponent::Init(void)
{
	Component::Init();
}

void ColliderComponent::Uninit(void)
{
	Component::Uninit();
}

void ColliderComponent::Update(void)
{
	Component::Update();



}

void ColliderComponent::FixedUpdate(void)
{
	Component::FixedUpdate();

}

void ColliderComponent::OnEnable(void)
{
	Component::OnEnable();
	OnCollider();
}

void ColliderComponent::OnDisable(void)
{
	Component::OnDisable();
	OffCollider();
}

BOOL ColliderComponent::GetHitTag(string* tag)
{
	for (GameObject* gameObject : result.hitObject)
	{
		if (gameObject->GetTag() == tag)
		{
			return TRUE;
		}
	}

	return FALSE;
}

GameObject* ColliderComponent::GetHitTagObject(string* tag)
{
	for (GameObject* gameObject : result.hitObject)
	{
		if (gameObject->GetTag() == tag)
		{
			return gameObject;
		}
	}


	return nullptr;
}


ColliderComponent::Shape ColliderComponent::GetShape(void)
{
	return this->shape;

}

void ColliderComponent::SetShape(Shape shape)
{
	this->shape = shape;
}



void ColliderComponent::SetHitObject(GameObject* gameObject)
{
	this->result.hitObject.push_back(gameObject);

}

BOOL ColliderComponent::GetHitObject(GameObject* gameObject)
{
	BOOL ans = FALSE;

	if (gameObject==nullptr)
	{
		return FALSE;
	}

	for (int i = 0; i < result.hitObject.size(); i++)
	{
		if (result.hitObject[i]==gameObject)
		{
			ans = TRUE;
		}
	}


	return ans;
}


vector<pair<GameObject*, XMFLOAT4>>& ColliderComponent::GetHitRigidObject(void)
{
	return result.hitRigidObject;
}

void ColliderComponent::Clear(void)
{
	this->result.hitObject.clear();
	this->result.hitRigidObject.clear();


}

void ColliderComponent::SetPivot(XMFLOAT3 pivot)
{
	this->pivot = XMLoadFloat3(&pivot);
}

void ColliderComponent::SetPivot(XMVECTOR pivot)
{
	this->pivot = pivot;
}

XMVECTOR ColliderComponent::GetPivot(void)
{
	return pivot;
}

XMFLOAT3 ColliderComponent::GetCenter(void)
{
	return this->center;
}

float ColliderComponent::GetCheckRadius(void)
{
	return this->checkRadius;
}

void ColliderComponent::SetCheckRadius(float r)
{
	checkRadius = r;
}

void ColliderComponent::UpdateCenter(void)
{
	XMStoreFloat3(&this->center, XMVector3Transform(pivot, GetWorldMtx()));

}

void ColliderComponent::SetIsRigid(BOOL b)
{
	isRigid = b;
}

BOOL ColliderComponent::GetIsRigid(void)
{
	return isRigid;
}

void ColliderComponent::SetRigidObject(GameObject* obj, XMFLOAT4 depth)
{
	result.hitRigidObject.push_back(make_pair(obj, depth));
}

BOOL ColliderComponent::GetEnable(void)
{
	return enable;
}








void ColliderComponent::OnCollider(void)
{
	if (enable==FALSE)
	{
		enable = TRUE;
		this->GetGameObject()->GetWorld()->GetCollisionManager()->AddCollider(this);

	}
}

void ColliderComponent::OffCollider(void)
{
	if (enable == TRUE)
	{
		enable = FALSE;
		this->GetGameObject()->GetWorld()->GetCollisionManager()->DeleteCllider(this);
	}
}

