#pragma once
#include "Coreminimal.h"
#include "xaudio2.h"
#include "x3daudio.h"

class GameEngine;

enum class SoundType
{
	BGM,
	SE,
	VOICE,
};

struct AudioData
{
	IXAudio2SourceVoice* sorceVoice;
	BYTE* pDataAudio;					// オーディオデータ
	DWORD sizeAudio;					// オーディオデータサイズ
	SoundType type;
	string fileName;
	string name;

};



class SoundEngine
{
public:




	SoundEngine(GameEngine* gameEngine);
	~SoundEngine();

	AudioData LoadSoundData(string filePath, SoundType type);

	void LoadSoundData(AudioData* pData, string filePath, SoundType type);


	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);

	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	
	void StartSound(AudioData* audioData);
	void StopSound(AudioData* audioData);
	void StopAllSound(void);

private:
	GameEngine* pGameEngine;
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasteringVoice;

};

