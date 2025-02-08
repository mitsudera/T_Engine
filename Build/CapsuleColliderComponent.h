#pragma once
#include "ColliderComponent.h"
class CapsuleColliderComponent :public ColliderComponent
{
public:
	CapsuleColliderComponent(GameObject* gameObject);
	~CapsuleColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;

	void SetCapsule(XMFLOAT3 start, XMFLOAT3 end, float radius);
	XMFLOAT3 GetStart(void);
	XMFLOAT3 GetEnd(void);
	float GetRadius(void);

	void DebugDraw(void);

protected:

	XMFLOAT3 startPoint;
	XMFLOAT3 endPoint;
	XMFLOAT3 lStartPoint;
	XMFLOAT3 lEndPoint;

	float radius;

};

