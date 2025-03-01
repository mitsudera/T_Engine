#include "gameobject.h"
#include "GameEngine.h"
#include "transformcomponent.h"
#include "Scene.h"
#include "ColliderComponent.h"
#include "component.h"
#include "primitivecomponent.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "MeshComponent.h"
#include "Material.h"
#include "SkinMeshComponent.h"
#include "SkinMeshLinkerComponent.h"
#include "BoneComponent.h"
#include "SkinMeshTreeData.h"
#include "ProjectSetting.h"

const string GameObject::TypeName = "GameObject";

GameObject::GameObject()
{
	pScene =nullptr;
	this->transformComponent = new TransformComponent(this);
	this->componentList.push_back(transformComponent);


}


GameObject::GameObject(Scene* scene)
{
	this->parent = nullptr;
	this->pScene = scene;
	tag = nullptr;

	this->layer = nullptr;


	this->transformComponent = new TransformComponent(this);
	this->componentList.push_back(transformComponent);

}

//GameObject::GameObject(GameObject* parent)
//{
//
//	this->parent = parent;
//	this->pScene = parent->GetScene();
//
//	this->tag = parent->GetTag();
//
//	this->layer = parent->GetLayer();
//
//	this->transformComponent = new TransformComponent(this);
//	this->componentList.push_back(transformComponent);
//}

GameObject::~GameObject()
{
	Destroy();
}

void GameObject::Awake(void)
{
	this->pWorld = pScene->GetWorld();

	this->pGameEngine = pScene->GetGameEngine();
	this->pProjectSetting = pGameEngine->GetProjectSetting();
	this->isActive = TRUE;
	this->transformComponent->Awake();
	notAnim = FALSE;
	type = Type::GameObject;
	
	this->tag = pProjectSetting->GetTag("Default");
	this->layer = pProjectSetting->GetLayer("Default");

	
	if (parent)
	{
		this->isActive = parent->GetActive();
		tag = parent->GetTag();
		notAnim = parent->GetNotAnim();;
		this->layer = parent->GetLayer();

	}
	SetID();
	pScene->AddAllGameObject(this);
}

void GameObject::Load(void)
{
	this->pWorld = pScene->GetWorld();

	this->pGameEngine = pScene->GetGameEngine();
	this->pProjectSetting = pGameEngine->GetProjectSetting();
	this->isActive = TRUE;
	this->transformComponent->Awake();
	notAnim = FALSE;
	type = Type::GameObject;
	

	
	pScene->AddAllGameObject(this);
}

void GameObject::InitAllComponent(void)
{
	for (Component* com : componentList)
	{
		com->Init();
	}
}


void GameObject::Destroy(void)
{

	for (Component* com: this->componentList)
	{
		com->Uninit();
		delete com;
	}
	this->componentList.clear();


	for (GameObject* child: childList)
	{
		child->Destroy();
		delete child;
	}


	this->childList.clear();

	
}

void GameObject::DynamicDestroy(void)
{
	for (Component* com : this->componentList)
	{
		com->Uninit();
		delete com;
	}
	this->componentList.clear();


	for (GameObject* child : childList)
	{
		child->DynamicDestroy();
		delete child;
	}


	this->childList.clear();
	pScene->RemoveAllGameObjectList(this);
	pScene->RemoveGameObject(this);
}

void GameObject::DeleteChild(GameObject* gameObject)
{
	childList.remove(gameObject);

	gameObject->Destroy();
	delete gameObject;

}

void GameObject::DeleteComponnt(Component* com)
{
	componentList.remove(com);
	com->Uninit();
	delete com;
}


Scene* GameObject::GetScene(void)
{

	return this->pScene;
}

World* GameObject::GetWorld(void)
{
	return this->pWorld;
}

GameEngine* GameObject::GetGameEngine(void)
{
	return this->pGameEngine;
}

TransformComponent* GameObject::GetTransFormComponent(void)
{
	return this->transformComponent;
}

void GameObject::SetTag(string* tag)
{
	this->tag = tag;
	for (GameObject* child : childList)
	{
		child->SetTag(tag);
	}

}

