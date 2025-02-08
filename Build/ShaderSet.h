#pragma once
#include "Coreminimal.h"

class Renderer;

// í∏ì_ç\ë¢ëÃ
struct VERTEX_3D
{
	XMFLOAT3	Position;
	XMFLOAT3	Normal;
	XMFLOAT4	Diffuse;
	XMFLOAT2	TexCoord;
	XMFLOAT3	Tangent;
	XMFLOAT3	BiNormal;
};


class ShaderSet
{
public:
		
	typedef enum 
	{
		Lambart,
		Phong,
		UI,
		Terrain,
		SkinMeshPhong,
		Standard,
		MAXShader,

	}ShaderIndex;

	typedef enum 
	{
		StandardShadow,
		SkinMeshShadow,
		MAXShadowShader,

	}ShadowShaderIndex;




	struct MaterialCBuffer 
	{
		XMFLOAT4	ambient;
		XMFLOAT4	diffuse;
		XMFLOAT4	specular;
		XMFLOAT4	emission;
		float		shininess;
		int			noDiffuseTex;
		int			noNormalTex;
		int			noArmTex;

	};


	ShaderSet();
	~ShaderSet();
	void SetShaderRenderer(void);

	void ShaderRelease(void);

	virtual void Uninit(void);

	void CreateVS(string filePath,string shaderName);
	void CreateCustomVS(string filePath, string shaderName, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	void CreateHS(string filePath,string shaderName);
	void CreateDS(string filePath,string shaderName);
	void CreateGS(string filePath,string shaderName);
	void CreatePS(string filePath,string shaderName);

	ShaderIndex GetShaderIndex(void);

	ShadowShaderIndex GetShadowShaderIndex(void);

private:

	ID3D11VertexShader* VS;
	ID3D11HullShader* HS;
	ID3D11DomainShader* DS;
	ID3D11GeometryShader* GS;
	ID3D11PixelShader* PS;

	ID3D11InputLayout* VertexLayout;

protected:

	Renderer* pRenderer;

	string name;

	ID3D11Buffer* materialBuffer;

	ShaderIndex shaderIndex;
	ShadowShaderIndex shadowShaderIndex;
};

