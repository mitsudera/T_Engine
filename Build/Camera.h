#pragma once
#include "gameobject.h"

class CameraComponent;

class Camera :public GameObject
{
public:
	Camera(Scene* scene);
	Camera(GameObject* gameObject);
	~Camera();

	virtual void Awake(void) override;


private:

};

