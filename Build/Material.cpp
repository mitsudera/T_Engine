#include "Material.h"
#include "renderer.h"
#include "ShaderSet.h"
#include "AssetsManager.h"
#include "DX11Texture.h"

Material::Material()
{
	assetsType = Assets::AssetsType::Material;
	this->noDiffuseTex = 1;
	this->noNormalTex = 1;
	this->noSpecularTex = 1;
	this->ambient = { 1.0f,1.0f,1.0f,1.0f };
	this->diffuse= { 1.0f,1.0f,1.0f,1.0f };
	this->specular= { 0.0f,0.0f,0.0f,0.0f };
	this->emission = { 0.0f,0.0f,0.0f,0.0f };
	this->shininess = 10.0f;
}

Material::~Material()
{
}


void Material::SetShaderRenderer(void)
{
	pShader->SetShaderRenderer();

}

void Material::SetBufferShadowMapping(void)
{
}

ShaderSet* Material::GetShaderSet(void)
{
	return this->pShader;
}


void Material::LoadDiffuseTex(string fName)
{
	textureDiffuse = pAssetsManager->LoadTexture(fName);
	this->difPath = textureDiffuse->GetPath();
	this->noDiffuseTex = 0;
}

void Material::LoadNormalTex(string fName)
{
	textureNormal = pAssetsManager->LoadTexture(fName);
	this->norPath = textureNormal->GetPath();
	this->noNormalTex = 0;

}

void Material::LoadSpeculerTex(string fName)
{
	textureSpeculer = pAssetsManager->LoadTexture(fName);
	this->spePath = textureSpeculer->GetPath();
	this->noSpecularTex = 0;

}



DX11Texture* Material::GetDiffuseTexture(void)
{
	return this->textureDiffuse;
}

void Material::SetDiffuse(XMFLOAT4 dif)
{
	diffuse = dif;
}

void Material::SetAmbient(XMFLOAT4 amb)
{
	ambient = amb;
}

void Material::SetSpecular(XMFLOAT4 spe)
{
	specular = spe;
}

void Material::SetEmission(XMFLOAT4 emi)
{
	emission = emi;
}

void Material::SetShininess(float shin)
{
	shininess = shin;
}

void Material::SetDifPath(string path)
{
	difPath = path;
}

string Material::GetDifPath(void)
{
	return difPath;
}

void Material::SetNorPath(string path)
{
	norPath = path;
}

string Material::GetNorPath(void)
{
	return norPath;
}

void Material::SetSpePath(string path)
{
	spePath = path;
}

string Material::GetSpePath(void)
{
	return spePath;
}
