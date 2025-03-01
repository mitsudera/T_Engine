#include "TerrainComponent.h"
#include "renderer.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "CBufferManager.h"
#include "AssetsManager.h"
#include "Material.h"
#include "NotEffectShader.h"
#include "GameEngine.h"
#include "TerrainMaterial.h"
TerrainComponent::TerrainComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

TerrainComponent::~TerrainComponent()
{
}

void TerrainComponent::Awake(void)
{
	PrimitiveComponent::Awake();

	TypeName = typeid(TerrainComponent).name();;

	

}

void TerrainComponent::Update(void)
{
	PrimitiveComponent::Update();

}

void TerrainComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
	
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
	if (heightMapSRV) heightMapSRV->Release();
	delete[] heightMap->color;
	delete heightMap;
	delete[] vertexArray;
}

void TerrainComponent::Draw(void)
{
	PrimitiveComponent::Draw();
	if (this->alphaTest == TRUE)
	{
		this->pRenderer->SetAlphaTestEnable(TRUE);
	}



	this->pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

	UINT stride = sizeof(VERTEX_3D);
	UINT worldOffset = 0;

	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &worldOffset);
	// インデックスバッファ設定
	pRenderer->GetDeviceContext()->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	
	XMMATRIX world = XMMatrixIdentity();
	world = this->pGameObject->GetTransFormComponent()->GetWorldMtx(world);
	pCBufferManager->SetWorldMtx(&world);


	this->material->SetBufferMaterial();
	pRenderer->GetDeviceContext()->DSSetShaderResources(5, 1, &this->heightMapSRV);

	this->pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	this->pRenderer->GetDeviceContext()->DrawIndexed(indexNum, 0, 0);

	this->pRenderer->SetAlphaTestEnable(FALSE);


}
void TerrainComponent::CreateVetexBuffer(XMINT2 resolution, XMFLOAT2 size)
{

	terrainResolution = resolution;
	terrainSize = size;
	vertNum = resolution.x * resolution.y;
	XMINT2 quadNum;
	quadNum.x = resolution.x - 1;
	quadNum.y = resolution.y - 1;

	indexNum = (quadNum.x * quadNum.y) * 4;


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);




	float oneXSize = size.x / (float)(quadNum.x);
	float oneYSize = size.y / (float)(quadNum.y);

	float oneUSize = 1.0f / (float)(quadNum.x);
	float oneVSize = 1.0f / (float)(quadNum.y);
	vertexArray = nullptr;

	vertexArray = new VERTEX_3D[vertNum];

	int vCnt = 0;
	
	

	for (int y = 0; y < resolution.y; y++)
	{
		for (int x = 0; x < resolution.x; x++)
		{
			vertexArray[vCnt].Position = { (float)((oneXSize * (float)x) - size.x * 0.5f),0.0f,-(float)((oneYSize * (float)y)-size.y*0.5f) };
			vertexArray[vCnt].Normal = { 0.0f,1.0f,0.0 };
			vertexArray[vCnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
			vertexArray[vCnt].TexCoord = { (oneUSize * (float)x),(oneVSize * (float)y) };
			vertexArray[vCnt].Tangent = { 1.0f,0.0f,0.0f };
			vertexArray[vCnt].BiNormal = { 0.0f,0.0f,-1.0f };

			vCnt++;
		}

	}



	//頂点バッファの中身を埋める

	D3D11_MAPPED_SUBRESOURCE msr;
	this->pRenderer->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * vertNum);

	this->pRenderer->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


	// インデックスバッファ生成
	
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->indexBuffer);


	int indexCnt = 0;
	
	unsigned int* indexArray = nullptr;
	indexArray = new unsigned int[indexNum];

	int test;
	for (int y = 0; y < quadNum.y; y++)
	{
		for (int x = 0; x < quadNum.x; x++)
		{
			indexArray[indexCnt] = (resolution.x * y) + x;
			test = (quadNum.x * y) + x;
			indexCnt++;
			indexArray[indexCnt] = (resolution.x * y) + x + 1;
			test = (quadNum.x * y) + x + 1;
			indexCnt++;
			indexArray[indexCnt] = (resolution.x * (y + 1)) + x;
			test = (quadNum.x * (y + 1)) + x;
			indexCnt++;
			indexArray[indexCnt] = (resolution.x * (y + 1)) + x + 1;
			test = (quadNum.x * (y + 1)) + x + 1;
			indexCnt++;
		}
	}

	//インデックスバッファの中を埋める
	this->pRenderer->GetDeviceContext()->Map(this->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	int* pIdx = (int*)msr.pData;

	memcpy(pIdx, indexArray, sizeof(int) * indexNum);

	this->pRenderer->GetDeviceContext()->Unmap(this->indexBuffer, 0);



}

void TerrainComponent::LoadHeightMap(string path)
{

	ID3D11ShaderResourceView* newSrv;

	D3DX11CreateShaderResourceViewFromFile(
		pRenderer->GetDevice(),
		path.c_str(),
		NULL,
		NULL,
		&newSrv,
		NULL);

	if (newSrv)
	{
		if (heightMapSRV)heightMapSRV->Release();

		heightMapSRV = newSrv;

		if (this->heightMap) delete heightMap;

		this->heightMap = pGameEngine->GetWicFactory()->LoadFileTexture(path);
		this->heightMapPath = path;
	}




	//XMFLOAT4 col = heightMap->sampling(XMFLOAT2(0.5f, 0.5f));
}

float TerrainComponent::GetHeight(XMFLOAT3 pos)
{
	XMFLOAT2 length;
	length.x = pos.x - GetTransFormComponent()->GetPosition().x;
	length.y = pos.z - GetTransFormComponent()->GetPosition().z;

	//範囲内にいるか？
	if (length.x > (terrainSize.x * 0.5) ||
		length.x < -(terrainSize.x * 0.5) ||
		length.y >(terrainSize.y * 0.5) ||
		length.y < -(terrainSize.y * 0.5))
	{

		return GetTransFormComponent()->GetPosition().y;

	}

	XMFLOAT2 sampleUV;

	sampleUV.x = ((terrainSize.x * 0.5f) + length.x) / terrainSize.x;
	sampleUV.y = ((terrainSize.y * 0.5f) - length.y) / terrainSize.y;

	TerrainMaterial* mat = dynamic_cast<TerrainMaterial*>(this->material);

	float heighFacter = mat->GetHeightFacter();

	float height = heightMap->sampling(sampleUV).x * heighFacter;

	return GetTransFormComponent()->GetWorldPos().y + height;
}

XMINT2 TerrainComponent::GetResolution(void)
{
	return terrainResolution;
}

XMFLOAT2 TerrainComponent::GetHW(void)
{
	return this->terrainSize;
}

void TerrainComponent::SetHeightMapPath(string path)
{
	this->heightMapPath = path;
}

string TerrainComponent::GetHeightMapPath(void)
{
	return this->heightMapPath;
}

void TerrainComponent::SetHeightFacter(float facter)
{
	TerrainMaterial* mat = dynamic_cast<TerrainMaterial*>(this->material);

	mat->SetHeightFacter(facter);

}

float TerrainComponent::GetHeightFacter(void)
{
	TerrainMaterial* mat = dynamic_cast<TerrainMaterial*>(this->material);

	return mat->GetHeightFacter();
}
