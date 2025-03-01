#pragma once
#include "Object.h"
class AssetsManager;
class Assets : public Object
{
public:
	Assets();
	~Assets();

	enum class AssetsType :int
	{
		Material,
		Texture,
		MeshData,
		SkinMeshData,
		AnimationData,
		Scene,
		Prefab,
	};

	AssetsType GetAssetsType(void);
	virtual void SetName(string name, int count) override;
	virtual void SetName(string name) override;
	void SetConstName(string name);
	void SetPath(string path);
	string GetPath(void);

protected:

	AssetsManager* pAssetsManager;
	AssetsType assetsType;
	string path;
	string newPath;
};

