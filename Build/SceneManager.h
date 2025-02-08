#pragma once
#include"Coreminimal.h"



class GameEngine;
class World;
class Scene;


class SceneManager
{
public:



	SceneManager(World* world);
	~SceneManager();


	void Update(void);

	Scene* CreateScene(void);
	Scene* CreateScene(string name);

	void LoadScene(string name);
	void UnloadScene(string name);
	void UnloadScene(void);

	vector<Scene*>& GetSceneArray(void);


private:
	
	GameEngine* pGameEngine;
	World* pWorld;
	vector<Scene*> sceneArray;
	vector<Scene*> loadSceneArray;
	vector<Scene*> unloadSceneArray;







};

