#pragma once
#include "Coreminimal.h"
class GameEngine;
class GameObject;
class Scene;
class Component;
class ProjectSetting
{
public:
	ProjectSetting(GameEngine* gameEngine);
	~ProjectSetting();

	void LoadDefaultSeting(void);

	vector<string>& GetDefaultGameObjectTypeNameArray(void);
	vector<string>& GetComponentNameArray(void);

	GameObject* CreateGameObjectByTypeName(string typeName, Scene* scene);

	GameObject* DynamicCreateGameObjectByTypeName(string typeName, Scene* scene);

	Component* AddComponentByTypeName(string typeName, GameObject* gameObject);
	Component* DynamicAddComponentByTypeName(string typeName, GameObject* gameObject);

	template<class T>
	T* CreateGameObject(Scene* scene);

	template<class T>
	T* DynamicCreateGameObject(Scene* scene);

	GameObject* DynamicCroneGameObject(GameObject* gameObject);

	GameObject* DynamicCroneGameObject(GameObject* gameObject, GameObject* parent);

	template<class T>
	T* LoadGameObject(Scene* scene);

	template<class T>
	T* DynamicLoadGameObject(Scene* scene);

	template<class T>
	T* CreateGameObject(string name, Scene* scene);

	template<class T>
	T* DynamicCreateGameObject(string name, Scene* scene);

	void SetProjectName(string name);
	string GetProjectName(void);

	void SetDefaultScene(string name);
	string GetDefaultScene(void);

	string* AddNewTag(string tag);
	string* AddNewLayer(string layer);

	string* GetTag(string tag);
	string* GetLayer(string layer);

	list<string*>& GetTagList(void);
	list<string*>& GetLayerList(void);

private:
	GameEngine* pGameEngine;
	string projectName = "project";
	string defaultScene="";
	list<string*> objectTagList;
	list<string*> layerList;
	vector<string> defaultGameObjectTypeNameArray;
	vector<string> componentTypeNameArray;
};

template<class T>
T* ProjectSetting::CreateGameObject(Scene* scene)
{
	T* obj = new T(scene);


	GameObject* o = dynamic_cast<GameObject*>(obj);
	o->Awake();

	return obj;
}

template<class T>
T* ProjectSetting::DynamicCreateGameObject(Scene* scene)
{
	T* obj = new T(scene);


	GameObject* o = dynamic_cast<GameObject*>(obj);
	o->Awake();

	o->InitAllComponent();
	return obj;
}

template<class T>
T* ProjectSetting::LoadGameObject(Scene* scene)
{
	T* obj = new T(scene);


	GameObject* o = dynamic_cast<GameObject*>(obj);
	o->Load();

	return obj;
}

template<class T>
T* ProjectSetting::DynamicLoadGameObject(Scene* scene)
{
	T* obj = new T(scene);


	GameObject* o = dynamic_cast<GameObject*>(obj);
	o->Load();

	o->InitAllComponent();
	return obj;
}


template<class T>
T* ProjectSetting::CreateGameObject(string name, Scene* scene)
{
	T* obj = new T(scene);


	GameObject* o = dynamic_cast<GameObject*>(obj);
	o->Awake();
	o->SetName(name);
	return obj;
}

template<class T>
T* ProjectSetting::DynamicCreateGameObject(string name, Scene* scene)
{
	T* obj = new T(scene);


	GameObject* o = dynamic_cast<GameObject*>(obj);
	o->Awake();

	o->InitAllComponent();

	o->SetName(name);

	return obj;
}


