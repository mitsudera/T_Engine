#include "SceneManager.h"
#include "Scene.h"
#include "GameEngine.h"
#include "SoundEngine.h"
#include "World.h"
#include "EditerCamera.h"
#include "Camera.h"
#include "CameraComponent.h"
SceneManager::SceneManager(World* world)
{
	this->pWorld = world;
	this->pGameEngine = world->GetGameEngine();

}

SceneManager::~SceneManager()
{
	for (Scene* scene :sceneArray)
	{
		delete scene;
	}
}

void SceneManager::Update(void)
{
	for (Scene* unloadScene : unloadSceneArray)
	{
		pWorld->UnloadScene(unloadScene);
	}

	unloadSceneArray.clear();

	for (Scene* loadScene : loadSceneArray)
	{
		pWorld->LoadScene(loadScene);
	}

	loadSceneArray.clear();
}

Scene* SceneManager::CreateScene(void)
{

	return CreateScene("noname");
}

Scene* SceneManager::CreateScene(string name)
{
	Scene* scene = new Scene(pWorld);
	scene->SetName(name);
	sceneArray.push_back(scene);
	UnloadScene();
	LoadScene(name);

	scene->CreateDefaultObject();



	return scene;

}



void SceneManager::LoadScene(string name)
{
	for (Scene* scene:sceneArray)
	{
		if (scene->GetName() == name)
		{
			loadSceneArray.push_back(scene);
			return;
		}
		
	}
}

void SceneManager::UnloadScene(string name)
{
	for (Scene* scene : pWorld->GetActiveSceneList())
	{
		if (scene->GetName() == name)
		{
			unloadSceneArray.push_back(scene);
			return;
		}

	}

}

void SceneManager::UnloadScene(void)
{
	for (Scene* scene : pWorld->GetActiveSceneList())
	{
		unloadSceneArray.push_back(scene);

	}

}

vector<Scene*>& SceneManager::GetSceneArray(void)
{
	return this->sceneArray;
}


