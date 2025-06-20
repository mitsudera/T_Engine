#include "Scene.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "CameraComponent.h"
#include "component.h"
#include "transformcomponent.h"
#include "SoundEngine.h"
#include "World.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "SkySphere.h"
#include "SceneManager.h"
#include "ProjectSetting.h"
#include "SaveSystem.h"
#include "SceneAssetsData.h"

Scene::Scene()
{
}

Scene::Scene(World* world)
{
	this->pWorld = world;
	this->pGameEngine = world->GetGameEngine();
	this->pSceneManager = world->GetSceneManager();
	this->pProjectSetting = pGameEngine->GetProjectSetting();
}

Scene::~Scene()
{

}

void Scene::LoadSave(string fileName)
{
}

void Scene::Awake()
{
	sceneTime = 0.0f;
	type = Type::Scene;
	idCnt = 0;
}

void Scene::Init(void)
{
	for (Component* com : allComponent)
	{

		com->Init();

	}

}

void Scene::FixedUpdate()
{
	for (Component* com : allComponent)
	{
		if (!com->GetActive())
			continue;

		com->FixedUpdate();

	}

	for (TransformComponent* com : allTransformComponent)
	{
		if (!com->GetActive())
			continue;

		com->UpdateMatrix();

	}



}

void Scene::FixedLateUpdate()
{
	for (Component* com : allComponent)
	{
		if (!com->GetActive())
			continue;

		com->FixedLateUpdate();

	}

	for (TransformComponent* com : allTransformComponent)
	{
		if (!com->GetActive())
			continue;

		com->UpdateMatrix();

	}

}

void Scene::Update()
{

	for (Component* com :allComponent)
	{
		if (!com->GetActive())
			continue;
		if (com == nullptr)
			continue;
		
		com->Update();

	}

	for (TransformComponent* com :allTransformComponent)
	{
		if (!com->GetActive())
			continue;

		com->UpdateMatrix();

	}


	sceneTime += pGameEngine->GetDeltaTime();
}

void Scene::LateUpdate()
{
	for (Component* com : allComponent)
	{
		if (!com->GetActive())
			continue;

		com->LateUpdate();

	}

	for (TransformComponent* com : allTransformComponent)
	{
		if (!com->GetActive())
			continue;

		com->UpdateMatrix();

	}

}

void Scene::Draw()
{

	for (CameraComponent* camera:cameraList)
	{
		if (!camera->GetActive())
			continue;

		camera->Render();
	}
}

void Scene::Uninit()
{
	this->pGameEngine->GetSoundEngine()->StopAllSound();

	for (GameObject* gameObject : GetGameObject())
	{
		gameObject->Destroy();
		delete gameObject;
	}
	gameObjectList.clear();
	cameraList.clear();
	allComponent.clear();
	allTransformComponent.clear();
	allPrimitiveComponent.clear();
	allRigidBodyComponent.clear();
}

void Scene::CreateDefaultObject(void)
{

	AddGameObject(pProjectSetting->CreateGameObject<DirectionalLight>(this));

	SkySphere* sky = pProjectSetting->CreateGameObject<SkySphere>(this);
	sky->SetHasShadowAll(FALSE);
	AddGameObject(sky);


	GameObject* camera = pProjectSetting->CreateGameObject<Camera>(this);
	AddGameObject(camera);

	pWorld->SetMainCamera(camera->GetComponent<CameraComponent>());

}

GameObject* Scene::DynamicCreateGameObjectByTypeName(string typeName)
{
	GameObject* newObj= pProjectSetting->DynamicCreateGameObjectByTypeName(typeName,this);
	AddGameObject(newObj);
	return newObj;
}

GameObject* Scene::DynamicCroneGameObject(GameObject* gameObject)
{
	GameObject* newObj = pProjectSetting->DynamicCroneGameObject(gameObject);
	AddGameObject(newObj);
	return newObj;
}

GameObject* Scene::CreateGameObjectByTypeName(string typeName)
{
	GameObject* newObj = pProjectSetting->CreateGameObjectByTypeName(typeName,this);
	AddGameObject(newObj);
	return newObj;
}

GameObject* Scene::CreateGameObject(void)
{
	GameObject* newObj = pProjectSetting->CreateGameObject<GameObject>(this);
	AddGameObject(newObj);
	return newObj;
}

GameObject* Scene::LoadGameObject(void)
{
	GameObject* newObj = pProjectSetting->CreateGameObject<GameObject>(this);
	AddGameObject(newObj);
	return newObj;
}

