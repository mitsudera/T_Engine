#pragma once
#include "ShaderSet.h"
#include "Coreminimal.h"

class Renderer;

struct DebugLineVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

class DebugLineShader :public ShaderSet
{
public:
	DebugLineShader(Renderer* renderer);
	~DebugLineShader();

	void SetMaterialCbuffer(MaterialCBuffer data);

private:

};

