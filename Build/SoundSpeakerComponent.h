#pragma once
#include "Coreminimal.h"
#include "component.h"
#include "SoundEngine.h"



class SoundSpeakerComponent : public Component
{
public:
	SoundSpeakerComponent(GameObject* gameObject);
	~SoundSpeakerComponent();

	virtual void Awake(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;



	int LoadSound(string fileName, string name, SoundType type);

	void StartSound(int index);
	void StartSound(string name);
	void StopSound(int index);
	void StopSound(string name);
	void StopAllSound(void);


private:
	
	SoundEngine* pSoundEngine;

	vector<AudioData*> audioDataArray;
	


};

