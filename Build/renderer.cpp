//=============================================================================
//
// レンダリング処理 [Renderer.cpp]
// Author : 
//
//=============================================================================
#include "Renderer.h"
#include "CameraComponent.h"
#include "GameEngine.h"
#include "FullScreenQuadVertex.h"
#include "MeshFieldVertex.h"
//デバッグ用画面テキスト出力を有効にする
#define DEBUG_DISP_TEXTOUT
//シェーダーデバッグ設定を有効にする
//#define DEBUG_SHADER

//*********************************************************
// 構造体
//*********************************************************




Renderer::Renderer(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;


	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	m_D3DDevice = NULL;
	m_ImmediateContext = NULL;
	SwapChain = NULL;
	RenderTargetViewBackBuffer = NULL;
	DepthStencilViewBackBuffer = NULL;




	//DepthStateEnable;
	//DepthStateDisable;

	//BlendStateNone;
	//BlendStateAlphaBlend;
	//BlendStateAdd;
	//BlendStateSubtract;
	//BlendStateParam;


	//RasterStateCullOff;
	//RasterStateCullCW;
	//RasterStateCullCCW;
	//RasterStateFillSOLID;
	//RasterStateFillWIRE;
}

Renderer::~Renderer()
{

}



ID3D11Device* Renderer::GetDevice( void )
{
	return m_D3DDevice;
}


ID3D11DeviceContext* Renderer::GetDeviceContext( void )
{
	return m_ImmediateContext;
}


void Renderer::SetDepthEnable( BOOL Enable )
{
	if( Enable )
		m_ImmediateContext->OMSetDepthStencilState( DepthStateEnable, NULL );
	else
		m_ImmediateContext->OMSetDepthStencilState( DepthStateDisable, NULL );

}


