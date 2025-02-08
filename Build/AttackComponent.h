#pragma once
#include "component.h"

class SoundSpeakerComponent;

class AttackComponent :public Component
{
public:
	AttackComponent(GameObject* gameObject);
	~AttackComponent();

	virtual void Awake(void)override;
	virtual void Init(void)override;
	virtual void Update(void)override;

	void SetAttack(int damage, float endTime);
	void SetAttackDelay(int damage,float delayTime, float endTime);
	void SetEnable(BOOL enable);
	void SetDamage(int damage);

	int GetDamage(void);
	BOOL GetEnable(void);
	void PlayHitSound(void);
	void SetHitSound(SoundSpeakerComponent* speaker, string seName);

private:
	BOOL hasHitSound;
	SoundSpeakerComponent* hitSoundSpeaker;
	string hitSoundName;
	int damage;
	BOOL enable;
	float timeCnt;
	float endTime;

	BOOL delay;
	float delayTime;

	
};

