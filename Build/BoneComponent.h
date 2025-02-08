#pragma once
#include "component.h"
class BoneData;
class SkinMeshLinkerComponent;
class BoneComponent : public Component
{
public:

	enum class Joint :int
	{
		Standard,
		Spring,
	};

	BoneComponent(GameObject* gameObject);
	~BoneComponent();
	virtual void Awake(void) override;
	virtual void Init(void) override;

	virtual void FixedUpdate(void) override;
	virtual void LateUpdate(void)override;
	virtual void DebugDraw(void)override;
	XMMATRIX GetInitMtxInverse(void);

	void SetBone(BoneData* data, SkinMeshLinkerComponent* linker);


	XMMATRIX& GetBoneMtx(void);

	void SetIsPhysics(BOOL b);
	BOOL GetIsPhysics(void);
	void SetJoint(Joint j);
	void SetIsRoot(BOOL b);
	void SetParentBone(BoneComponent* parent);
	void CreateChild(GameObject* child);

	void SetSpringPhysics(float mass, float tension, float resistance);

	string GetRigName(void);
private:
	BOOL isPhysics;
	Joint joint;
	float mass; // éøó 
	// í£óÕ
	float tension;
	// íÔçR
	float resistance;
	XMVECTOR wpv;
	XMVECTOR defaultLength;//ÇŒÇÀÇÃé©ëRí∑

	string rigName;

	BoneComponent* parentBone;
	vector<BoneComponent*> childArray;
	BOOL isRoot;
	XMVECTOR velocity;
	XMMATRIX initMtx;
	XMMATRIX initMtxInv;
	XMMATRIX boneMtx;
	SkinMeshLinkerComponent* linker;
	TransformComponent* transform;
};

