#pragma once
#include "ShaderSet.h"
#include "Coreminimal.h"

class Renderer;

class StandardShader :public ShaderSet
{
public:
	StandardShader(Renderer* renderer);
	~StandardShader();

	void SetMaterialCbuffer(MaterialCBuffer data);

private:

};

