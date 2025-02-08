#pragma once
#include "PostEffectShader.h"
class FadeShader : public PostEffectShader
{
public:
	FadeShader(Renderer* renderer);
	~FadeShader();

	virtual void Init(void) override;
	virtual void PostEffectDraw(ID3D11ShaderResourceView* srv, ID3D11RenderTargetView* rtv) override;

	struct FadeCbuffer
	{
		XMFLOAT4 fade;
	};

	void SetFade(float f);

private:



};

