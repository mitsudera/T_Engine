#pragma once
#include "CoreMinimal.h"


//*********************************************************
// マクロ定義
//*********************************************************


enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE,		//ライト無し
	LIGHT_TYPE_DIRECTIONAL,	//ディレクショナルライト
	LIGHT_TYPE_POINT,		//ポイントライト

	LIGHT_TYPE_NUM
};

enum BLEND_MODE
{
	BLEND_MODE_NONE,		//ブレンド無し
	BLEND_MODE_ALPHABLEND,	//αブレンド
	BLEND_MODE_ADD,			//加算ブレンド
	BLEND_MODE_SUBTRACT,	//減算ブレンド

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//カリング無し
	CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
	CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)

	CULL_MODE_NUM
};

enum FILL_MODE
{
	FILL_MODE_SOLID,		// 通常(ソリッド表示)
	FILL_MODE_WIREFRAME,	// ワイヤーフレーム

	FILL_MODE_NUM
};

enum TEXT_ANCHOR
{
	TEXT_ANCHOR_TOP_LEFT,		// 左上揃え
	TEXT_ANCHOR_TOP_CENTER,		// 中上揃え
	TEXT_ANCHOR_TOP_RIGHT,		// 右上揃え
	TEXT_ANCHOR_CENTER_LEFT,	// 左中央揃え
	TEXT_ANCHOR_CENTER_CENTER,	// 中中央揃え
	TEXT_ANCHOR_CENTER_RIGHT,	// 右中央揃え
	TEXT_ANCHOR_BOTTOM_LEFT,	// 左下揃え
	TEXT_ANCHOR_BOTTOM_CENTER,	// 中央下揃え
	TEXT_ANCHOR_BOTTOM_RIGHT,	// 右下揃え
	TEXT_ANCHOR_MAX				// 最大
};

class GameEngine;
class FullScreenQuadVertex;
class MeshFieldVertex;


class Renderer
{
public:

	Renderer(GameEngine* pGameEngine);
	~Renderer();



	HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void UninitRenderer(void);


	void Clear(void);
	void SetClearColor(float* color4);
	void Present(void);

	ID3D11Device* GetDevice( void );
	ID3D11DeviceContext* GetDeviceContext( void );

	void SetDepthEnable( BOOL Enable );
	void SetBlendState(BLEND_MODE bm);
	void SetCullingMode(CULL_MODE cm);
	void SetFillMode(FILL_MODE fm);
	void SetAlphaTestEnable(BOOL flag);

	void DebugTextOut(char* text, int x, int y);


	IDXGISwapChain* GetSwapChain(void);

	void DrawStringText(string text, float fontSize, XMFLOAT4 color, XMFLOAT2 pos, XMFLOAT2 size, string font, TEXT_ANCHOR textAnchor);


	//void DrawStringText(string text, float fontSize, XMFLOAT4 color, XMFLOAT2 pos, XMFLOAT2 size, TEXT_ANCHOR anchor, string font);


	ID3D11RenderTargetView* GetBackBuffer(void);
	ID3D11DepthStencilView* GetBackBufferDSV(void);

	GameEngine* GetGameEngine(void);

	void DrawFullScreen(void);

	void SetRenderTargetBackBuffer(void);

private:

	GameEngine* pGameEngine;

	D3D_FEATURE_LEVEL			FeatureLevel;

	ID3D11Device*				m_D3DDevice ;
	ID3D11DeviceContext*		m_ImmediateContext;
	IDXGISwapChain*				SwapChain;
	ID3D11RenderTargetView*		RenderTargetViewBackBuffer;
	ID3D11DepthStencilView*		DepthStencilViewBackBuffer;
	float ClearColor[4];	// 背景色


	ID3D11DepthStencilState*	DepthStateEnable;
	ID3D11DepthStencilState*	DepthStateDisable;

	ID3D11BlendState*			BlendStateNone;
	ID3D11BlendState*			BlendStateAlphaBlend;
	ID3D11BlendState*			BlendStateAdd;
	ID3D11BlendState*			BlendStateSubtract;
	BLEND_MODE					BlendStateParam;


	ID3D11RasterizerState*		RasterStateCullOff;
	ID3D11RasterizerState*		RasterStateCullCW;
	ID3D11RasterizerState*		RasterStateCullCCW;
	ID3D11RasterizerState*		RasterStateFillSOLID;
	ID3D11RasterizerState*		RasterStateFillWIRE;

	ID3D11Texture2D* depthTexture;
	ID3D11SamplerState* samplerWrap;
	ID3D11SamplerState* samplerBorder;
	ID3D11SamplerState* samplerClamp;
	ID3D11SamplerState* samplerMirror;




	//utility
	FullScreenQuadVertex* fullScreenVertex;
};


