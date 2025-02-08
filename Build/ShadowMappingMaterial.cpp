#include "ShadowMappingMaterial.h"
#include "ShadowShader.h"
#include "AssetsManager.h"
#include "DX11Texture.h"

ShadowMappingMaterial::ShadowMappingMaterial(AssetsManager* assetsManager)
{
	pAssetsManager = assetsManager;
	this->pShader = assetsManager->GetShadowShader();
	this->shadowShader = assetsManager->GetShadowShader();

}

ShadowMappingMaterial::~ShadowMappingMaterial()
{
}

void ShadowMappingMaterial::SetBufferMaterial(void)
{
	ShadowShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	this->shadowShader->SetMaterialCbuffer(mCBuffer);
	if (!noDiffuseTex) textureDiffuse->SetShaderResourcePS(0);


}

void ShadowMappingMaterial::SetShadowMaterial(Material* material)
{
	this->diffuse = material->diffuse;
	this->noDiffuseTex = material->noDiffuseTex;

	this->textureDiffuse = material->GetDiffuseTexture();
}
