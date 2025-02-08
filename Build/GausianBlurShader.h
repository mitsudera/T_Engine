#pragma once
#include "PostEffectShader.h"
class GausianBlurShader : public PostEffectShader
{
public:
	GausianBlurShader(Renderer* renderer);
	~GausianBlurShader();

	virtual void Init(void) override;
	virtual void PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv) override;

	struct GausCbuffer
	{
		float weight[8];
	};

private:

	

};

