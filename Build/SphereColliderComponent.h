#pragma once
#include "ColliderComponent.h"
class SphereColliderComponent :public ColliderComponent
{
public:
	SphereColliderComponent(GameObject* gameObject);
	~SphereColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void DebugDraw(void) override;

	void SetRadius(float r);

protected:

};

