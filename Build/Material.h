#pragma once
#include "Coreminimal.h"
#include "Assets.h"

class Renderer;

class ShaderSet;

class AssetsManager;

class DX11Texture;

class Material:public Assets
{
public:
	Material();
	~Material();

	virtual void SetName(string name);
	void SetName(unsigned int count);
	void SetShaderRenderer(void);

	virtual void SetBufferMaterial(void) = 0;

	void SetBufferShadowMapping(void);

	ShaderSet* GetShaderSet(void);


	void LoadDiffuseTex(string fName);
	void LoadNormalTex(string fName);
	void LoadSpeculerTex(string fName);


	DX11Texture* GetDiffuseTexture(void);

	void SetDiffuse(XMFLOAT4 dif);
	void SetAmbient(XMFLOAT4 amb);
	void SetSpecular(XMFLOAT4 spe);
	void SetEmission(XMFLOAT4 emi);
	void SetShininess(float shin);

	XMFLOAT4	ambient;
	XMFLOAT4	diffuse;
	XMFLOAT4	specular;
	XMFLOAT4	emission;
	float		shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noSpecularTex;


	void SetDifPath(string path);
	string GetDifPath(void);

	void SetNorPath(string path);
	string GetNorPath(void);

	void SetSpePath(string path);
	string GetSpePath(void);

protected:
	DX11Texture* textureDiffuse;
	DX11Texture* textureNormal;
	DX11Texture* textureSpeculer;

	string difPath;
	string norPath;
	string spePath;



	ShaderSet* pShader;

};

