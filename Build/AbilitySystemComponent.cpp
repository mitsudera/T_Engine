#include "AbilitySystemComponent.h"

Ability::Ability()
{
}

Ability::~Ability()
{
}

void Ability::UpdateAbility(void)
{
}

AbilitySystemComponent::AbilitySystemComponent(GameObject* gameObject)
{
}

AbilitySystemComponent::~AbilitySystemComponent()
{
}

void AbilitySystemComponent::Init(void)
{
}

void AbilitySystemComponent::Update(void)
{
}

void AbilitySystemComponent::Uninit(void)
{
	for (Ability* ability:abilityArray)
	{
		delete ability;
	}
	abilityArray.clear();
	
}

void AbilitySystemComponent::AddAbility(Ability* ability)
{
	abilityArray.push_back(ability);
}
