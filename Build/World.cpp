#include "World.h"
#include "GameEngine.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "Renderer.h"
#include "LightManager.h"
#include "CollisionManager.h"
#include "ShadowMap.h"
#include "GUI.h"
#include "EditerCamera.h"
#include "input.h"
World::World(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;

	this->collisionManager = new CollisionManager(this);
	this->lightManager = new LightManager(this);
	lightManager->Awake();
	this->shadowMap = new ShadowMap(this);
	this->shadowMap->CreateShadowMap(ShadowMap::ShadowQuality::High);
	this->sceneManager = new SceneManager(this);

	if (pGameEngine->GetEditerMode())
	{
		this->editerCamera = new EditerCamera(this);
		this->gui = new GUI(this);

	}
	showGui = TRUE;
}

World::~World()
{
	for (Scene* scene : activeSceneList)
	{
		scene->Uninit();
		delete scene;
	}
	delete sceneManager;

	delete lightManager;
	delete shadowMap;
	delete collisionManager;
	if (pGameEngine->GetEditerMode())
	{
		delete gui;
		delete editerCamera;
	}

}


void World::FixedUpdate()
{

	for (Scene* scene:activeSceneList)
	{
		scene->FixedUpdate();
	}

	this->collisionManager->Update();



}

void World::FixedLateUpdate()
{

	for (Scene* scene : activeSceneList)
	{
		scene->FixedLateUpdate();
	}


}

void World::Update()
{
	sceneManager->Update();
	for (Scene* scene : activeSceneList)
	{
		scene->Update();
	}
	
	if (pGameEngine->GetInput()->GetKeyboardTrigger(DIK_H))
	{
		HideGui();
	}
	else if(pGameEngine->GetInput()->GetKeyboardTrigger(DIK_G))
	{
		ShowGui();
	}
}

void World::LateUpdate()
{
	for (Scene* scene : activeSceneList)
	{
		scene->LateUpdate();
	}
	collisionManager->Update();
	lightManager->Update();

	if (pGameEngine->GetEditerMode())
	{
		editerCamera->Update();


		if (showGui)
		{
			gui->Update();

		}
	}

}

void World::Draw()
{
	if (activeSceneList.empty())
		return;

	this->shadowMap->ShadowMapping();


	if (pGameEngine->GetEditerMode()==FALSE)
	{
		for (Scene* scene : activeSceneList)
		{
			scene->Draw();
		}



	}
	else
	{
		if (pGameEngine->GetTestMode())
		{
			for (Scene* scene : activeSceneList)
			{
				scene->Draw();
			}

		}
		else
		{
			editerCamera->Render();

		}

		if (showGui)
		{
			gui->Draw();

		}

	}
	



}

void World::LoadScene(Scene* scene)
{
	activeSceneList.push_back(scene);

	scene->Init();

}

void World::UnloadScene(Scene* scene)
{
	for (Scene* s : activeSceneList)
	{
		if (s != scene)
			continue;

		s->Uninit();
		delete s;
	}

}

GameEngine* World::GetGameEngine(void)
{
	return this->pGameEngine;
}


CollisionManager* World::GetCollisionManager(void)
{
	return this->collisionManager;
}

LightManager* World::GetLightmanager(void)
{
	return this->lightManager;
}

ShadowMap* World::GetShadowMap(void)
{
	return this->shadowMap;
}

list<Scene*>& World::GetActiveSceneList(void)
{
	return this->activeSceneList;
}

SceneManager* World::GetSceneManager(void)
{
	return this->sceneManager;
}

void World::SetMainCamera(CameraComponent* camera)
{
	this->mainCamera = camera;
}

CameraComponent* World::GetMainCamera(void)
{
	return this->mainCamera;
}

EditerCamera* World::GetEditerCamera(void)
{
	return this->editerCamera;
}

void World::TestPlay(void)
{
	for (Scene* scene : activeSceneList)
	{

		scene->Init();
	}

}

void World::ShowGui(void)
{
	showGui = TRUE;
}

void World::HideGui(void)
{
	showGui = FALSE;
}

BOOL World::GetShowGui(void)
{
	return showGui;
}

