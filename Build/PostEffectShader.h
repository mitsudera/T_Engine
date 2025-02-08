#pragma once
#include "Coreminimal.h"

class Renderer;
class RenderTexture;
class CBufferManager;

class PostEffectShader
{
public:
	PostEffectShader();
	~PostEffectShader();
	virtual void Init(void);
	virtual void PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv);

	void SetTexSize(float w, float h);

protected:

	XMFLOAT2 texSize;
	Renderer* pRenderer;
	CBufferManager* pCBufferManager;
	RenderTexture* renderTex1;
	RenderTexture* renderTex2;
	vector<ID3D11PixelShader*> PSArray;
	void AddShader(string filePath, string shaderName);
	ID3D11Buffer* cBuffer;

};

