#pragma once
#include "gameobject.h"
class SkySphere : public GameObject
{
public:
	SkySphere(Scene* scene);
	~SkySphere();

	virtual void Awake(void) override;

};

