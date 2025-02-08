#pragma once
#include "ColliderComponent.h"
class LineColliderComponent :public ColliderComponent
{
public:
	LineColliderComponent(GameObject* gameObject);
	~LineColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	
	void SetLine(XMFLOAT3 start, XMFLOAT3 end);

	XMFLOAT3 GetStart(void);
	XMFLOAT3 GetEnd(void);

protected:
	XMFLOAT3 startPoint;
	XMFLOAT3 endPoint;


};

