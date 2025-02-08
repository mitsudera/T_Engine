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
	// Material ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void SetBufferMaterial(void) override;




private:



	StandardShader* pStandardShader;
};

