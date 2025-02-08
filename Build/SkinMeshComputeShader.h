#pragma once
#include "ComputeShader.h"

//#define BONE_MAX (64)

class SkinMeshComputeShader : public ComputeShader
{
public:

	SkinMeshComputeShader(Renderer* renderer);
	~SkinMeshComputeShader();
	virtual void SetCS(void)override;

	virtual void Destroy(void)override;

	void SetBoneBuffer(XMMATRIX* boneArray);

private:
	ID3D11Buffer* boneBuffer;

};

