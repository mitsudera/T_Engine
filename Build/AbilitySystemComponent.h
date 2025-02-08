#pragma once
#include "component.h"
class Ability
{
public:
	Ability();
	~Ability();
	virtual void UpdateAbility(void);

protected:
	string name;
	
};
class AbilitySystemComponent :public Component
{
public:
	AbilitySystemComponent(GameObject* gameObject);
	~AbilitySystemComponent();

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	void AddAbility(Ability* ability);
private:

	vector<Ability*> abilityArray;
	Ability* activeAbility;

};

