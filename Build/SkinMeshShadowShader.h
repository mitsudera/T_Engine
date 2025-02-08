#pragma once
#include "ShaderSet.h"
class SkinMeshShadowShader :public ShaderSet
{
public:
	SkinMeshShadowShader(Renderer* renderer);
	~SkinMeshShadowShader();

	void SetMaterialCbuffer(MaterialCBuffer data);


};

