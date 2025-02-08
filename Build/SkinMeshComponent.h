#pragma once
#include "primitivecomponent.h"
#include "SkinMeshPhongShader.h"
#include "SkinMeshComputeShader.h"
class SkinMeshData;
class SkinMeshLinkerComponent;

struct CP
{
	XMFLOAT3 pos;
	int refBoneNum;
	float weight[BONE_MAX];
	int refBoneIndex[BONE_MAX];

};


struct SkinMeshMath
{
	int BoneNum;
	int dummy[3];

};

struct MeshVertex
{
public:

};


class SkinMeshComponent : public PrimitiveComponent
{
public:
	SkinMeshComponent(GameObject* pGameObject);
	~SkinMeshComponent();

	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;

	virtual void ShadowMapping(void) override;


	void SetSkinMeshData(SkinMeshData* data,SkinMeshLinkerComponent* linker);

	void CreateVertexArray(int n, SkinMeshVertex* vertexArray);


	

private:


	SkinMeshData* skinMeshData;
	SkinMeshLinkerComponent* linker;
	
	
	ID3D11Buffer* vertexBuffer;
	SkinMeshVertex* vertexArray;
	int vertNum;



	int boneCnt;

};

