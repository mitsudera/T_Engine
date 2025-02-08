#pragma once
#include "Coreminimal.h"
class Renderer;
class CBufferManager;

class ComputeShader
{
public:
	ComputeShader();
	ComputeShader(Renderer* renderer);
	~ComputeShader();
	virtual void SetCS(void);

	void CreateCS(string filePath, string shaderName);

	virtual void Destroy(void);

protected:
	Renderer* pRenderer;
	CBufferManager* pCBufferManager;
	ID3D11ComputeShader* CS;

};

