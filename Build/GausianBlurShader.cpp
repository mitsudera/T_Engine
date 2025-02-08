#include "GausianBlurShader.h"
#include "renderer.h"
#include "CBufferManager.h"

GausianBlurShader::GausianBlurShader(Renderer* renderer)
{
	pRenderer = renderer;


}

GausianBlurShader::~GausianBlurShader()
{
}

void GausianBlurShader::Init(void)
{

	PostEffectShader::Init();
	AddShader("shaders/GausianBlur.hlsl", "xpass");
	AddShader("shaders/GausianBlur.hlsl", "ypass");

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(GausCbuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);



	GausCbuffer initData;
	ZeroMemory(&initData, sizeof(initData));
	float total = 0;
	constexpr float disperision = 40.0f;
	for (int i = 0; i < 8; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		initData.weight[i] = expf(-0.5f * pos * pos / disperision);
		if (i == 0) {
			total += initData.weight[i];
		}
		else {
			total += 2.0f * initData.weight[i];
		}
	}

	for (int i = 0; i < 8; i++) {
		initData.weight[i] /= total;
	}


	pRenderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->cBuffer);

	pRenderer->GetDeviceContext()->UpdateSubresource(cBuffer, 0, NULL, &initData, 0, 0);


}

void GausianBlurShader::PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv)
{
	pCBufferManager->SetCBufferVSPS(cBuffer, CBufferManager::BufferSlot::Free1);

	PostEffectShader::PostEffectDraw(srv, rtv);
}
