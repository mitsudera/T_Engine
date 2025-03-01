#pragma once
#include "Coreminimal.h"

class GameEngine;
class Renderer;
class Scene;
class SceneManager;
class CameraComponent;
class LightManager;
class CollisionManager;
class ShadowMap;
class GUI;
class EditerCamera;
class GameObject;

class World
{
public:
	World(GameEngine* gameEngine);
	~World();

	void FixedUpdate();
	void FixedLateUpdate();
	void Update();
	void LateUpdate();
	void Draw();

	void LoadScene(Scene* scene);
	void UnloadScene(Scene* scene);

	GameEngine* GetGameEngine(void);
	CollisionManager* GetCollisionManager(void);
	LightManager* GetLightmanager(void);
	ShadowMap* GetShadowMap(void);

	list<Scene*>& GetActiveSceneList(void);
	SceneManager* GetSceneManager(void);

	void SetMainCamera(CameraComponent* camera);

	CameraComponent* GetMainCamera(void);

	EditerCamera* GetEditerCamera(void);

	void TestPlay(void);

private:
	
	GameEngine* pGameEngine;
	Renderer* pRenderer;
	list<Scene*> activeSceneList;
	SceneManager* sceneManager;
	CameraComponent* mainCamera;//バックバッファへの描画を行うカメラ
	LightManager* lightManager;
	CollisionManager* collisionManager;
	ShadowMap* shadowMap;
	GUI* gui;
	EditerCamera* editerCamera;


	BOOL notDtime;
	BOOL drawSkip;


};

