#include "StandardMaterial.h"
#include "StandardShader.h"
#include "DX11Texture.h"
#include "AssetsManager.h"

StandardMaterial::StandardMaterial(AssetsManager* assetsManager)
{

	this->pAssetsManager = assetsManager;

	this->pShader = assetsManager->GetStandardShader();
	this->pStandardShader = assetsManager->GetStandardShader();

	this->diffuse = { 1.0f,1.0f,1.0f,1.0f };
	this->noDiffuseTex = 1;
	this->noNormalTex = 1;
	this->noArmTex = 1;
	this->textureDiffuse = 0;
	this->textureNormal = 0;
	this->textureSpeculer = 0;


}

StandardMaterial::StandardMaterial(StandardMaterial* lambart)
{
	this->pAssetsManager = lambart->pAssetsManager;

	this->pShader = lambart->pShader;
	this->pStandardShader = lambart->pStandardShader;

	this->diffuse = lambart->diffuse;
	this->noDiffuseTex = lambart->noDiffuseTex;
	this->noNormalTex = lambart->noNormalTex;
	this->noArmTex = lambart->noArmTex;
	this->textureDiffuse = lambart->textureDiffuse;
	this->textureNormal = lambart->textureNormal;
	this->textureSpeculer = lambart->textureSpeculer;



}

StandardMaterial::~StandardMaterial()
{
}

void StandardMaterial::SetBufferMaterial(void)
{
	StandardShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noArmTex;
	this->pStandardShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) textureDiffuse->SetShaderResourcePS(0);
	if (!noNormalTex)textureNormal->SetShaderResourcePS(1);
	if (!noArmTex) textureSpeculer->SetShaderResourcePS(2);


}


