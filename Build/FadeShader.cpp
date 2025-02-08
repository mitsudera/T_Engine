#include "FadeShader.h"
#include "renderer.h"
#include "CBufferManager.h"
#include "GameEngine.h"
FadeShader::FadeShader(Renderer* renderer)
{
	pRenderer = renderer;
	this->texSize = pRenderer->GetGameEngine()->GetWindowSize();
	Init();
}

FadeShader::~FadeShader()
{
}

void FadeShader::Init(void)
{

	PostEffectShader::Init();
	AddShader("shaders/Fade.hlsl", "PS_Main");

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(FadeCbuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);



	FadeCbuffer initData;

	initData.fade.x = 1.0f;
	pRenderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->cBuffer);

	pRenderer->GetDeviceContext()->UpdateSubresource(cBuffer, 0, NULL, &initData, 0, 0);


}

void FadeShader::PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv)
{
	pCBufferManager->SetCBufferVSPS(cBuffer, CBufferManager::BufferSlot::Free1);

	PostEffectShader::PostEffectDraw(srv, rtv);
}

void FadeShader::SetFade(float f)
{
	FadeCbuffer data;

	data.fade.x = f;
	pRenderer->GetDeviceContext()->UpdateSubresource(cBuffer, 0, NULL, &data, 0, 0);

}
