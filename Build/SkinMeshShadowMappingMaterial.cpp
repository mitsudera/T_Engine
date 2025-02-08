#include "SkinMeshShadowMappingMaterial.h"
#include "SkinMeshShadowShader.h"
#include "AssetsManager.h"
#include "DX11Texture.h"

SkinMeshShadowMappingMaterial::SkinMeshShadowMappingMaterial(AssetsManager* assetsManager)
{
	pAssetsManager = assetsManager;
	this->pShader = assetsManager->GetSkinMeshShadowShader();
	this->shadowShader = assetsManager->GetSkinMeshShadowShader();

}

SkinMeshShadowMappingMaterial::~SkinMeshShadowMappingMaterial()
{
}

void SkinMeshShadowMappingMaterial::SetBufferMaterial(void)
{
	SkinMeshShadowShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	this->shadowShader->SetMaterialCbuffer(mCBuffer);
	if (!noDiffuseTex) textureDiffuse->SetShaderResourcePS(0);


}

void SkinMeshShadowMappingMaterial::SetShadowMaterial(Material* material)
{
	this->diffuse = material->diffuse;
	this->noDiffuseTex = material->noDiffuseTex;

	this->textureDiffuse = material->GetDiffuseTexture();
}
