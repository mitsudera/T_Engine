#pragma once
#include "ColliderComponent.h"

class TerrainComponent;


class TerrainColliderComponent : public ColliderComponent
{
public:
	TerrainColliderComponent(GameObject* gameObject);
	~TerrainColliderComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void SetTerrainComponent(TerrainComponent* com);

	float GetHeight(XMFLOAT3 pos);
	
	XMFLOAT2  GetSize(void);

protected:

	TerrainComponent* terrain;
	XMFLOAT2 size;

};

