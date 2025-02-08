#pragma once
#include "ShaderSet.h"
class ShadowShader :public ShaderSet
{
public:
	ShadowShader(Renderer* renderer);
	~ShadowShader();

	void SetMaterialCbuffer(MaterialCBuffer data);


};