string* GameObject::GetTag(void)
{
	return this->tag;
}

void GameObject::SetLayer(string* layer)
{
	this->layer = layer;
	for (GameObject* child : childList)
	{
		child->SetLayer(layer);
	}
}

string* GameObject::GetLayer(void)
{
	return this->layer;
}


BOOL GameObject::GetActive(void)
{
	return isActive;
}

void GameObject::SetActive(BOOL isActive)
{
	this->isActive = isActive;

	for (Component* com : componentList)
	{
		com->SetActive(isActive);
	}
	for (GameObject* child: childList)
	{
		child->SetActive(isActive);
	}
}

GameObject* GameObject::GetParent(void)
{
	return this->parent;
}

GameObject* GameObject::GetRootObject(void)
{
	if (this->parent == nullptr)
	{
		return this;

	}
	else
	{
		return this->parent->GetRootObject();
	}

}

GameObject* GameObject::GetChild(int index)
{
	int cnt = 0;
	for (GameObject* child : childList)
	{

		if (cnt == index)
		{
			return child;
		}
		cnt++;
	}
	return nullptr;
}

GameObject* GameObject::GetChild(string name)
{
	for (GameObject* gameObj:childList)
	{
		if (gameObj->GetName()==name)
		{
			return gameObj;
		}
	}

	return nullptr;
}

list<GameObject*>& GameObject::GetChild()
{
	return this->childList;
}

GameObject* GameObject::SerchAllChild(string name)
{
	GameObject* ans = nullptr;

	if (this->GetName() == name)
	{
		return this;
	}
	
	for (GameObject* obj : this->childList)
	{
		ans = obj->SerchAllChild(name);
		if (ans != nullptr)
			return ans;
	}

	return ans;
}

list<Component*>& GameObject::GetComponentList(void)
{
	return this->componentList;
}

Component* GameObject::GetComponentAttrbute(Component::Attribute attr, int n)
{
	int cnt = 0;

	for (Component* com:componentList)
	{
		if (com->GetAttribute()==attr)
		{
			if (cnt==n)
			{
				return com;

			}
			else
			{
				cnt++;
			}
		}
	}
	return nullptr;
}

Component* GameObject::AddComponentByTypeName(string typeName)
{
	return pProjectSetting->AddComponentByTypeName(typeName, this);
}

Component* GameObject::DynamicAddComponentByTypeName(string typeName)
{
	return pProjectSetting->DynamicAddComponentByTypeName(typeName, this);
}

void GameObject::SetHasShadowAll(BOOL b)
{

	for (Component* component : GetComponentList())
	{
		if (component->GetAttribute() != Component::Attribute::Primitive)
			continue;

		PrimitiveComponent* primitiveComponent = static_cast<PrimitiveComponent*>(component);


		primitiveComponent->SetHasShadow(b);

	}
	for (GameObject* child : childList)
	{
		child->SetHasShadowAll(b);
	}

}

GameObject* GameObject::CreateChild(string name)
{
	GameObject* newObj = new GameObject(pScene);
	newObj->Awake();
	this->AddChild(newObj);
	newObj->SetName(name);

	return newObj;
}
GameObject* GameObject::LoadChild(string name)
{
	GameObject* newObj = new GameObject(pScene);
	newObj->Load();
	this->AddChild(newObj);
	newObj->SetName(name);

	return newObj;
}

GameObject* GameObject::CreateChild(void)
{
	return CreateChild("GameObject");
}

GameObject* GameObject::LoadChild(void)
{
	return LoadChild("GameObject");
}

void GameObject::RemoveChild(GameObject* child)
{
	childList.remove(child);
}

void GameObject::Unparent(void)
{
	if (!parent)
		return;

	this->parent->RemoveChild(this);

	this->parent = nullptr;
}

GameObject* GameObject::CreateChildByTypeName(string typeName)
{

	GameObject* newObj=pProjectSetting->CreateGameObjectByTypeName(typeName, pScene);
	AddChild(newObj);
	return newObj;

}

