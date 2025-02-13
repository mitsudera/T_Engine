#pragma once
#include "Coreminimal.h"
#include "component.h"
#include "ShaderSet.h"
#include "Object.h"



class ColliderComponent;
class Scene;
class TransformComponent;
class Component;
class MeshData;
class GameEngine;
class ColliderComponent;
class SkinMeshData;
class BoneData;
class SkinMeshLinkerComponent;
class SkinMeshTreeNode;
class World;
class ProjectSetting;

class GameObject:public Object
{
public:
	static const string TypeName;


	enum class ObjectTag:int
	{
		Default,
		Player,
		Enemy,
		PlayerAttack,
		EnemyAttack,
		Field,

		ObjectTagMax,
	};


	enum class Layer:int
	{
		Default,
		Sky,
		Field,
		Water,
		UI,
		Text,

		LayerMax,
	};

	GameObject();
	GameObject(Scene* scene);
	//GameObject(GameObject* parent);
	~GameObject();

	virtual void Awake(void);

	void InitAllComponent(void);

	void Destroy(void);
	
	void DeleteChild(GameObject* gameObject);
	void DeleteComponnt(Component* com);

	Scene* GetScene(void);
	World* GetWorld(void);
	GameEngine* GetGameEngine(void);

	TransformComponent* GetTransFormComponent(void);

	void SetTag(ObjectTag tag);
	ObjectTag GetTag(void);
	void SetLayer(Layer layer);
	Layer GetLayer(void);

	BOOL GetActive(void);
	void SetActive(BOOL isActive);

	GameObject* GetParent(void);
	GameObject* GetRootObject(void);

	GameObject* GetChild(int index);
	GameObject* GetChild(string name);
	list<GameObject*>& GetChild();
	GameObject* SerchAllChild(string name);

	list<Component*>& GetComponentList(void);

	//指定したアトリビュートを持つコンポーネントの中でn番目のコンポーネントのポインタを取得
	Component* GetComponentAttrbute(Component::Attribute attr, int n);

	template<class T>
	T* GetComponent(void);

	template<class T>
	T* AddComponent(void);

	template<class T>
	T* DynamicAddComponent(void);

	Component* AddComponentByTypeName(string typeName);
	Component* DynamicAddComponentByTypeName(string typeName);


	void SetHasShadowAll(BOOL b);

	GameObject* CreateChild(string name);
	GameObject* CreateChild(void);

	GameObject* CreateChildByTypeName(string typeName);
	GameObject* DynamicCreateChildByTypeName(string typeName);

	void AddChild(GameObject* child);

	void SetParent(GameObject* parent);


	void LoadFbxFileMesh(string fName);

	void LoadFbxFileSkinMesh(string fName);

	void DynamicLoadFbxFileMesh(string fName);

	void DynamicLoadFbxFileSkinMesh(string fName);


	void LoadMeshNode(MeshData* node);


	void LoadSkinMeshNode(SkinMeshTreeNode* node,SkinMeshLinkerComponent* linker);

	void DynamicLoadMeshNode(MeshData* node);

	void DynamicLoadSkinMeshNode(SkinMeshTreeNode* node, SkinMeshLinkerComponent* linker);


	void SetNotAnim(BOOL b);
	BOOL GetNotAnim(void);

	virtual void SetName(string name, int count) override;
	virtual void SetName(string name) override;

	void SetID(void);
	void SetID(unsigned long id);

protected:
	Scene* pScene;
	World* pWorld;
	GameEngine* pGameEngine;
	ProjectSetting* pProjectSetting;
	TransformComponent* transformComponent;

	list<Component*> componentList;

	ObjectTag tag;
	Layer layer;

	BOOL notAnim;

	BOOL isActive;

	GameObject* parent;
	list <GameObject*> childList;

};



template<class T>
T* GameObject::GetComponent(void)
{
	for (Component* com : componentList) {
		T* buff = dynamic_cast<T*>(com);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;

}

template<class T>
T* GameObject::AddComponent(void)
{
	T* com = new T(this);

	this->componentList.push_back(com);

	Component* c = dynamic_cast<Component*>(com);
	c->Awake();
	return com;
}

template<class T>
T* GameObject::DynamicAddComponent(void)
{
	T* com = new T(this);

	this->componentList.push_back(com);

	Component* c = dynamic_cast<Component*>(com);
	c->Awake();
	c->Init();
	return com;
}
