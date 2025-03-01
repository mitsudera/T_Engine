#pragma once
#include "Coreminimal.h"
#include "GameEngine.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class GameEngine;
class Scene;
class TransformComponent;
class SceneAssetsData;
class AssetsManager;
class Assets;
class ProjectSetting;
class World;
class SceneManager;

struct ComponentLoadData
{
	string typeName;
};

struct TransformComponentLoadData:public ComponentLoadData
{
	TransformComponentLoadData(TransformComponent* com);

	XMMATRIX lMtx;


};

struct GameObjectLoadData
{
	string name;
	unsigned long ID;
	unsigned long parentID;
	vector<ComponentLoadData*> componentSaveArray;

};
struct SceneLoadData
{
	string name;
	unsigned long idCnt;
	vector<unsigned long> notUseIdArray;
	vector<GameObjectLoadData*> gameObjectSaveArray;

};

struct ProjectSave
{
	vector<string> sceneFileNameArray;
	vector<string> activeSceneNameArray;

};


class SaveSystem
{
public:
	SaveSystem(GameEngine* gameEngine);
	~SaveSystem();

	void SaveProject(void);
	bool LoadProject(void);


	void SaveScene(Scene* scene);
	Scene* LoadScene(SceneAssetsData* aData);

	void SaveAssets(Assets* assets);

	void LoadAssets(string fileName);
	

	void LoadAllAssets(void);

private:
	GameEngine* pGameEngine;
	AssetsManager* pAssetsManager;
	ProjectSetting* pProjectSetting;
};

