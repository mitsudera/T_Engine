#pragma once
#include "PostEffectShader.h"
class NotEffectShader : public PostEffectShader
{
public:
	NotEffectShader(Renderer* renderer);
	~NotEffectShader();

	virtual void Init(void) override;
	virtual void PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv) override;



private:



};