void Renderer::SetBlendState(BLEND_MODE bm)
{
	BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (BlendStateParam)
	{
	case BLEND_MODE_NONE:
		m_ImmediateContext->OMSetBlendState(BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		m_ImmediateContext->OMSetBlendState(BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		m_ImmediateContext->OMSetBlendState(BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		m_ImmediateContext->OMSetBlendState(BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void Renderer::SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		m_ImmediateContext->RSSetState(RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		m_ImmediateContext->RSSetState(RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		m_ImmediateContext->RSSetState(RasterStateCullCCW);
		break;
	}
}

void Renderer::SetFillMode(FILL_MODE fm)
{
	switch (fm)
	{
	case FILL_MODE_SOLID:
		m_ImmediateContext->RSSetState(RasterStateFillSOLID);
		break;
	case FILL_MODE_WIREFRAME:
		m_ImmediateContext->RSSetState(RasterStateFillWIRE);
		break;
	}
}

void Renderer::SetAlphaTestEnable(BOOL flag)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	if (flag)
		blendDesc.AlphaToCoverageEnable = TRUE;
	else
		blendDesc.AlphaToCoverageEnable = FALSE;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (BlendStateParam)
	{
	case BLEND_MODE_NONE:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ALPHABLEND:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ADD:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_SUBTRACT:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL)
		blendState->Release();
}



ID3D11RenderTargetView* Renderer::GetBackBuffer(void)
{
	return this->RenderTargetViewBackBuffer;
}

ID3D11DepthStencilView* Renderer::GetBackBufferDSV(void)
{
	return this->DepthStencilViewBackBuffer;
}

GameEngine* Renderer::GetGameEngine(void)
{
	return this->pGameEngine;
}

void Renderer::DrawFullScreen(void)
{
	return this->fullScreenVertex->Draw();
}

void Renderer::SetRenderTargetBackBuffer(void)
{
	this->m_ImmediateContext->OMSetRenderTargets(1,&this->RenderTargetViewBackBuffer, this->DepthStencilViewBackBuffer);
}







//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Renderer::InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	XMFLOAT2 screen = pGameEngine->GetWindowSize();

	// デバイス、スワップチェーン、コンテキスト生成
	DWORD deviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = (UINT)screen.x;
	sd.BufferDesc.Height = (UINT)screen.y;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = bWindow;
	// 文字出力用設定
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	


	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&SwapChain,
		&m_D3DDevice,
		&FeatureLevel,
		&m_ImmediateContext);
	if (FAILED(hr))
		return hr;

	// 文字出力用設定
	hr = SwapChain->ResizeBuffers(0, (UINT)screen.x, (UINT)screen.y, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE); // N.B. the GDI compatible flag
	if (FAILED(hr))
		return hr;

	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	m_D3DDevice->CreateRenderTargetView( pBackBuffer, NULL, &RenderTargetViewBackBuffer );
	pBackBuffer->Release();



	//ステンシル用テクスチャー作成
	depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(td) );
	td.Width			= sd.BufferDesc.Width;
	td.Height			= sd.BufferDesc.Height;
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc		= sd.SampleDesc;
	td.Usage			= D3D11_USAGE_DEFAULT;
	td.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags	= 0;
    td.MiscFlags		= 0;
	m_D3DDevice->CreateTexture2D( &td, NULL, &depthTexture );

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	m_D3DDevice->CreateDepthStencilView( depthTexture, &dsvd, &DepthStencilViewBackBuffer );


	m_ImmediateContext->OMSetRenderTargets( 1, &RenderTargetViewBackBuffer, DepthStencilViewBackBuffer );


	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)screen.x;
	vp.Height = (FLOAT)screen.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports( 1, &vp );



	// ラスタライザステート作成
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE; 
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullOff);
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateFillSOLID);

	rd.CullMode = D3D11_CULL_FRONT;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateCullCCW);

	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	m_D3DDevice->CreateRasterizerState(&rd, &RasterStateFillWIRE);

	// カリングモード設定（CCW）
	SetCullingMode(CULL_MODE_BACK);
	// フィルモード設定
	SetFillMode(FILL_MODE_SOLID);



	// ブレンドステートの作成
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState( &blendDesc, &BlendStateAlphaBlend );

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &BlendStateSubtract);

	// アルファブレンド設定
	SetBlendState(BLEND_MODE_ALPHABLEND);




	// 深度ステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &DepthStateDisable );//深度無効ステート

	// 深度ステンシルステート設定
	SetDepthEnable(TRUE);


	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerWrap = NULL;
	m_D3DDevice->CreateSamplerState( &samplerDesc, &samplerWrap );

	m_ImmediateContext->PSSetSamplers( 0, 1, &samplerWrap );
	m_ImmediateContext->DSSetSamplers( 0, 1, &samplerWrap );

	// Borderサンプラーステート設定
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;

	samplerBorder = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerBorder);
	m_ImmediateContext->PSSetSamplers(1, 1, &samplerBorder);
	m_ImmediateContext->DSSetSamplers(1, 1, &samplerBorder);

	// Clampサンプラーステート設定
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerClamp = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerClamp);
	m_ImmediateContext->PSSetSamplers(2, 1, &samplerClamp);
	m_ImmediateContext->DSSetSamplers(2, 1, &samplerClamp);


	// Mirrorサンプラーステート設定
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerMirror = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerMirror);
	m_ImmediateContext->PSSetSamplers(3, 1, &samplerMirror);
	m_ImmediateContext->DSSetSamplers(3, 1, &samplerMirror);





	this->fullScreenVertex = new FullScreenQuadVertex(this);




	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Renderer::UninitRenderer(void)
{
	delete fullScreenVertex;

	// オブジェクト解放
	if (DepthStateEnable)		DepthStateEnable->Release();
	if (DepthStateDisable)	DepthStateDisable->Release();
	if (BlendStateNone)		BlendStateNone->Release();
	if (BlendStateAlphaBlend)	BlendStateAlphaBlend->Release();
	if (BlendStateAdd)		BlendStateAdd->Release();
	if (BlendStateSubtract)	BlendStateSubtract->Release();
	if (RasterStateCullOff)	RasterStateCullOff->Release();
	if (RasterStateCullCW)	RasterStateCullCW->Release();
	if (RasterStateCullCCW)	RasterStateCullCCW->Release();
	if (RasterStateFillSOLID) RasterStateFillSOLID->Release();
	if (RasterStateFillWIRE) RasterStateFillWIRE->Release();


	if (m_ImmediateContext)		m_ImmediateContext->ClearState();
	if (RenderTargetViewBackBuffer)		RenderTargetViewBackBuffer->Release();
	if (DepthStencilViewBackBuffer)		DepthStencilViewBackBuffer->Release();
	if (m_ImmediateContext)		m_ImmediateContext->Release();
	if (m_D3DDevice)			m_D3DDevice->Release();
	if (depthTexture)			depthTexture->Release();
	if (samplerWrap)			samplerWrap->Release();
	if (samplerBorder)			samplerBorder->Release();
	if (samplerClamp)			samplerClamp->Release();
	if (samplerMirror)			samplerMirror->Release();

	if (SwapChain)				SwapChain->Release();


}



