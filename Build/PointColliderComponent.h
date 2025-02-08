#pragma once
#include "ColliderComponent.h"
class PointColliderComponent :public ColliderComponent
{
public:
	PointColliderComponent(GameObject* gameObject);
	~PointColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;



protected:

};

