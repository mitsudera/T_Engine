#include "PostEffectShader.h"
#include "renderer.h"
#include "RenderTexture.h"
#include "GameEngine.h"
PostEffectShader::PostEffectShader()
{
	texSize.x = 1920.0f;
	texSize.y = 1080.0f;
}

PostEffectShader::~PostEffectShader()
{

	delete renderTex1;
	delete renderTex2;

	if (cBuffer) cBuffer->Release();

	for (ID3D11PixelShader* ps : PSArray)
	{
		ps->Release();

	}

	
}

void PostEffectShader::Init(void)
{
	pCBufferManager = pRenderer->GetGameEngine()->GetCBufferManager();
	//this->texSize = pRenderer->GetGameEngine()->GetWindowSize();
	renderTex1 = new RenderTexture(pRenderer->GetGameEngine()->GetAssetsManager());
	renderTex2 = new RenderTexture(pRenderer->GetGameEngine()->GetAssetsManager());
	renderTex1->CreateRenderTexture((int)texSize.x, (int)texSize.y, "postEffectBuffer1");
	renderTex2->CreateRenderTexture((int)texSize.x, (int)texSize.y, "postEffectBuffer2");

}

void PostEffectShader::PostEffectDraw(ID3D11ShaderResourceView* srv,ID3D11RenderTargetView* rtv)
{
	pRenderer->SetDepthEnable(FALSE);
	for (int i = 0; i < PSArray.size(); i++)
	{
		if(i%2==1)
		{
			renderTex1->SetRTV(RenderTexture::BindMode::Texture,0);
			renderTex2->SetPSSRV(RenderTexture::BindMode::Texture, 0);
		}
		else if(i%2==0)
		{
			renderTex2->SetRTV(RenderTexture::BindMode::Texture, 0);
			renderTex1->SetPSSRV(RenderTexture::BindMode::Texture, 0);

		}

		if (i == 0)
		{
			pRenderer->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
		}

		if (i == PSArray.size() - 1)
		{
			pRenderer->GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);
		}


		pRenderer->GetDeviceContext()->PSSetShader(PSArray[i], NULL, 0);
		pRenderer->DrawFullScreen();

		ID3D11ShaderResourceView* nullSrv = nullptr;
		pRenderer->GetDeviceContext()->PSSetShaderResources(0, 1, &nullSrv);
		pRenderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);

	}
}

void PostEffectShader::SetTexSize(float w, float h)
{
	texSize.x = w;
	texSize.y = h;
}

void PostEffectShader::AddShader(string filePath, string shaderName)
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3D11PixelShader* ps;

	HRESULT hr = D3DX11CompileFromFile(filePath.c_str(), NULL, NULL, shaderName.c_str(), "ps_5_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &ps);
	//‰ð•ú
	pPSBlob->Release();
	PSArray.push_back(ps);

}
