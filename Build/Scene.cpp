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

void Scene::Awake()
{
	sceneTime = 0.0f;
	type = Type::Scene;
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
	camera->GetComponent<CameraComponent>()->SetSky(GetGameObject<SkySphere>());
	AddGameObject(camera);

	pWorld->SetMainCamera(camera->GetComponent<CameraComponent>());

}

GameObject* Scene::DynamicCreateGameObjectByTypeName(string typeName)
{
	GameObject* newObj= pProjectSetting->DynamicCreateGameObjectByTypeName(typeName,this);
	AddGameObject(newObj);
	return newObj;
}

GameObject* Scene::CreateGameObjectByTypeName(string typeName)
{
	GameObject* newObj = pProjectSetting->CreateGameObjectByTypeName(typeName,this);
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
	gameObjectList.remove(gameObject);

	gameObject->Destroy();
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

void Scene::SetName(string name, int count)
{
	if (name.empty())
	{
		name = "Scene";
	}

	if (pSceneManager->GetSceneArray().empty())
	{
		Object::SetName(name,0);
		return;
	}
	// 名前が重複しないことを確認
	for (Scene* scene : pSceneManager->GetSceneArray())
	{
		if (count == 0)
		{
			if (scene->GetName() == name)
			{
				SetName(name, count + 1);
				return;
			}

		}
		else
		{
			if (scene->GetName() == name + "(" + to_string(count) + ")")
			{
				SetName(name, count + 1);
				return;
			}

		}

	}

	Object::SetName(name, count);
	return;
}

void Scene::SetName(string name)
{
	SetName(name, 0);
}
