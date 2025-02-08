#pragma once
#include "ColliderComponent.h"
class BoxColliderComponent :public ColliderComponent
{
public:
	BoxColliderComponent(GameObject* gameObject);
	~BoxColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	virtual void DebugDraw(void) override;


	void SetBox(XMFLOAT3 size);

	void SetBox(float x, float y, float z);

	XMFLOAT3 GetSize(void);

protected:

	XMFLOAT3 size;

};

