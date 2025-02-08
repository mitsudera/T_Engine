#pragma once
#include "Material.h"

class SkinMeshShadowShader;

class SkinMeshShadowMappingMaterial :public Material
{
public:
	SkinMeshShadowMappingMaterial(AssetsManager* assetsManager);
	~SkinMeshShadowMappingMaterial();


	virtual void SetBufferMaterial(void) override;



	void SetShadowMaterial(Material* material);

private:

	SkinMeshShadowShader* shadowShader;
};