void Scene::AddGameObject(GameObject* gameObject)
{
	for (GameObject* obj : gameObjectList)
	{
		if (obj == gameObject)
		{
			return;

		}
		
	}

	gameObject->SetName(gameObject->GetName());
	gameObjectList.push_back(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	gameObjectList.remove(gameObject);
}

void Scene::DeleteGameObject(GameObject* gameObject)
{
	gameObject->Unparent();
	gameObjectList.remove(gameObject);
	allGameObjectList.remove(gameObject);
	gameObject->Destroy();
	delete gameObject;

}

void Scene::DynamicDeleteGameObject(GameObject* gameObject)
{
	gameObject->Unparent();
	gameObject->DynamicDestroy();
	delete gameObject;

}


World* Scene::GetWorld(void)
{
	return this->pWorld;
}

GameEngine* Scene::GetGameEngine(void)
{
	return this->pGameEngine;
}

list<GameObject*>& Scene::GetGameObject(void)
{
	return this->gameObjectList;
}

list<GameObject*>& Scene::GetAllGameObject(void)
{
	return allGameObjectList;
}

void Scene::AddAllGameObject(GameObject* gameObject)
{
	allGameObjectList.push_back(gameObject);
}

GameObject* Scene::GetGameObjectName(string name)
{
	for (GameObject* object:gameObjectList)
	{
		if (object->GetName() == name)
		{
			return object;

		}

	}
	return nullptr;
}

void Scene::AddCamera(CameraComponent* camera)
{

	this->cameraList.push_back(camera);
}

float Scene::GetSceneTime(void)
{
	return sceneTime;
}



GameObject* Scene::GetGameObject(unsigned long id)
{
	for (GameObject* gameObject : allGameObjectList)
	{
		if (gameObject->GetID() == id)
		{
			return gameObject;

		}

	}

	return nullptr;
}

GameObject* Scene::CreateDefaultGameObject(string type)
{
	return nullptr;
}

list<Component*>& Scene::GetAllComponent(void)
{
	return allComponent;
}

list<TransformComponent*>& Scene::GetAllTransformComponent(void)
{
	return allTransformComponent;
}

list<PrimitiveComponent*>& Scene::GetAllPrimitiveComponent(void)
{
	return allPrimitiveComponent;
}

list<RigidBodyComponent*>& Scene::GetAllRigidBodyComponent(void)
{
	return allRigidBodyComponent;
}

void Scene::AddSceneComponent(Component* com)
{
	this->allComponent.push_back(com);
}

void Scene::AddSceneTransformComponent(TransformComponent* com)
{
	this->allTransformComponent.push_back(com);
}

void Scene::AddScenePrimitiveComponent(PrimitiveComponent* com)
{
	this->allPrimitiveComponent.push_back(com);
}

void Scene::AddSceneRigidBodyComponent(RigidBodyComponent* com)
{
	this->allRigidBodyComponent.push_back(com);
}

void Scene::RemoveSceneComponent(Component* com)
{
	allComponent.remove(com);
}

void Scene::RemoveSceneTransformComponent(TransformComponent* com)
{
	allTransformComponent.remove(com);
}

void Scene::RemoveScenePrimitiveComponent(PrimitiveComponent* com)
{
	allPrimitiveComponent.remove(com);
}

void Scene::RemoveSceneRigidBodyComponent(RigidBodyComponent* com)
{
	allRigidBodyComponent.remove(com);
}

void Scene::RemoveCameraList(CameraComponent* com)
{
	cameraList.remove(com);
}


void Scene::SetName(string name)
{
	sceneAssets->SetName(name);
	Object::SetName(sceneAssets->GetName());
}

string Scene::GetName(void)
{
	return sceneAssets->GetName();
}

unsigned long Scene::GetNotUseId(void)
{
	if (notUseIDList.empty())
	{
		idCnt++;

		return idCnt;
	}
	else
	{
		unsigned long id = notUseIDList.front();
		notUseIDList.pop_front();
		return id;
	}
}

list<unsigned long>& Scene::GetNotUseIDList(void)
{
	return notUseIDList;
}

void Scene::SetNotUseID(unsigned long id)
{
	notUseIDList.push_back(id);
}

void Scene::SetIDCnt(unsigned long idCnt)
{
	this->idCnt = idCnt;
}

unsigned long Scene::GetIDCnt(void)
{
	return idCnt;
}

SceneAssetsData* Scene::GetSceneAssetsData(void)
{
	return this->sceneAssets;
}

void Scene::SetSceneAssetsData(SceneAssetsData* data)
{
	this->sceneAssets = data;
}

void Scene::RemoveAllGameObjectList(GameObject* gameObject)
{
	allGameObjectList.remove(gameObject);
}


