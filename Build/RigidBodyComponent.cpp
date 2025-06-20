#include "RigidBodyComponent.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "GameEngine.h"
#include "TerrainComponent.h"
#include "ColliderComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "ProjectSetting.h"

constexpr XMFLOAT3 gravity = XMFLOAT3(0.0f, -981.f, 0.0f); // 標準重力
constexpr float onGroundFacter = 5.0f;

RigidBodyComponent::RigidBodyComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Awake(void)
{
	Component::Awake();
	collider = pGameObject->GetComponent<ColliderComponent>();
	collider->SetIsRigid(TRUE);
	this->pGameObject->GetScene()->AddSceneRigidBodyComponent(this);

    useGravity = TRUE;
    mass = 1.0f;
    drag = 0.1f;
	friction = 50.0f;
	onGround = TRUE;
	isStatic = FALSE;
	isFixTerrain = TRUE;
	TypeName = typeid(RigidBodyComponent).name();;
	offsetY = 0.0f;
}

void RigidBodyComponent::Init(void)
{
	Component::Init();
	transform = GetTransFormComponent();
	worldPos = XMLoadFloat3(&transform->GetWorldPos());
	offGroundTime = 0.0f;
	groundLen = 0.0f;
	move = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	velocity= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

}

void RigidBodyComponent::FixedUpdate(void)
{
	Component::FixedUpdate();

	float deltaTime = pGameEngine->GetFixedDeltaTime();

	XMVECTOR bWpos = worldPos;
	worldPos += velocity * deltaTime;

	worldPos += move;
	move = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	
	if (!isStatic)
	{


		XMVECTOR accel= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		// 重力の適用
		if (useGravity)
		{
			XMVECTOR gravityV = XMLoadFloat3(&gravity); // 標準重力
			accel += gravityV * deltaTime;
		}



		// 空気抵抗の適用
		XMVECTOR dragForce = velocity * drag * -1.0f;
		accel += dragForce * deltaTime;


		if (onGround)
		{
			XMVECTOR fricForce = velocity * friction * -1.0f;
			fricForce.m128_f32[1] = 0.0f;
			accel += fricForce * deltaTime;
		}
		velocity += accel;




		transform->SetWorldPosition(worldPos);

	}


}

void RigidBodyComponent::FixedLateUpdate(void)
{
	Component::FixedLateUpdate();
	if (isStatic)
	{
		if (isFixTerrain)
		{
			//地面との当たり判定を取得し座標修正
			if (collider->GetHitTag(pGameEngine->GetProjectSetting()->GetTag("Field")))
			{


				float h = collider->GetHitTagObject(pGameEngine->GetProjectSetting()->GetTag("Field"))->GetComponent<TerrainComponent>()->GetHeight(GetWorldPos());

				worldPos.m128_f32[1] = h;
				onGround = TRUE;
				transform->SetWorldPosition(worldPos);

			}
			else
			{
				onGround = FALSE;
			}

		}


		return;

	}


	float deltaTime = pGameEngine->GetFixedDeltaTime();
	bool fixY = false;
	//他の剛体との当たり判定を取得し座標修正
	for (pair<GameObject*, XMFLOAT4> rbObj : collider->GetHitRigidObject())
	{
		RigidBodyComponent* colRb = rbObj.first->GetComponent<RigidBodyComponent>();
		float penetrationDepth = rbObj.second.w;

		XMFLOAT3 cnormal = XMFLOAT3(rbObj.second.x, rbObj.second.y, rbObj.second.z);


		XMFLOAT3 myCenter = GetWorldPos();
		XMFLOAT3 colCenter = colRb->GetWorldPos();
		XMVECTOR v = XMLoadFloat3(&myCenter) - XMLoadFloat3(&colCenter);

		if (v.m128_f32[0] > 0.0f && cnormal.x > 0.0f)
		{
			cnormal.x *= -1;
		}
		else if(v.m128_f32[0] < 0.0f && cnormal.x < 0.0f)
		{
			cnormal.x *= -1;

		}
		if (v.m128_f32[1] > 0.0f && cnormal.y > 0.0f)
		{
			cnormal.y *= -1;
		}
		else if (v.m128_f32[1] < 0.0f && cnormal.y < 0.0f)
		{
			cnormal.y *= -1;

		}

		if (v.m128_f32[2] > 0.0f && cnormal.z > 0.0f)
		{
			cnormal.z *= -1;
		}
		else if (v.m128_f32[2] < 0.0f && cnormal.z < 0.0f)
		{
			cnormal.z *= -1;

		}


		XMVECTOR direction = XMLoadFloat3(&cnormal);


		// 位置の修正
		XMVECTOR correction = penetrationDepth * -direction;

		if (correction.m128_f32[1] > 0.0f)
		{
			velocity.m128_f32[1] = 0.0f;
			onGround = TRUE;
			offGroundTime = 0.0f;
			groundLen = 0.0f;
			fixY = true;
		}
		worldPos += correction;



	}

	if (!fixY)
	{
		//地面との当たり判定を取得し座標修正
		if (collider->GetHitTag(pGameEngine->GetProjectSetting()->GetTag("Field")))
		{


			float h = collider->GetHitTagObject(pGameEngine->GetProjectSetting()->GetTag("Field"))->GetComponent<TerrainComponent>()->GetHeight(GetWorldPos());
			if (worldPos.m128_f32[1] > h)
			{
				onGround = FALSE;
				offGroundTime += deltaTime;
				groundLen = worldPos.m128_f32[1] - h;
			}
			else
			{

				worldPos.m128_f32[1] = h;
				velocity.m128_f32[1] = 0.0f;
				onGround = TRUE;
				offGroundTime = 0.0f;
				groundLen = 0.0f;
			}
		}
		else
		{
			onGround = FALSE;
		}

	}



	transform->SetWorldPosition(worldPos);


}



