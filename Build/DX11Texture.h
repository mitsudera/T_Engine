#pragma once
#include "Assets.h"

#include "CoreMinimal.h"

class AssetsManager;

class DX11Texture :public Assets
{
	AssetsManager* pManager;
	ID3D11ShaderResourceView* SRV;

public:
	DX11Texture(AssetsManager* Manager);
	~DX11Texture();

	ID3D11ShaderResourceView* GetTexture(void);
	void CreateSRV(char* path);
	void CreateSRV(string path);


	void SetShaderResourcePS(int n);
	void SetShaderResourceDS(int n);

	void SetManager(AssetsManager* Manager);

private:

};
