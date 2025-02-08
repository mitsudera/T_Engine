#include "UIMaterial.h"
#include "UIShader.h"
#include "AssetsManager.h"

UIMaterial::UIMaterial(AssetsManager* assetsManager)
{
	this->pAssetsManager = assetsManager;
	this->pShader = assetsManager->GetUIShader();
	this->pUIShader = assetsManager->GetUIShader();
}

UIMaterial::UIMaterial(UIMaterial* ui)
{
	this->pAssetsManager = ui->pAssetsManager;
	this->pShader = ui->pShader;
	this->pUIShader = ui->pUIShader;

}

UIMaterial::~UIMaterial()
{
}

void UIMaterial::SetBufferMaterial(void)
{
}
