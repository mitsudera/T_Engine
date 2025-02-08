#pragma once
#include "ShaderSet.h"

class Renderer;
class CBufferManager;
constexpr auto BONE_MAX = (256);

struct SkinMeshVertex
{

	XMFLOAT3	Position;
	XMFLOAT3	Normal;
	XMFLOAT4	Diffuse;
	XMFLOAT2	TexCoord;
	XMFLOAT3	Tangent;
	XMFLOAT3	BiNormal;
	XMFLOAT4	BoneWeight;
	XMUINT4		BoneIndex;

};


class SkinMeshPhongShader : public ShaderSet
{

public:

	SkinMeshPhongShader(Renderer* renderer);
	~SkinMeshPhongShader();


	virtual void Uninit(void)override;

	void SetMaterialCbuffer(MaterialCBuffer data);

	void SetBoneBuffer(XMMATRIX* bone);
private:

	ID3D11Buffer* boneBuffer;
	CBufferManager* pCBufferManager;
};

