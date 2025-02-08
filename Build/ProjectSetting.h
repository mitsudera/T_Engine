#pragma once
#include "Coreminimal.h"
class GameEngine;
class GameObject;
class Scene;
class ProjectSetting
{
public:
	ProjectSetting(GameEngine* gameEngine);
	~ProjectSetting();

	vector<string>& GetDefaultGameObjectTypeNameArray(void);
	vector<string>& GetComponentNameArray(void);

	GameObject* CreateGameObjectByTypeName(string typeName, Scene* scene);

	GameObject* DynamicCreateGameObjectByTypeName(string typeName, Scene* scene);

	template<class T>
	T* CreateGameObject(Scene* scene);

	template<class T>
	T* DynamicCreateGameObject(Scene* scene);

	template<class T>
	T* CreateGameObject(string name, Scene* scene);

	template<class T>
	T* DynamicCreateGameObject(string name, Scene* scene);




private:
	GameEngine* pGameEngine;
	vector<string> objectTagArray;
	vector<string> layerArray;
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


