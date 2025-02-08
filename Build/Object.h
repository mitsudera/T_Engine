#pragma once
#include "Coreminimal.h"
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
	string GetName(void);
	Type GetType(void);


protected:
	unsigned int ID;
	Type type;
private:
	string name;

};

