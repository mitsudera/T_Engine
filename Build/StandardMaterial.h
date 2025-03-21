#pragma once
#include "Material.h"

class StandardShader;
class DX11Texture;
class StandardMaterial :public Material
{
public:
	StandardMaterial(AssetsManager* assetsManager);
	StandardMaterial(StandardMaterial* lambart);
	~StandardMaterial();
	// Material を介して継承されました
	virtual void SetBufferMaterial(void) override;




private:



	StandardShader* pStandardShader;
};

