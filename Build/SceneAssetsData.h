#pragma once
#include "Assets.h"
class Scene;
class SceneAssetsData :public Assets
{
public:

	SceneAssetsData(AssetsManager* am);

	virtual void SetName(string name)override;

private:


};

