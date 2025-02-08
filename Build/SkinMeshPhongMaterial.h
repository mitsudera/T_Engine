#pragma once
#include "Material.h"

class SkinMeshPhongShader;

class DX11Texture;

class SkinMeshPhongMaterial :public Material
{
public:
	SkinMeshPhongMaterial(AssetsManager* assetsManager);
	SkinMeshPhongMaterial(SkinMeshPhongMaterial* phong);
	~SkinMeshPhongMaterial();

	// Material ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void SetBufferMaterial(void) override;

	void LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial);


private:
	SkinMeshPhongShader* pSkinMeshPhongShader;




};

