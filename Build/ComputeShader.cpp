#include "ComputeShader.h"
#include "renderer.h"
#include "GameEngine.h"
ComputeShader::ComputeShader()
{
}
ComputeShader::ComputeShader(Renderer* renderer)
{
	this->pRenderer = renderer;
	this->pCBufferManager = renderer->GetGameEngine()->GetCBufferManager();
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::SetCS(void)
{
	pRenderer->GetDeviceContext()->CSSetShader(CS, nullptr, 0);
}


void ComputeShader::CreateCS(string filePath, string shaderName)
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pCSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DX11CompileFromFile(filePath.c_str(), NULL, NULL, shaderName.c_str(), "cs_5_0", shFlag, 0, NULL, &pCSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "CS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreateComputeShader(pCSBlob->GetBufferPointer(), pCSBlob->GetBufferSize(), NULL, &this->CS);
	//‰ð•ú
	pCSBlob->Release();

}

void ComputeShader::Destroy(void)
{
	if(CS) CS->Release();

}
