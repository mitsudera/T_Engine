#pragma once
#include "Coreminimal.h"
#include "ShadowMap.h"

class GameEngine;
class AssetsManager;
class Renderer;
class CBufferManager;
class GausianBlurShader;
class World;
struct ShadowCbuffer
{
	XMMATRIX wvpn;
	XMMATRIX wvpf;
	int enable;
	int mode;
	float facter;
	float bias;
};

class ShadowMap
{

public:

	enum class ShadowQuality :int
	{
		Low,
		Middle,
		High,
	};

	ShadowMap(World* world);
	~ShadowMap();

	void CreateShadowMap(ShadowQuality quality);
	void ShadowMapping(void);

	int GetNearShadowTexIndex(void);
	int GetFarShadowTexIndex(void);

	void SetFar(float vFar);
	void SetNear(float vNear);

	void SetEnable(BOOL enable);
	void SetVariance(BOOL enable);

private:
	World* pWorld;
	GameEngine* pGameEngine;
	AssetsManager* pAssetsManager;
	Renderer* pRenderer;
	CBufferManager* pCBufferManager;
	int shadowNearTextureIndex;
	int shadowNearResultTextureIndex;
	int shadowFarTextureIndex;
	int shadowFarResultTextureIndex;
	ShadowQuality quality;
	ID3D11Buffer* shadowBuffer;
	ShadowCbuffer shadowBufferStruct;

	GausianBlurShader* blurShader;

	float vhwn;
	float vhwf;


	float vFar;
	float vNear;

	D3D11_VIEWPORT            ViewPortShadowMap;       // ビューポート

	BOOL variance;//分散シャドウマップ


};

