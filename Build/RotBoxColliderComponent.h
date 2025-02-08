#pragma once
#include "ColliderComponent.h"
class RotBoxColliderComponent :public ColliderComponent
{
public:
	RotBoxColliderComponent(GameObject* gameObject);
	~RotBoxColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;

	virtual void DebugDraw(void) override;


	void SetRotBox(XMFLOAT3 size);

	XMFLOAT3 GetSize(void);
	XMVECTOR GetRot(void);

protected:

	XMFLOAT3 size;
	XMVECTOR rotQton;
};

