#include "GameEngine.h"
#include "Main.h"
#include "renderer.h"
#include "AssetsManager.h"
#include "input.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CBufferManager.h"
#include "LightManager.h"
#include "CollisionManager.h"
#include "CameraComponent.h"
#include "ShadowMap.h"
#include "SoundEngine.h"
#include "WicFactory.h"
#include "DebugUtility.h"
#include "World.h"
#include "ProjectSetting.h"
#include "SaveSystem.h"

GameEngine::GameEngine(Main* main)
{
	this->main = main;
	accumulatedTime = 0.0f;
	fixedDeltaTimeRate = 60.0f;
	fixedDeltaTime = 1.0f / fixedDeltaTimeRate;

	editerMode = TRUE;
	testMode = FALSE;
}

GameEngine::~GameEngine()
{
}

void GameEngine::Awake()
{
	
	this->windowSize.x = screenWidth;
	this->windowSize.y = screenHeight;

	this->saveSystem = new SaveSystem(this);

	this->projectSetting = new ProjectSetting(this);

	this->renderer = new Renderer(this);
	this->renderer->InitRenderer(*main->GetInstanceHandle(), main->GetWindowHangle(), true);


	this->soundEngine = new SoundEngine(this);

	this->cBufferManager = new CBufferManager(this);



	this->assetsManager = new AssetsManager(this);
	this->assetsManager->Awake();



	this->wicFactory = new WicFactory(this);

	this->input = new Input();
	this->input->Awake(*main->GetInstanceHandle(), main->GetWindowHangle());


	this->debugUtility = new DebugUtility(this);
	
	this->world = new World(this);


	SetFullScreen(TRUE);


}

void GameEngine::Update()
{
	deltaTime = main->GetDeltaTime();


	accumulatedTime += deltaTime;
	while (accumulatedTime >= fixedDeltaTime)
	{
		this->FixedUpdate();
		accumulatedTime -= fixedDeltaTime;
	}


	this->mouseDeltaX = input->GetMouseX() - oldMousePosX;
	this->mouseDeltaY = input->GetMouseY() - oldMousePosY;


	this->oldMousePosX = input->GetMouseX();
	this->oldMousePosY = input->GetMouseY();

	this->input->Update();
	this->world->Update();


	this->LateUpdate();





}

void GameEngine::FixedUpdate()
{
	world->FixedUpdate();
	world->FixedLateUpdate();
}

void GameEngine::LateUpdate()
{
	world->LateUpdate();

}

void GameEngine::Draw()
{

	renderer->Clear();

	world->Draw();

	renderer->Present();

}

void GameEngine::Uninit()
{

	delete world;

	this->input->Uninit();
	delete input;

	this->assetsManager->Uninit();
	delete assetsManager;



	delete debugUtility;

	delete cBufferManager;


	delete wicFactory;

	delete soundEngine;

	this->renderer->UninitRenderer();
	delete renderer;

}

long GameEngine::GetMouseMoveX(void)
{
	return mouseDeltaX;
}

long GameEngine::GetMouseMoveY(void)
{
	return mouseDeltaY;
}

float GameEngine::GetDeltaTime(void)
{
	return deltaTime;
}

XMFLOAT2 GameEngine::GetWindowSize(void)
{
	return this->windowSize;
}

AssetsManager* GameEngine::GetAssetsManager(void)
{
	return this->assetsManager;
}

Renderer* GameEngine::GetRenderer(void)
{
	return this->renderer;
}

CBufferManager* GameEngine::GetCBufferManager(void)
{
	return this->cBufferManager;
}

ProjectSetting* GameEngine::GetProjectSetting(void)
{
	return this->projectSetting;
}

World* GameEngine::GetWorld(void)
{
	return world;
}

SaveSystem* GameEngine::GetSaveSystem(void)
{
	return this->saveSystem;
}

Input* GameEngine::GetInput(void)
{
	return this->input;
}


SoundEngine* GameEngine::GetSoundEngine(void)
{
	return this->soundEngine;
}

WicFactory* GameEngine::GetWicFactory(void)
{
	return this->wicFactory;
}

DebugUtility* GameEngine::GetDebugUtility(void)
{
	return this->debugUtility;
}




void GameEngine::SetFullScreen(BOOL flag)
{
	if (flag == fullscreen)
		return;

	fullscreen = flag;
	main->ToggleFullScreen();

}

void GameEngine::ChengeWindowSize(int width, int height)
{
	this->windowSize.x = (float)width;
	this->windowSize.y = (float)height;
	if (fullscreen)
		return;

	main->ChengeWindowSize(width, height);
}


HWND GameEngine::GetWindowHandle(void)
{
	return main->GetWindowHangle();
}

void GameEngine::Exit(void)
{
	this->main->Exit();
}

void GameEngine::SetNextScene(Scene* scene)
{
	this->nextScene = scene;
}


float GameEngine::GetFixedDeltaTime(void)
{
	return fixedDeltaTime;
}

float GameEngine::GetFixedDeltaTimeRate(void)
{
	return fixedDeltaTimeRate;
}

BOOL GameEngine::GetEditerMode(void)
{
	return editerMode;
}

void GameEngine::SetEditerMode(BOOL b)
{
	editerMode = b;
}

void GameEngine::TestPlay(void)
{
	testMode = TRUE;
	world->TestPlay();

}

BOOL GameEngine::GetTestMode(void)
{
	return testMode;
}



