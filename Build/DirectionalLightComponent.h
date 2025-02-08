#pragma once
#include "Component.h"
#include "LightManager.h"

class DirectionalLightComponent :public Component
{
public:

	DirectionalLightComponent(GameObject* gameObject);
	~DirectionalLightComponent();

	virtual void Awake(void) override;
	
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	DIREC_LIGHT_PARAM& GetLightParam(void);

	XMFLOAT3 GetDirection(void);

	void SetDiffuse(XMFLOAT4 dif);
	void SetAmbient(XMFLOAT4 amb);
	void SetEnable(BOOL enable);

private:

	DIREC_LIGHT_PARAM  param;
	int index;
};

