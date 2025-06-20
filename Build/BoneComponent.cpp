#include "BoneComponent.h"
#include "SkinMeshTreeData.h"
#include "SkinMeshLinkerComponent.h"
#include "transformcomponent.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "DebugUtility.h"
#include "CBufferManager.h"
#include "AssetsManager.h"

constexpr XMFLOAT3 gravity = XMFLOAT3(0.0f, -981.0f, 0.0f); // 標準重力

BoneComponent::BoneComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}
BoneComponent::~BoneComponent()
{
}
void BoneComponent::Awake(void)
{
	Component::Awake();
	isPhysics = FALSE;
	joint = Joint::Standard;
	TypeName = typeid(BoneComponent).name();
	mass = 20.0f;
	tension = 15000.0f;
	resistance = 400.0;

}
void BoneComponent::Init(void)
{
	Component::Init();
	transform = GetTransFormComponent();
}
void BoneComponent::FixedUpdate(void)
{
	Component::FixedUpdate();
	if (this->isPhysics)
	{
		switch (joint)
		{
		case BoneComponent::Joint::Standard:
		{
			// 重力の適用
			XMVECTOR gravityV = XMLoadFloat3(&gravity); // 標準重力
			velocity += gravityV * pGameEngine->GetFixedDeltaTime();


			// 空気抵抗の適用
			XMVECTOR dragForce = velocity * 0.1f * -1.0f;
			velocity += dragForce * pGameEngine->GetFixedDeltaTime();




			XMFLOAT3 pos = transform->GetWorldPos();

			XMVECTOR posv = XMLoadFloat3(&pos);
			XMVECTOR rposv = posv + velocity;

			XMFLOAT3 rpos;
			XMStoreFloat3(&rpos, rposv);

			float h = 0.0f;
			if (rpos.y < h)
			{
				rpos.y = h;
				rposv = XMLoadFloat3(&rpos);
			}




			if (!isRoot)
			{
				XMVECTOR pposv = XMLoadFloat3(&parentBone->GetTransFormComponent()->GetWorldPos());
				XMVECTOR lenV = posv - pposv;
				XMVECTOR len = XMVector3Length(lenV);
				rposv = XMVector3Normalize(rposv) * len;

			}

			XMStoreFloat3(&pos, rposv);


			transform->SetWorldPosition(pos);

			break;

		}
		case BoneComponent::Joint::Spring:
		{
			XMFLOAT3 zero = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMVECTOR force = XMLoadFloat3(&zero);
			XMVECTOR accel = XMLoadFloat3(&zero);

			XMVECTOR dPos = XMVector3Transform(defaultLength, parentBone->GetWorldMtx());
			XMVECTOR lenv = wpv - dPos;
			lenv.m128_f32[3] = 0.0f;

			//張力
			force += -lenv * tension;

			// 重力の適用
			XMVECTOR gravityV = XMLoadFloat3(&gravity);
			force += gravityV * mass;

			////抵抗
			force += velocity * -resistance;



			accel += (force / mass);

			velocity += accel * pGameEngine->GetFixedDeltaTime();

			wpv += velocity * pGameEngine->GetFixedDeltaTime();

			XMFLOAT3 resPos;

			XMStoreFloat3(&resPos, wpv);

			transform->SetWorldPosition(resPos);

			break;

		}
		case BoneComponent::Joint::Hair:
		{

			break;
		}


		}
	}


}
void BoneComponent::LateUpdate(void)
{
	Component::LateUpdate();

}
void BoneComponent::DebugDraw(void)
{
	Component::DebugDraw();
	XMMATRIX world = XMMatrixIdentity();
	world = GetTransFormComponent()->GetWorldMtx(world);
	this->pGameEngine->GetCBufferManager()->SetWorldMtx(&world);

	pGameEngine->GetDebugUtility()->DrawDebugBox();

}
XMMATRIX BoneComponent::GetInitMtxInverse(void)
{
	return this->initMtxInv;
}
void BoneComponent::SetBone(BoneData* data, SkinMeshLinkerComponent* linker)
{
	boneData = data;
	rigName = data->GetName();

	this->GetTransFormComponent()->SetLocalMtx(data->GetLocalOffset());

	this->initMtx = data->GetWorldOffset();
	this->initMtxInv = XMMatrixInverse(nullptr, initMtx);

	this->linker = linker;


	this->GetTransFormComponent()->UpdateMatrix();


	if (!data->GetParent())
	{
		isRoot = TRUE;
		XMFLOAT3 zero = XMFLOAT3(0.0f, 0.0f, 0.0f);
		defaultLength = XMLoadFloat3(&zero);
		fdl = 0.0f;
		XMFLOAT3 wpos = GetWorldPos();
		wpv = XMLoadFloat3(&wpos);
		this->parentBone = nullptr;

	}
	else if (data->GetParent()->GetAttribute() != SkinMeshTreeNode::Attribute::Bone)
	{
		isRoot = TRUE;
		XMFLOAT3 zero = XMFLOAT3(0.0f, 0.0f, 0.0f);
		defaultLength = XMLoadFloat3(&zero);
		fdl = 0.0f;
		XMFLOAT3 wpos = GetWorldPos();
		wpv = XMLoadFloat3(&wpos);


		this->parentBone = nullptr;
	}
	else
	{
		isRoot = FALSE;
		this->parentBone = pGameObject->GetParent()->GetComponent<BoneComponent>();
		defaultLength = XMLoadFloat3(&GetTransFormComponent()->GetPosition());
		XMVECTOR fdlv = XMVector3Length(defaultLength);

		XMStoreFloat(&fdl, fdlv);

		XMFLOAT3 wpos = GetWorldPos();
		wpv = XMLoadFloat3(&wpos);


	}

	//this->initMtxInv = XMMatrixInverse(nullptr, GetTransFormComponent()->GetWorldMtx());


	linker->AddBone(this);

}

