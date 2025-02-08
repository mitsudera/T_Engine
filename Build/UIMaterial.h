#pragma once
#include "Material.h"

class UIShader;

class UIMaterial :public Material
{
public:
	UIMaterial(AssetsManager* assetsManager);
	UIMaterial(UIMaterial* ui);
	~UIMaterial();
	// Material ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void SetBufferMaterial(void) override;



private:



	UIShader* pUIShader;

};

