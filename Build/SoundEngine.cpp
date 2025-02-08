#include "SoundEngine.h"
#include "GameEngine.h"

SoundEngine::SoundEngine(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;
	HWND hWnd = pGameEngine->GetWindowHandle();
	HRESULT hr;
	hr= CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(pGameEngine->GetWindowHandle(), "COMライブラリの初期化に失敗", "警告！", MB_ICONWARNING);

		return;

	}
	hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		MessageBox(pGameEngine->GetWindowHandle(), "XAudio2オブジェクトの作成に失敗", "警告！", MB_ICONWARNING);
		// COMライブラリの終了処理
		CoUninitialize();

		return;

	}

	// マスターボイスの生成
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2オブジェクトの開放
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return;
	}



}

SoundEngine::~SoundEngine()
{


	if (pMasteringVoice)
	{ 
		pMasteringVoice->DestroyVoice();
	}

	if (pXAudio2)
	{
		pXAudio2->Release(); 
	}
	// COMライブラリの終了処理
	CoUninitialize();

}

AudioData SoundEngine::LoadSoundData(string filePath, SoundType type)
{
	HWND hWnd = pGameEngine->GetWindowHandle();
	HRESULT hr;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	AudioData audioData;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
		return audioData;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
		return audioData;
	}

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return audioData;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return audioData;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return audioData;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return audioData;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return audioData;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &audioData.sizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return audioData;
	}
	audioData.pDataAudio = (BYTE*)malloc(audioData.sizeAudio);
	hr = ReadChunkData(hFile, audioData.pDataAudio, audioData.sizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return audioData;
	}

	// ソースボイスの生成
	hr = this->pXAudio2->CreateSourceVoice(&audioData.sorceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return audioData;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audioData.sizeAudio;
	buffer.pAudioData = audioData.pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	switch (type)
	{
	case SoundType::BGM:
		buffer.LoopCount = -1;

		break;
	case SoundType::SE:
		buffer.LoopCount = 0;

		break;
	case SoundType::VOICE:
		buffer.LoopCount = 0;

		break;
	default:
		break;
	}

	audioData.type = type;

	// オーディオバッファの登録
	audioData.sorceVoice->SubmitSourceBuffer(&buffer);


	// ファイルを閉じる
	CloseHandle(hFile);

	return audioData;

}

void SoundEngine::LoadSoundData(AudioData* pData, string filePath, SoundType type)
{
	HWND hWnd = pGameEngine->GetWindowHandle();
	HRESULT hr;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &pData->sizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	pData->pDataAudio = (BYTE*)malloc(pData->sizeAudio);
	hr = ReadChunkData(hFile, pData->pDataAudio, pData->sizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}

	// ソースボイスの生成
	hr = this->pXAudio2->CreateSourceVoice(&pData->sorceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = pData->sizeAudio;
	buffer.pAudioData = pData->pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	switch (type)
	{
	case SoundType::BGM:
		buffer.LoopCount = -1;

		break;
	case SoundType::SE:
		buffer.LoopCount = 0;

		break;
	case SoundType::VOICE:
		buffer.LoopCount = 0;

		break;
	default:
		break;
	}
	
	pData->type = type;

	// オーディオバッファの登録
	pData->sorceVoice->SubmitSourceBuffer(&buffer);

	
	// ファイルを閉じる
	CloseHandle(hFile);

	return;
}


HRESULT SoundEngine::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT SoundEngine::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
void SoundEngine::StartSound(AudioData* audioData)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audioData->sizeAudio;
	buffer.pAudioData = audioData->pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;
	switch (audioData->type)
	{
	case SoundType::BGM:
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

		break;
	case SoundType::SE:
		buffer.LoopCount = 0;

		break;
	case SoundType::VOICE:
		buffer.LoopCount = 0;

		break;
	default:
		break;
	}


	// 状態取得
	audioData->sorceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		audioData->sorceVoice->Stop(0);

		// オーディオバッファの削除
		audioData->sorceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	audioData->sorceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	audioData->sorceVoice->Start(0);

}

void SoundEngine::StopSound(AudioData* audioData)
{

	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	audioData->sorceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		audioData->sorceVoice->Stop(0);

		// オーディオバッファの削除
		audioData->sorceVoice->FlushSourceBuffers();
	}

}

void SoundEngine::StopAllSound(void)
{

}




