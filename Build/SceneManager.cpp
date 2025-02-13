#include "SceneManager.h"
#include "Scene.h"
#include "GameEngine.h"
#include "SoundEngine.h"
#include "World.h"
#include "EditerCamera.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "SaveSystem.h"
#include "AssetsManager.h"
#include "SceneAssetsData.h"

SceneManager::SceneManager(World* world)
{
	this->pWorld = world;
	this->pGameEngine = world->GetGameEngine();
	this->pSaveSystem = pGameEngine->GetSaveSystem();
	this->pAssetsManager = pGameEngine->GetAssetsManager();
}

SceneManager::~SceneManager()
{
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

Scene* SceneManager::CreateNewScene(void)
{

	return CreateNewScene("noname");
}

Scene* SceneManager::CreateNewScene(string name)
{
	Scene* scene = new Scene(pWorld);
	scene->Awake();

	scene->SetSceneAssetsData(pGameEngine->GetAssetsManager()->CreateNewSceneAssets(name));

	scene->SetName(name);
	UnloadScene();
	loadSceneArray.push_back(scene);

	scene->CreateDefaultObject();



	return scene;

}

Scene* SceneManager::LoadScene(SceneAssetsData* data)
{
	Scene* scene = pSaveSystem->LoadScene(data);


	loadSceneArray.push_back(scene);




	return scene;
}

Scene* SceneManager::LoadScene(string name)
{

	for (SceneAssetsData* data : pAssetsManager->GetSceneAssetsDataList())
	{
		if (data->GetName() == name)
		{
			Scene* scene = pSaveSystem->LoadScene(data);
			loadSceneArray.push_back(scene);
			return scene;

		}
	}


	return nullptr;

}


Scene* SceneManager::OpenScene(string name)
{
	for (SceneAssetsData* data : pAssetsManager->GetSceneAssetsDataList())
	{
		if (data->GetName() == name)
		{
			Scene* scene = pSaveSystem->LoadScene(data);
			UnloadScene();
			loadSceneArray.push_back(scene);
			return scene;

		}
	}


	return nullptr;
}
Scene* SceneManager::OpenScene(SceneAssetsData* data)
{
	Scene* scene = pSaveSystem->LoadScene(data);

	UnloadScene();

	loadSceneArray.push_back(scene);




	return scene;
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

void SceneManager::UnloadScene(Scene* scene)
{
	for (Scene* ascene : pWorld->GetActiveSceneList())
	{
		if (ascene == scene)
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


