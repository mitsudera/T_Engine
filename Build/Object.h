#pragma once
#include "Coreminimal.h"

class World;

class Object
{
public:

	enum class Type:int
	{
		Scene,
		GameObject,
		Component,
		Assets,
	};

	Object();
	~Object();

	virtual void SetName(string name, int count);
	virtual void SetName(string name);
	virtual string GetName(void);
	Type GetType(void);
	
	unsigned long GetID(void);

protected:
	unsigned long ID;
	Type type;
private:

	World* pWorld;
	string name;

};