void BoneComponent::SetBone(string path, unsigned int index, SkinMeshLinkerComponent* linker)
{
	string fileName = path;
	// 最後の '/' の位置を見つける
	size_t pos = fileName.rfind('/');

	// 最後の '/' が見つかった場合
	if (pos != string::npos) {
		// '/' より後の部分を切り出す
		fileName = fileName.substr(pos + 1);
	}

	boneData = dynamic_cast<BoneData*>(pGameEngine->GetAssetsManager()->LoadSkinMeshFileFbx(fileName)->GetNode(index));

	SetBone(boneData, linker);
}

XMMATRIX& BoneComponent::GetBoneMtx(void)
{
	initMtx;
	XMMATRIX mtx = GetWorldMtx();
	XMFLOAT3 p = GetWorldPos();
	boneMtx = XMMatrixTranspose(XMMatrixMultiply(initMtxInv, GetWorldMtx()));
	this;
	return boneMtx;
}

void BoneComponent::SetIsPhysics(BOOL b)
{
	isPhysics = b;
	if (b)
	{
		pGameObject->SetNotAnim(TRUE);
		velocity = XMVectorZero();
	}
	else
	{
		pGameObject->SetNotAnim(FALSE);

	}

	for (GameObject* go : pGameObject->GetChild())
	{
		go->GetComponent<BoneComponent>()->SetIsPhysics(b);
	}



}

BOOL BoneComponent::GetIsPhysics(void)
{
	return isPhysics;
}

void BoneComponent::SetJoint(Joint j)
{
	joint = j;

	for (GameObject* go : pGameObject->GetChild())
	{
		go->GetComponent<BoneComponent>()->SetJoint(j);
	}

}

void BoneComponent::SetIsRoot(BOOL b)
{
	isRoot = b;
}

void BoneComponent::SetParentBone(BoneComponent* parent)
{
	this->parentBone = parent;
}

void BoneComponent::CreateChild(GameObject* child)
{
	BoneComponent* bc = child->GetComponent<BoneComponent>();
	if (bc)
	{
		this->childArray.push_back(bc);

	}
}

void BoneComponent::SetSpringPhysics(float mass, float tension, float resistance)
{
	isPhysics = TRUE;
	joint = Joint::Spring;

	this->mass = mass;
	this->tension = tension;
	this->resistance = resistance;
}

string BoneComponent::GetRigName(void)
{
	return rigName;
}

BoneComponent::Joint BoneComponent::GetJoint(void)
{
	return joint;
}

float BoneComponent::GetMass(void)
{
	return mass;
}

float BoneComponent::GetTension(void)
{
	return tension;
}

float BoneComponent::GetResistance(void)
{
	return resistance;
}

void BoneComponent::SetMass(float mass)
{
	this->mass = mass;
}

void BoneComponent::SetTension(float tention)
{
	this->tension = tention;
}

void BoneComponent::SetResistance(float resistance)
{
	this->resistance = resistance;
}

BoneData* BoneComponent::GetBoneData(void)
{
	return boneData;
}

SkinMeshLinkerComponent* BoneComponent::GetLinker(void)
{
	return linker;
}

