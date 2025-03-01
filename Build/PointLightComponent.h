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
	void SetLightParam(POINT_LIGHT_PARAM param);
private:
	POINT_LIGHT_PARAM param;
	int index;
};

