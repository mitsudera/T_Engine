#include "SoundSpeakerComponent.h"
#include "gameobject.h"
#include "SoundEngine.h"
#include "GameEngine.h"


const string soundPath = "Data/Sound/";

SoundSpeakerComponent::SoundSpeakerComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

SoundSpeakerComponent::~SoundSpeakerComponent()
{
	
}

void SoundSpeakerComponent::Awake(void)
{
	Component::Awake();
	pSoundEngine = pGameEngine->GetSoundEngine();
	TypeName = typeid(SoundSpeakerComponent).name();;

}

void SoundSpeakerComponent::Uninit(void)
{
	Component::Uninit();
	StopAllSound();

	for (AudioData* audioData : audioDataArray)
	{
		// ソースボイスの破棄
		audioData->sorceVoice->DestroyVoice();

		// オーディオデータの開放
		free(audioData->pDataAudio);

		delete audioData;

	}

}

void SoundSpeakerComponent::Update(void)
{
	Component::Update();


}

int SoundSpeakerComponent::LoadSound(string fileName, string name, SoundType type)
{
	string filePath;

	switch (type)
	{
	case SoundType::BGM:
		filePath = soundPath + "BGM/" + fileName;

		break;
	case SoundType::SE:
		filePath = soundPath + "SE/" + fileName;

		break;
	case SoundType::VOICE:
		filePath = soundPath + "VOICE/" + fileName;

		break;
	default:
		break;
	}

	AudioData* newData = new AudioData;
	pSoundEngine->LoadSoundData(newData, filePath, type);

	newData->fileName = fileName;
	newData->name = name;

	
	audioDataArray.push_back(newData);

	return (int)audioDataArray.size() - 1;
}

void SoundSpeakerComponent::StartSound(int index)
{
	pSoundEngine->StartSound(this->audioDataArray[index]);
}

void SoundSpeakerComponent::StartSound(string name)
{
	for (AudioData* audioData:audioDataArray)
	{
		if (audioData->name == name)
		{
			pSoundEngine->StartSound(audioData);
			return;
		}
	}
}

void SoundSpeakerComponent::StopSound(int index)
{
	pSoundEngine->StopSound(this->audioDataArray[index]);

}

void SoundSpeakerComponent::StopSound(string name)
{
	for (AudioData* audioData : audioDataArray)
	{
		if (audioData->name == name)
		{
			pSoundEngine->StopSound(audioData);
			return;
		}
	}

}

void SoundSpeakerComponent::StopAllSound(void)
{
	for (AudioData* audioData : audioDataArray)
	{
		pSoundEngine->StopSound(audioData);
	}

}
