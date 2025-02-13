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

	void SaveScene(Scene* scene);
	Scene* LoadScene(SceneAssetsData* aData);


private:
	GameEngine* pGameEngine;
	AssetsManager* pAssetsManager;


};

