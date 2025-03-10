#pragma once
#include "Material.h"

class UIShader;

class UIMaterial :public Material
{
public:
	UIMaterial(AssetsManager* assetsManager);
	UIMaterial(UIMaterial* ui);
	~UIMaterial();
	// Material を介して継承されました
	virtual void SetBufferMaterial(void) override;



private:



	UIShader* pUIShader;

};

