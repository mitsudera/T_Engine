#include "NotEffectShader.h"
#include "renderer.h"
#include "CBufferManager.h"
#include "GameEngine.h"
NotEffectShader::NotEffectShader(Renderer* renderer)
{
	pRenderer = renderer;
	this->texSize = pRenderer->GetGameEngine()->GetWindowSize();
	Init();
}

NotEffectShader::~NotEffectShader()
{
}

void NotEffectShader::Init(void)
{

	PostEffectShader::Init();
	AddShader("shaders/NotEffect.hlsl", "PSMain");



}

void NotEffectShader::PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv)
{

	PostEffectShader::PostEffectDraw(srv, rtv);
}

