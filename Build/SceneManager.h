#pragma once
#include"Coreminimal.h"



class GameEngine;
class World;
class Scene;
class SaveSystem;
class SceneAssetsData;
class AssetsManager;

class SceneManager
{
public:



	SceneManager(World* world);
	~SceneManager();


	void Update(void);

	Scene* CreateNewScene(void);
	Scene* CreateNewScene(string name);
	Scene* LoadScene(SceneAssetsData* data);

	Scene* LoadScene(string name);

	Scene* OpenScene(string name);

	Scene* OpenScene(SceneAssetsData* data);

	void UnloadScene(string name);

	void UnloadScene(Scene* scene);
	void UnloadScene(void);



private:
	
	GameEngine* pGameEngine;
	World* pWorld;
	SaveSystem* pSaveSystem;
	AssetsManager* pAssetsManager;

	vector<Scene*> loadSceneArray;
	vector<Scene*> unloadSceneArray;






};

