#include "RenderTexture.h"
#include "AssetsManager.h"
#include "GameEngine.h"
#include "renderer.h"
RenderTexture::RenderTexture(AssetsManager* am)
{
	pAssetsManager = am;
	pRenderer = pAssetsManager->GetGameEngine()->GetRenderer();
}

RenderTexture::~RenderTexture()
{
	texture->Release();			// シャドウ・マップ
	rtv->Release();	// 深度/ステンシル・ビュー
	srv->Release();	// シェーダ・リソース・ビュー

	depthTexture->Release();			// シャドウ・マップ
	dsv->Release();	// 深度/ステンシル・ビュー
	dssrv->Release();	// シェーダ・リソース・ビュー

}

void RenderTexture::SetPSSRV(BindMode mode,int slot)
{
	if (mode == BindMode::BOTH || mode == BindMode::Texture)
	{
		pRenderer->GetDeviceContext()->PSSetShaderResources(slot, 1, &srv);
	}
	if (mode == BindMode::BOTH)
	{
		pRenderer->GetDeviceContext()->PSSetShaderResources(slot+1, 1, &dssrv);
	}
	if (mode == BindMode::DepthTexture)
	{
		pRenderer->GetDeviceContext()->PSSetShaderResources(slot, 1, &dssrv);

	}
}

void RenderTexture::SetRTV(BindMode mode,int slot)
{
	if (mode == BindMode::BOTH)
	{
		pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &rtv, dsv);
	}
	if (mode == BindMode::Texture)
	{
		pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);
	}
	if (mode == BindMode::DepthTexture)
	{
		pRenderer->GetDeviceContext()->OMSetRenderTargets(1, nullptr, dsv);
	}

}

string RenderTexture::GetName(void)
{
	return this->name;
}

void RenderTexture::CreateRenderTexture(int widht, int height, string name)
{
	this->name = name;

	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = (UINT)widht;   // 幅
	texDesc.Height = (UINT)height;  // 高さ
	texDesc.MipLevels = 1;       // ミップマップ レベル数
	texDesc.ArraySize = 1;       // 配列サイズ
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;  // フォーマット
	texDesc.SampleDesc.Count = 1;  // マルチサンプリングの設定
	texDesc.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	texDesc.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; //
	texDesc.CPUAccessFlags = 0;   // CPUからはアクセスしない
	texDesc.MiscFlags = 0;   // その他の設定なし



	hr = pRenderer->GetDevice()->CreateTexture2D(
		&texDesc,         // 作成する2Dテクスチャの設定
		nullptr,               // 
		&texture);     // 作成したテクスチャを受け取る変数


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = pRenderer->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv);




	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srvDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srvDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	hr = pRenderer->GetDevice()->CreateShaderResourceView(
		texture,          // アクセスするテクスチャ リソース
		&srvDesc,               // シェーダ リソース ビューの設定
		&srv);  // 受け取る変数

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)widht;   // 幅
	descDepth.Height = (UINT)height;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // フォーマット
	descDepth.SampleDesc.Count = 1;  // マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	hr = pRenderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		nullptr,               // 
		&depthTexture);     // 作成したテクスチャを受け取る変数



		// 深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pRenderer->GetDevice()->CreateDepthStencilView(
		depthTexture,         // 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,             // 深度/ステンシル・ビューの設定
		&dsv); // 作成したビューを受け取る変数

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	hr = pRenderer->GetDevice()->CreateShaderResourceView(
		depthTexture,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&dssrv);  // 受け取る変数


}

ID3D11RenderTargetView* RenderTexture::GetRenderTargetView(void)
{
	return this->rtv;
}

ID3D11DepthStencilView* RenderTexture::GetDepthStencilView(void)
{
	return this->dsv;
}

ID3D11ShaderResourceView* RenderTexture::GetSRV(void)
{
	return srv;
}

ID3D11ShaderResourceView* RenderTexture::GetDSSRV(void)
{
	return dssrv;
}

void RenderTexture::ClearRTV(XMFLOAT4 clearColor)
{
	float cc[4] = { clearColor.x,clearColor.y,clearColor.z,clearColor.w };
	pRenderer->GetDeviceContext()->ClearRenderTargetView(
		rtv, // クリアする描画ターゲット
		cc);         // クリアする値


}

void RenderTexture::ClearDSV(float f)
{
	pRenderer->GetDeviceContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, f, 0);

}

