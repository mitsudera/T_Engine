#pragma once
#include "Coreminimal.h"

class AssetsManager;
class Renderer;
//レンダリング可能テクスチャクラス
class RenderTexture
{
public:
	enum class BindMode :int
	{
		Texture,
		DepthTexture,
		BOTH,
	};


	RenderTexture(AssetsManager* am);
	~RenderTexture();

	void SetPSSRV(BindMode mode, int slot);

	void SetRTV(BindMode mode, int slot);

	string GetName(void);

	void CreateRenderTexture(int widht, int height, string name);

	ID3D11RenderTargetView* GetRenderTargetView(void);
	ID3D11DepthStencilView* GetDepthStencilView(void);

	ID3D11ShaderResourceView* GetSRV(void);
	ID3D11ShaderResourceView* GetDSSRV(void);

	void ClearRTV(XMFLOAT4 clearColor);
	void ClearDSV(float f);

private:

	AssetsManager* pAssetsManager;
	ID3D11Texture2D* texture;			// シャドウ・マップ
	ID3D11RenderTargetView* rtv;	// 深度/ステンシル・ビュー
	ID3D11ShaderResourceView* srv;	// シェーダ・リソース・ビュー

	ID3D11Texture2D* depthTexture;			// シャドウ・マップ
	ID3D11DepthStencilView* dsv;	// 深度/ステンシル・ビュー
	ID3D11ShaderResourceView* dssrv;	// シェーダ・リソース・ビュー

	string name;

	Renderer* pRenderer;
};

