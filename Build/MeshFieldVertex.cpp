#include "MeshFieldVertex.h"
#include "ShaderSet.h"
#include "renderer.h"

MeshFieldVertex::MeshFieldVertex(Renderer* renderer)
{
	this->pRenderer = renderer;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);

	VERTEX_3D vertexArray[4];


	vertexArray[0].Position = { -1.0f,0.0f,1.0f };
	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[0].TexCoord = { 0.0f,0.0f };


	vertexArray[1].Position = { 1.0f,0.0f,1.0f };
	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[1].TexCoord = { 1.0f,0.0f };

	vertexArray[2].Position = { -1.0f,0.0f ,-1.0f };
	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[2].TexCoord = { 0.0f,1.0f };

	vertexArray[3].Position = { 1.0f ,0.0f,-1.0f };
	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	vertexArray[3].TexCoord = { 1.0f,1.0f };

	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->pRenderer->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->pRenderer->GetDeviceContext()->Unmap(this->vertexBuffer, 0);

}

MeshFieldVertex::~MeshFieldVertex()
{
	if (this->vertexBuffer) this->vertexBuffer->Release();

}

void MeshFieldVertex::SetVertexBuffer()
{

	pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pRenderer->GetDeviceContext()->Draw(4, 0);

}

