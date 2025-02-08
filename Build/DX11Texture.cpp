#include "DX11Texture.h"

#include "AssetsManager.h"
#include "renderer.h"
#include "GameEngine.h"
DX11Texture::DX11Texture(AssetsManager* Manager)
{
	pManager = Manager;
	this->SRV = nullptr;


}

DX11Texture::~DX11Texture()
{
	if (this->SRV)	SRV->Release();
}


ID3D11ShaderResourceView* DX11Texture::GetTexture(void)
{
	return this->SRV;
}
void DX11Texture::CreateSRV(char* path)
{
	string fpath = path;

	CreateSRV(fpath);
}

void DX11Texture::CreateSRV(string path)
{


	ID3D11ShaderResourceView* newSrv;

	wstring wstr(path.begin(), path.end());

	HRESULT hr = CreateWICTextureFromFile(
		this->pManager->GetGameEngine()->GetRenderer()->GetDevice(),
		wstr.c_str(),
		nullptr,
		&newSrv);

	// WICでの読み込みが失敗した場合（主にddsファイルの時）
	if (FAILED(hr))
	{
		hr = CreateDDSTextureFromFile(
			this->pManager->GetGameEngine()->GetRenderer()->GetDevice(),
			wstr.c_str(),
			nullptr,
			&newSrv);
	}


	//srvの生成に成功
	if (newSrv)
	{
		if (SRV) SRV->Release();

		SRV = newSrv;
		this->path = path;

	}


}
void DX11Texture::SetShaderResourcePS(int n)
{
	this->pManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->PSSetShaderResources(n, 1, &SRV);
}

void DX11Texture::SetShaderResourceDS(int n)
{
	this->pManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->DSSetShaderResources(n, 1, &SRV);

}

void DX11Texture::SetManager(AssetsManager* Manager)
{
	this->pManager = Manager;
}

