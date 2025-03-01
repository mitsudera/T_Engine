#include "Object.h"

Object::Object()
{


}

Object::~Object()
{
}

void Object::SetName(string name, int count)
{
	if (count == 0)
	{
		this->name = name;
	}
	else
	{
		this->name = name + "(" + to_string(count) + ")";

	}

}

void Object::SetName(string name)
{
	this->name = name;
}

string Object::GetName(void)
{
	return this->name;
}

Object::Type Object::GetType(void)
{
	return type;
}

unsigned long Object::GetID(void)
{
	return ID;
}

void Object::SetID(unsigned long id)
{
	ID = id;
}

