#include "TerrainMaterial.h"
#include "DX11Texture.h"
#include "AssetsManager.h"

TerrainMaterial::TerrainMaterial(AssetsManager* assetsManager)
{
	this->pAssetsManager = assetsManager;
	this->pTerrainShader = assetsManager->GetTerrainShader();
	this->pShader = assetsManager->GetTerrainShader();


}

TerrainMaterial::TerrainMaterial(TerrainMaterial* terrain)
{
	this->pAssetsManager = terrain->pAssetsManager;

	this->pShader = terrain->pShader;
	this->pTerrainShader = terrain->pTerrainShader;

	this->diffuse = terrain->diffuse;
	this->ambient = terrain->ambient;
	this->emission = terrain->emission;
	this->specular = terrain->specular;
	this->shininess = terrain->shininess;
	this->noDiffuseTex = terrain->noDiffuseTex;
	this->noNormalTex = terrain->noNormalTex;
	this->noSpecularTex = terrain->noSpecularTex;
	this->textureDiffuse = terrain->textureDiffuse;
	this->textureNormal = terrain->textureNormal;
	this->textureSpeculer = terrain->textureSpeculer;

}

TerrainMaterial::~TerrainMaterial()
{
}

void TerrainMaterial::SetBufferMaterial(void)
{
	TerrainShader::MaterialCBuffer mCBuffer;
	mCBuffer.ambient = this->ambient;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.emission = this->emission;
	mCBuffer.shininess = this->shininess;
	mCBuffer.specular = this->specular;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noSpecularTex;
	this->pTerrainShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) textureDiffuse->SetShaderResourcePS(0);
	if (!noNormalTex)textureNormal->SetShaderResourcePS(1);
	if (!noSpecularTex) textureSpeculer->SetShaderResourcePS(2);

	TessCBuffer data;
	data.cbEdgeFactor = tessEdgeFacter;
	data.cbInsideFactor = tessInsideFacter;
	data.heightFactor = heightFacter;
	data.uvScale[0] = uvScale.x;
	data.uvScale[1] = uvScale.y;
	data.heghtMapUVScale[0] = heightMapUvScale.x;
	data.heghtMapUVScale[1] = heightMapUvScale.y;
	this->pTerrainShader->SetTessBuffer(data);

}


void TerrainMaterial::SetTessEdgeFacter(float facter)
{
	this->tessEdgeFacter = facter;
}

void TerrainMaterial::SetTessInsideFacter(float facter)
{
	this->tessInsideFacter = facter;
}

void TerrainMaterial::SetHeightFacter(float facter)
{
	heightFacter = facter;
}

void TerrainMaterial::SetUVScale(XMFLOAT2 scale)
{
	uvScale = scale;
}

void TerrainMaterial::SetHeghtMapUVScale(XMFLOAT2 scale)
{
	heightMapUvScale = scale;
}

float TerrainMaterial::GetHeightFacter(void)
{
	return this->heightFacter;
}




