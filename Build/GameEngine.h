#pragma once
#include "Coreminimal.h"


class AssetsManager;
class Renderer;
class Input;
class Main;
class CameraComponent;
class SceneManager;
class Scene;
class CBufferManager;
class LightManager;
class CollisionManager;
class ShadowMap;
class SoundEngine;
class WicFactory;
class DebugUtility;
class World;
class ProjectSetting;
class SaveSystem;

class GameEngine
{
public:
	GameEngine(Main* main);
	~GameEngine();

	void Awake();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Draw();
	void Uninit();

	long GetMouseMoveX(void);
	long GetMouseMoveY(void);

	// 前フレームからの経過ミリ秒を取得する
	float GetDeltaTime(void);

	XMFLOAT2 GetWindowSize(void);

	AssetsManager* GetAssetsManager(void);
	Renderer* GetRenderer(void);
	Input* GetInput(void);
	WicFactory* GetWicFactory(void);
	DebugUtility* GetDebugUtility(void);
	SoundEngine* GetSoundEngine(void);
	CBufferManager* GetCBufferManager(void);
	ProjectSetting* GetProjectSetting(void);
	World* GetWorld(void);
	SaveSystem* GetSaveSystem(void);

	void SetFullScreen(BOOL flag);

	void ChengeWindowSize(int width, int height);


	HWND GetWindowHandle(void);

	void Exit(void);

	void SetNextScene(Scene* scene);

	void SwichScene(void);

	float GetFixedDeltaTime(void);
	float GetFixedDeltaTimeRate(void);

	BOOL GetEditerMode(void);

	void SetEditerMode(BOOL b);

	void TestPlay(void);

	BOOL GetTestMode(void);
private:

	Main* main;
	AssetsManager* assetsManager;
	Input* input;
	SoundEngine* soundEngine;
	Renderer* renderer;
	WicFactory* wicFactory;
	DebugUtility* debugUtility;
	ProjectSetting* projectSetting;
	CBufferManager* cBufferManager;
	World* world;
	SaveSystem* saveSystem;

	long oldMousePosX;
	long oldMousePosY;

	long mouseDeltaX;
	long mouseDeltaY;

	BOOL fullscreen;
	BOOL editerMode;
	BOOL testMode;

	Scene* nextScene;

	XMFLOAT2 windowSize;

	float deltaTime;
	float fixedDeltaTime;
	float fixedDeltaTimeRate;
	float accumulatedTime;

};

