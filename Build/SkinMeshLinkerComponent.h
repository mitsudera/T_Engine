#pragma once
#include "Primitivecomponent.h"
#include "SkinMeshPhongShader.h"
class BoneComponent;
class Renderer;
class CBufferManager;

class SkinMeshLinkerComponent :public PrimitiveComponent
{
public:
	SkinMeshLinkerComponent(GameObject* gameObject);
	~SkinMeshLinkerComponent();

	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Draw(void) override;
	virtual void ShadowMapping(void) override;

	virtual void Uninit(void) override;


	void AddBone(BoneComponent* bone);

	vector<BoneComponent*>& GetBoneArray(void);

	int GetBoneCount(void);

	void SetBoneCount(int cnt);

	void SetRagdoll(BOOL enable);

	void SetSpring(string name, float mass, float tension, float resistance);

private:
	vector<BoneComponent*> boneArray;
	Renderer* pRenderer;
	CBufferManager* pCBufferManager;
	XMMATRIX* boneMtxArray;
	SkinMeshPhongShader* skinMeshShader;
	int boneCount;
};

