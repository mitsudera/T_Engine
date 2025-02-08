#include "FullScreenQuadVertex.h"
#include "renderer.h"
#include "ShaderSet.h"
FullScreenQuadVertex::FullScreenQuadVertex(Renderer* renderer)
{
	pRenderer = renderer;


	struct vert
	{
		XMFLOAT3	Position;
		XMFLOAT4	Diffuse;
		XMFLOAT2	TexCoord;

	};

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vert) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);



	vert vertexArray[4];


	vertexArray[0].Position = { -1.0f,1.0f,0.0f };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { 1.0f,1.0f,0.0f };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { -1.0f,-1.0f ,0.0f };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { 1.0f ,-1.0f,0.0f };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->pRenderer->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	vert* pVtx = (vert*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(vert) * 4);

	this->pRenderer->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


	// 頂点シェーダコンパイル・生成
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DX11CompileFromFile("shaders/FullScreenVertexShader.hlsl", NULL, NULL, "VS_Main", "vs_5_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}

	pRenderer->GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &this->vs);


	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	pRenderer->GetDevice()->CreateInputLayout(layout,
		numElements,
		(pVSBlob)->GetBufferPointer(),
		(pVSBlob)->GetBufferSize(),
		&VertexLayout);

	(pVSBlob)->Release();



}

FullScreenQuadVertex::~FullScreenQuadVertex()
{
	if (this->vertexBuffer) this->vertexBuffer->Release();
	if (this->VertexLayout) this->VertexLayout->Release();
	if (this->vs)this->vs->Release();
}

void FullScreenQuadVertex::Draw(void)
{
	pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);
	struct vert
	{
		XMFLOAT3	Position;
		XMFLOAT4	Diffuse;
		XMFLOAT2	TexCoord;

	};
	//頂点入力レイアウトをセット
	if (this->VertexLayout) pRenderer->GetDeviceContext()->IASetInputLayout(this->VertexLayout);

	// 頂点バッファ設定
	UINT stride = sizeof(vert);
	UINT offset = 0;
	pRenderer->GetDeviceContext()->VSSetShader(vs, NULL, 0);
	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pRenderer->GetDeviceContext()->Draw(4, 0);

}