GameObject* GameObject::DynamicCreateChildByTypeName(string typeName)
{
	GameObject* newObj = pProjectSetting->DynamicCreateGameObjectByTypeName(typeName, pScene);
	AddChild(newObj);
	return newObj;

}

void GameObject::AddChild(GameObject* child)
{
	if (child == nullptr)
	{
		return;
	}

	childList.push_back(child);
	child->SetParent(this);

}


void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;

	if (parent == nullptr)
	{
		pScene->AddGameObject(this);
	}
	else
	{
		pScene->RemoveGameObject(this);
	}

}



void GameObject::LoadFbxFileMesh(string fName)
{
	MeshData* root = pScene->GetGameEngine()->GetAssetsManager()->LoadMeshFileFbx(fName);
	LoadMeshNode(root);
}


void GameObject::LoadFbxFileSkinMesh(string fName)
{
	SkinMeshTreeData* root = pGameEngine->GetAssetsManager()->LoadSkinMeshFileFbx(fName);
	SkinMeshLinkerComponent* linker = AddComponent<SkinMeshLinkerComponent>();
	linker->SetBoneCount(root->GetBoneCnt());


	for (SkinMeshTreeNode* childData:root->GetNodeArray())
	{
		CreateChild(childData->GetName())->LoadSkinMeshNode(childData, linker);

	}


}

void GameObject::DynamicLoadFbxFileMesh(string fName)
{
	MeshData* root = pScene->GetGameEngine()->GetAssetsManager()->LoadMeshFileFbx(fName);
	DynamicLoadMeshNode(root);

}

void GameObject::DynamicLoadFbxFileSkinMesh(string fName)
{
	SkinMeshTreeData* root = pGameEngine->GetAssetsManager()->LoadSkinMeshFileFbx(fName);
	SkinMeshLinkerComponent* linker = AddComponent<SkinMeshLinkerComponent>();
	linker->SetBoneCount(root->GetBoneCnt());
	linker->Init();

	for (SkinMeshTreeNode* childData : root->GetNodeArray())
	{
		CreateChild(childData->GetName())->DynamicLoadSkinMeshNode(childData, linker);

	}

}

void GameObject::LoadMeshNode(MeshData* node)
{
	if (!node->GetIsRoot())
	{
		MeshComponent* mesh = this->AddComponent<MeshComponent>();
		mesh->SetMeshData(node);

	}


	for (MeshData* childData : node->GetChild())
	{
		CreateChild(childData->GetName())->LoadMeshNode(childData);
	}


}

void GameObject::LoadSkinMeshNode(SkinMeshTreeNode* node, SkinMeshLinkerComponent* linker)
{
	if (node->GetAttribute() == SkinMeshTreeNode::Attribute::Mesh)
	{

		SkinMeshData* data = dynamic_cast<SkinMeshData*>(node);
		SkinMeshComponent* skinmesh = AddComponent<SkinMeshComponent>();
		skinmesh->SetSkinMeshData(data, linker);

		for (SkinMeshTreeNode* childData : node->GetChildArray())
		{
			CreateChild(childData->GetName())->LoadSkinMeshNode(childData, linker);
		}

	}
	else if (node->GetAttribute() == SkinMeshTreeNode::Attribute::Bone)
	{
		BoneData* data = dynamic_cast<BoneData*>(node);
		BoneComponent* bone = AddComponent<BoneComponent>();
		bone->SetBone(data, linker);


		for (SkinMeshTreeNode* childData : node->GetChildArray())
		{
			GameObject* chi = CreateChild(childData->GetName());
			chi->LoadSkinMeshNode(childData, linker);
			bone->CreateChild(chi);
			
		}

	}
	else if (node->GetAttribute() == SkinMeshTreeNode::Attribute::Null)
	{

		for (SkinMeshTreeNode* childData : node->GetChildArray())
		{
			CreateChild(childData->GetName())->LoadSkinMeshNode(childData, linker);
		}

	}



}

