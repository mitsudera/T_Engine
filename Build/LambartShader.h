#pragma once
#include "ShaderSet.h"
#include "Coreminimal.h"

class Renderer;

class LambartShader :public ShaderSet
{
public:
	LambartShader(Renderer* renderer);
	~LambartShader();

	void SetMaterialCbuffer(MaterialCBuffer data);

private:

};

