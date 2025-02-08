#pragma once
#include "gameobject.h"

class SceneManager;
class GameEngine;
class CollisionManager;
class CameraComponent;
class PrimitiveComponent;
class RigidBodyComponent;
class World;
class ProjectSetting;

class Scene:public Object
{
public:

	Scene();
	Scene(World* world);
	~Scene();


	virtual void Awake();

	void Init(void);

	void FixedUpdate();
	void FixedLateUpdate();
	void Update();
	void LateUpdate();
	void Draw();
	void Uninit();

	void CreateDefaultObject(void);

	GameObject* DynamicCreateGameObjectByTypeName(string typeName);
	GameObject* CreateGameObjectByTypeName(string typeName);

	void AddGameObject(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject);
	void DeleteGameObject(GameObject* gameObject);

	World* GetWorld(void);

	GameEngine* GetGameEngine(void);

	list <GameObject*>& GetGameObject(void);

	GameObject* GetGameObjectName(string name);

	void AddCamera(CameraComponent* camera);

	float GetSceneTime(void);


	template<class T>
	T* GetGameObject(void);

	GameObject* CreateDefaultGameObject(string type);

	list<Component*>& GetAllComponent(void);
	list<TransformComponent*>& GetAllTransformComponent(void);
	list<PrimitiveComponent*>& GetAllPrimitiveComponent(void);
	list<RigidBodyComponent*>& GetAllRigidBodyComponent(void);

	void AddSceneComponent(Component* com);
	void AddSceneTransformComponent(TransformComponent* com);
	void AddScenePrimitiveComponent(PrimitiveComponent* com);
	void AddSceneRigidBodyComponent(RigidBodyComponent* com);

	void RemoveSceneComponent(Component* com);
	void RemoveSceneTransformComponent(TransformComponent* com);
	void RemoveScenePrimitiveComponent(PrimitiveComponent* com);
	void RemoveSceneRigidBodyComponent(RigidBodyComponent* com);

	virtual void SetName(string name, int count) override;
	virtual void SetName(string name) override;

protected:
	GameEngine* pGameEngine;
	World* pWorld;
	SceneManager* pSceneManager;
	ProjectSetting* pProjectSetting;
	list <GameObject*> gameObjectList;
	list <CameraComponent*> cameraList;
	float sceneTime;
	list<Component*> allComponent;
	list<TransformComponent*> allTransformComponent;
	list<PrimitiveComponent*> allPrimitiveComponent;
	list<RigidBodyComponent*> allRigidBodyComponent;

};



template<class T>
T* Scene::GetGameObject(void)
{
	for (GameObject* obj : gameObjectList) 
	{
		T* buff = dynamic_cast<T*>(obj);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;

}

