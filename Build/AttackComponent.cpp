#include "AttackComponent.h"
#include "GameEngine.h"
#include "SoundSpeakerComponent.h"
AttackComponent::AttackComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
	enable = FALSE;
	damage = 0;
	timeCnt = 0.0f;
	endTime = 0.0f;
	delayTime = 0.0f;
	delay = FALSE;

}

AttackComponent::~AttackComponent()
{
}


void AttackComponent::Awake(void)
{
	Component::Awake();
	hasHitSound = FALSE;

}

void AttackComponent::Init(void)
{
	Component::Init();
	enable = FALSE;
	damage = 0;
	timeCnt = 0.0f;
	endTime = 0.0f;
	delayTime = 0.0f;
	delay = FALSE;
}
void AttackComponent::Update(void)
{
	Component::Update();
	if (enable)
	{
		if (endTime < timeCnt)
		{
			enable = FALSE;
		}

		timeCnt += pGameEngine->GetDeltaTime();
	}
	else if (delay)
	{
		if (delayTime < timeCnt)
		{
			enable = TRUE;
			delay = FALSE;
			timeCnt = 0.0f;
		}
		timeCnt += pGameEngine->GetDeltaTime();

	}


}

void AttackComponent::SetAttack(int damage, float endTime)
{
	enable = TRUE;
	this->damage = damage;
	this->endTime = endTime;
	timeCnt = 0.0f;
}

void AttackComponent::SetAttackDelay(int damage, float delayTime, float endTime)
{
	enable = FALSE;
	this->damage = damage;
	this->endTime = endTime;
	this->delayTime = delayTime;
	delay = TRUE;
	timeCnt = 0.0f;

}

void AttackComponent::SetEnable(BOOL enable)
{
	this->enable = enable;
}

void AttackComponent::SetDamage(int damage)
{
	this->damage = damage;
}

int AttackComponent::GetDamage(void)
{
	return this->damage;
}

BOOL AttackComponent::GetEnable(void)
{
	return this->enable;
}

void AttackComponent::PlayHitSound(void)
{
	if (!hasHitSound) 
		return;

	hitSoundSpeaker->StartSound(hitSoundName);
}

void AttackComponent::SetHitSound(SoundSpeakerComponent* speaker, string seName)
{
	hasHitSound = TRUE;
	hitSoundSpeaker = speaker;
	hitSoundName = seName;
}
