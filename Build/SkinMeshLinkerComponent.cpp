#include "SkinMeshLinkerComponent.h"
#include "SkinMeshTreeData.h"
#include "SkinMeshComponent.h"
#include "BoneComponent.h"
#include "transformcomponent.h"
#include "renderer.h"
#include "CBufferManager.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "gameobject.h"
SkinMeshLinkerComponent::SkinMeshLinkerComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

SkinMeshLinkerComponent::~SkinMeshLinkerComponent()
{
}

void SkinMeshLinkerComponent::Awake(void)
{
	Component::Awake();
	pRenderer = pGameEngine->GetRenderer();
	pCBufferManager = pGameEngine->GetCBufferManager();
	boneMtxArray = new XMMATRIX[BONE_MAX];

	skinMeshShader = pGameEngine->GetAssetsManager()->GetSkinMeshShader();

	TypeName = typeid(SkinMeshLinkerComponent).name();;

}


void SkinMeshLinkerComponent::Update(void)
{
	Component::Update();


}

void SkinMeshLinkerComponent::LateUpdate(void)
{
	Component::LateUpdate();

	for (int i = 0; i < boneArray.size(); i++)
	{
		boneMtxArray[i] = boneArray[i]->GetBoneMtx();
	}
	boneMtxArray[255] = XMMatrixIdentity();

}

//void SkinMeshLinkerComponent::Draw(void)
//{
//	PrimitiveComponent::Draw();
//	skinMeshShader->SetBoneBuffer(boneMtxArray);
//}
//
//void SkinMeshLinkerComponent::ShadowMapping(void)
//{
//	PrimitiveComponent::ShadowMapping();
//	skinMeshShader->SetBoneBuffer(boneMtxArray);
//}
void SkinMeshLinkerComponent::SetBoneBuffer(void)
{
	skinMeshShader->SetBoneBuffer(boneMtxArray);
}


void SkinMeshLinkerComponent::Uninit(void)
{
	Component::Uninit();
}

void SkinMeshLinkerComponent::AddBone(BoneComponent* bone)
{
	boneArray.push_back(bone);
}

vector<BoneComponent*>& SkinMeshLinkerComponent::GetBoneArray(void)
{
	return boneArray;
}

int SkinMeshLinkerComponent::GetBoneCount(void)
{
	return this->boneCount;
}

void SkinMeshLinkerComponent::SetBoneCount(int cnt)
{
	boneCount = cnt;
}

void SkinMeshLinkerComponent::SetRagdoll(BOOL enable)
{
	for (BoneComponent* bc : boneArray)
	{
		bc->SetIsPhysics(enable);
		bc->SetJoint(BoneComponent::Joint::Standard);
	}
}

void SkinMeshLinkerComponent::SetSpring(string name, float mass, float tension, float resistance)
{
	for (BoneComponent* bc : boneArray)
	{
		if (bc->GetRigName() == name)
		{
			bc->SetSpringPhysics(mass, tension, resistance);
		}
	}

}


