#pragma once
#include "Material.h"

class ShadowShader;

class ShadowMappingMaterial :public Material
{
public:
	ShadowMappingMaterial(AssetsManager* assetsManager);
	~ShadowMappingMaterial();


	virtual void SetBufferMaterial(void) override;



	void SetShadowMaterial(Material* material);

private:

	ShadowShader* shadowShader;
};