//=============================================================================
// バックバッファクリア
//=============================================================================
void Renderer::Clear(void)
{
	// バックバッファクリア
	m_ImmediateContext->ClearRenderTargetView( RenderTargetViewBackBuffer, ClearColor );
	m_ImmediateContext->ClearDepthStencilView( DepthStencilViewBackBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void Renderer::SetClearColor(float* color4)
{
	ClearColor[0] = color4[0];
	ClearColor[1] = color4[1];
	ClearColor[2] = color4[2];
	ClearColor[3] = color4[3];
}


//=============================================================================
// プレゼント
//=============================================================================
void Renderer::Present(void)
{
	SwapChain->Present( 0, 0 );
}



//=============================================================================
// デバッグ用テキスト出力
//=============================================================================
void Renderer::DebugTextOut(char* text, int x, int y)
{
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	HRESULT hr;

	//バックバッファからサーフェスを取得する
	IDXGISurface1* pBackSurface = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);

	if (SUCCEEDED(hr))
	{
		//取得したサーフェスからデバイスコンテキストを取得する
		HDC hdc;
		hr = pBackSurface->GetDC(FALSE, &hdc);

		if (SUCCEEDED(hr))
		{
			//文字色を白に変更
			SetTextColor(hdc, RGB(255, 255, 255));
			//背景を透明に変更
			SetBkMode(hdc, TRANSPARENT);

			XMFLOAT2 screen = pGameEngine->GetWindowSize();

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = (LONG)screen.x;
			rect.bottom = (LONG)screen.y;

			//テキスト出力
			DrawText(hdc, text, (int)strlen(text), &rect, DT_LEFT);

			//デバイスコンテキストを解放する
			pBackSurface->ReleaseDC(NULL);
		}
		//サーフェスを解放する
		pBackSurface->Release();

		//レンダリングターゲットがリセットされるのでセットしなおす
		m_ImmediateContext->OMSetRenderTargets(1, &RenderTargetViewBackBuffer, DepthStencilViewBackBuffer);
	}
#endif
}




IDXGISwapChain* Renderer::GetSwapChain(void)
{
	return SwapChain;
}

void Renderer::DrawStringText(string text, float fontSize, XMFLOAT4 color, XMFLOAT2 pos, XMFLOAT2 size, string font, TEXT_ANCHOR textAnchor)
{
	HRESULT hr;

	//バックバッファからサーフェスを取得する
	IDXGISurface1* pBackSurface = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);



	if (SUCCEEDED(hr))
	{
		//取得したサーフェスからデバイスコンテキストを取得する
		HDC hdc;
		hr = pBackSurface->GetDC(FALSE, &hdc);

		if (SUCCEEDED(hr))
		{
			//文字色を変更
			SetTextColor(hdc, RGB(color.x * 255, color.y * 255, color.z * 255));

			//背景を透明に変更
			SetBkMode(hdc, TRANSPARENT);



			RECT rect;
			rect.left = (LONG)(pos.x);
			rect.top = (LONG)(pos.y);
			rect.right = (LONG)(pos.x + size.x );
			rect.bottom = (LONG)(pos.y + size.y );




			int rectDT = DT_SINGLELINE;	//縦のRECTも有効化する

			switch (textAnchor % 3)
			{	//横のRECT
			case 0:
				rectDT |= DT_LEFT;
				break;
			case 1:
				rectDT |= DT_CENTER;
				break;
			case 2:
				rectDT |= DT_RIGHT;
				break;
			default:
				break;
			}

			switch (textAnchor / 3)
			{	//縦のRECT
			case 0:
				rectDT |= DT_TOP;
				break;
			case 1:
				rectDT |= DT_VCENTER;
				break;
			case 2:
				rectDT |= DT_BOTTOM;
				break;
			default:
				break;
			}


			LPCSTR lpcwstrFontName = font.c_str();
			//Font作成 Showcard Gothic
			HFONT font = CreateFont((int)fontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Showcard Gothic");//フォントの指定　引数にできる
			HGDIOBJ hgdi = SelectObject(hdc, font);

			//テキスト出力
			DrawText(hdc, const_cast<char*>(text.c_str()), (int)text.length(), &rect, rectDT);

			SelectObject(hdc, hgdi); //フォントを元に戻す
			DeleteObject(font);	//オブジェクト削除

		//デバイスコンテキストを解放する
			pBackSurface->ReleaseDC(NULL);

		}
		//サーフェスを解放する
		pBackSurface->Release();

		//レンダリングターゲットがリセットされるのでセットしなおす
		m_ImmediateContext->OMSetRenderTargets(1, &RenderTargetViewBackBuffer, DepthStencilViewBackBuffer);
	}
}