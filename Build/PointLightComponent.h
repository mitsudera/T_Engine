#pragma once
#include "Component.h"
#include "LightManager.h"

class PointLightComponent :public Component
{
public:

	PointLightComponent(GameObject* gameObject);
	~PointLightComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	POINT_LIGHT_PARAM& GetLightParam(void);

private:
	POINT_LIGHT_PARAM param;
	int index;
};

