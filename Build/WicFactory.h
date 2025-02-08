#pragma once
#include "Coreminimal.h"

class GameEngine;

struct FileTexture
{
	~FileTexture();

	XMFLOAT4* color;
	UINT width;
	UINT height;
	string filePath;

	XMFLOAT4 sampling(XMFLOAT2 TexCoord);

};

class WicFactory
{
public:
	WicFactory(GameEngine* gameEngine);
	~WicFactory();
	
	FileTexture* LoadFileTexture(string filePath);

private:
	IWICImagingFactory* pWICFactory;
	GameEngine* pGameEngine;

};

