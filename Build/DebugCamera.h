#pragma once
#include "gameobject.h"

class CameraComponent;

class DebugCamera :public GameObject
{

public:
	DebugCamera(Scene* scene);
	DebugCamera(GameObject* gameObject);
	~DebugCamera();

	virtual void Awake(void) override;


private:

};