void GameObject::DynamicLoadMeshNode(MeshData* node)
{
	if (!node->GetIsRoot())
	{
		MeshComponent* mesh = this->AddComponent<MeshComponent>();
		mesh->SetMeshData(node);
		
	}


	for (MeshData* childData : node->GetChild())
	{
		CreateChild(childData->GetName())->DynamicLoadMeshNode(childData);
	}

	InitAllComponent();
}

void GameObject::DynamicLoadSkinMeshNode(SkinMeshTreeNode* node, SkinMeshLinkerComponent* linker)
{
	if (node->GetAttribute() == SkinMeshTreeNode::Attribute::Mesh)
	{

		SkinMeshData* data = dynamic_cast<SkinMeshData*>(node);
		SkinMeshComponent* skinmesh = AddComponent<SkinMeshComponent>();
		skinmesh->SetSkinMeshData(data, linker);

		for (SkinMeshTreeNode* childData : node->GetChildArray())
		{
			CreateChild(childData->GetName())->DynamicLoadSkinMeshNode(childData, linker);
		}

	}
	else if (node->GetAttribute() == SkinMeshTreeNode::Attribute::Bone)
	{
		BoneData* data = dynamic_cast<BoneData*>(node);
		BoneComponent* bone = AddComponent<BoneComponent>();
		bone->SetBone(data, linker);


		for (SkinMeshTreeNode* childData : node->GetChildArray())
		{
			GameObject* chi = CreateChild(childData->GetName());
			chi->DynamicLoadSkinMeshNode(childData, linker);
			bone->CreateChild(chi);
			
		}

	}
	else if (node->GetAttribute() == SkinMeshTreeNode::Attribute::Null)
	{

		for (SkinMeshTreeNode* childData : node->GetChildArray())
		{
			CreateChild(childData->GetName())->DynamicLoadSkinMeshNode(childData, linker);
		}

	}


	InitAllComponent();

}

void GameObject::SetNotAnim(BOOL b)
{
	notAnim = b;

	for (GameObject* child:childList)
	{
		child->SetNotAnim(b);
	}
}

BOOL GameObject::GetNotAnim(void)
{
	return notAnim;
}

void GameObject::SetName(string name, int count)
{


	if (parent)
	{
		if (parent->GetChild().empty())
		{
			Object::SetName(name,0);
			return;
		}


		// 名前が重複しないことを確認
		for (GameObject* gameObject : parent->GetChild())
		{

			if (count == 0)
			{
				if (gameObject == this && gameObject->GetName() == name)
				{
					Object::SetName(name, count);
					return;

				}


				if (gameObject->GetName() == name)
				{
					SetName(name, count + 1);
					return;
				}

			}
			else
			{

				if (gameObject == this && gameObject->GetName() == name + "(" + to_string(count) + ")")
				{
					Object::SetName(name, count);
					return;
				}

				if (gameObject->GetName() == (name + "(" + to_string(count) + ")"))
				{
					SetName(name, count + 1);
					return;
				}

			}

		}

		// 名前を設定
		Object::SetName(name,count);
		return;
	}
	else
	{
		if (pScene->GetGameObject().empty())
		{
			Object::SetName(name,0);
			return;
		}


		// 名前が重複しないことを確認
		for (GameObject* gameObject : pScene->GetGameObject())
		{


			if (count == 0)
			{
				if (gameObject == this && gameObject->GetName() == name)
				{
					Object::SetName(name, count);
					return;
				}

				if (gameObject->GetName() == name)
				{
					SetName(name, count + 1);
					return;
				}

			}
			else
			{
				if (gameObject == this && gameObject->GetName() == name + "(" + to_string(count) + ")")
				{
					Object::SetName(name, count);
					return;
				}

				if (gameObject->GetName() == name + "(" + to_string(count) + ")")
				{
					SetName(name, count + 1);
					return;
				}

			}
		}
		// 名前を設定
		Object::SetName(name, count);
		return;
	}


}

void GameObject::SetName(string name)
{
	SetName(name, 0);
}

void GameObject::SetID(void)
{
	this->ID = pScene->GetNotUseId();
}

void GameObject::SetID(unsigned long id)
{
	this->ID = id;
}




