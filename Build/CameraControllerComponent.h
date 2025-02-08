#pragma once
#include "component.h"

class CameraComponent;
class Input;

class CameraControllerComponent :public Component
{
public:
	CameraControllerComponent(GameObject* gameObject);
	~CameraControllerComponent();

	virtual void Awake(void) override;
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;



private:
};