void RigidBodyComponent::LateUpdate(void)
{
    Component::LateUpdate();
	

}

void RigidBodyComponent::Uninit(void)
{
	Component::Uninit();
	this->pGameObject->GetScene()->RemoveSceneRigidBodyComponent(this);

}

void RigidBodyComponent::OnEnable(void)
{
	Component::OnEnable();

}

void RigidBodyComponent::OnDisable(void)
{
	Component::OnDisable();
}

void RigidBodyComponent::SetMass(float f)
{
	mass = f;
}

void RigidBodyComponent::SetDrag(float f)
{
	drag = f;
}

void RigidBodyComponent::SetFriction(float f)
{
	friction = f;
}

void RigidBodyComponent::SetAngularDrag(float f)
{
	angularDrag = f;
}

void RigidBodyComponent::SetUseGarvity(BOOL b)
{
	useGravity = b;
}

void RigidBodyComponent::SetIsKinematic(BOOL b)
{
	isKinematic = b;
}

void RigidBodyComponent::SetIsStatic(BOOL b)
{
	isStatic = b;
}

void RigidBodyComponent::SetIsFixTerrain(BOOL b)
{
	isFixTerrain = b;
}

float RigidBodyComponent::GetMass(void)
{
	return mass;
}

float RigidBodyComponent::GetDrag(void)
{
	return drag;
}

float RigidBodyComponent::GetFriction(void)
{
	return friction;
}

float RigidBodyComponent::GetAngularDrag(void)
{
	return angularDrag;
}

BOOL RigidBodyComponent::GetUseGarvity(void)
{
	return useGravity;
}

BOOL RigidBodyComponent::GetIsKinematic(void)
{
	return isKinematic;
}

BOOL RigidBodyComponent::GetIsStatic(void)
{
	return isStatic;
}

BOOL RigidBodyComponent::GetIsFixTerrain(void)
{
	return isFixTerrain;
}



void RigidBodyComponent::AddForce(XMVECTOR force)
{
	velocity += force;
}

void RigidBodyComponent::RotVelocityY(float f)
{
	XMVECTOR qton = XMQuaternionRotationAxis(yonevec(), f);

	velocity = XMVector3Rotate(velocity, qton);

}

void RigidBodyComponent::MovePosition(XMVECTOR vec, float moveValue)
{
	move += vec * moveValue;
}

void RigidBodyComponent::MovePosition(XMFLOAT3 vec, float moveValue)
{
	MovePosition(XMLoadFloat3(&vec), moveValue);
}

float RigidBodyComponent::GetOffGroundTime(void)
{
	return offGroundTime;
}

float RigidBodyComponent::GetGroundLength(void)
{
	return groundLen;
}


BOOL RigidBodyComponent::GetOnGround(void)
{
	return this->onGround;
}

void RigidBodyComponent::SetWorldPos(XMFLOAT3 pos)
{
	this->worldPos = XMLoadFloat3(&pos);
}

