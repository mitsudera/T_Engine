#include "SceneAssetsData.h"
#include "AssetsManager.h"
#include "GameEngine.h"
#include "World.h"
SceneAssetsData::SceneAssetsData(AssetsManager* am)
{
	pAssetsManager = am;
	this->assetsType = SceneAssetsData::AssetsType::Scene;
}

void SceneAssetsData::SetName(string name)
{
	Assets::SetName(name);

}
