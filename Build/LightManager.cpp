#include "LightManager.h"
#include "GameEngine.h"
#include "renderer.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "transformcomponent.h"
#include "World.h"

LightManager::LightManager(World* world)
{
	this->pWorld = world;
	this->pGameEngine = world->GetGameEngine();
	this->cBufferManager = pGameEngine->GetCBufferManager();
}

LightManager::~LightManager()
{
	if (direcLightBuffer) direcLightBuffer->Release();
	if (pointLightBuffer) pointLightBuffer->Release();
}

void LightManager::Awake()
{

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(DIREC_LIGHT_CBUFFER);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pGameEngine->GetRenderer()->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->direcLightBuffer);
	pGameEngine->GetCBufferManager()->SetDirectionalLightBuffer(this->direcLightBuffer);

	hBufferDesc.ByteWidth = sizeof(POINT_LIGHT_CBUFFER);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pGameEngine->GetRenderer()->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->pointLightBuffer);
	pGameEngine->GetCBufferManager()->SetPointLightBuffer(this->pointLightBuffer);

	direcLightCBufferStruct.allEnable.x = 1;
	pointLightCBufferStruct.allEnable.x = 1;
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(pointLightBuffer, 0, NULL, &pointLightCBufferStruct, 0, 0);
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(direcLightBuffer, 0, NULL, &direcLightCBufferStruct, 0, 0);



}

void LightManager::Update()
{
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(direcLightBuffer, 0, NULL, &direcLightCBufferStruct, 0, 0);

}

void LightManager::ClearLight(void)
{
	direcLightList.clear();
	pointLightList.clear();
}



void LightManager::SetLightNear(XMFLOAT3 pos)
{
	XMVECTOR p1 = XMLoadFloat3(&pos);
	 vector< pair<float, PointLightComponent*>> lightDistances;

	// 全てのライトとの距離を計算して配列に保存する

	 for (PointLightComponent* com : pointLightList)
	 {
		 XMVECTOR p2 = XMLoadFloat3(&com->GetTransFormComponent()->GetWorldPos());
		 XMVECTOR len = p2 - p1;

		 float fLen;
		 XMStoreFloat(&fLen, XMVector3Length(len));

		 lightDistances.push_back(make_pair(fLen, com));
	 }

	// 距離でソートする
	 sort(lightDistances.begin(), lightDistances.end(),
		[](const  pair<float, PointLightComponent*>& a, const  pair<float, PointLightComponent*>& b) {
			return a.first < b.first;
		});

	// 近い順に8つのライトを設定する
	for (int i = 0; i < MAX_POINT_LIGHT && i < lightDistances.size(); i++)
	{
		PointLightComponent* nearestLight = lightDistances[i].second;
		SetPointLight(nearestLight, i);
	}
}


int LightManager::AddLight(DirectionalLightComponent* com)
{
	direcLightList.push_back(com);
	if (direcLightList.size() <= MAX_DIREC_LIGHT)
	{
		SetDirecLight(com, (int)direcLightList.size() - 1);
	}
	return (int)direcLightList.size() - 1;

}

int LightManager::AddLight(PointLightComponent* com)
{
	pointLightList.push_back(com);
	return (int)pointLightList.size() - 1;

}

void LightManager::RemoveLight(DirectionalLightComponent* com)
{
	direcLightList.remove(com);
}

void LightManager::RemoveLight(PointLightComponent* com)
{
	pointLightList.remove(com);
}




void LightManager::SetPointLight(PointLightComponent* pointlightComponent, int index)
{
	this->pointLightCBufferStruct.m_Position[index] = pointlightComponent->GetLightParam().m_Position;
	this->pointLightCBufferStruct.m_Diffuse[index] = pointlightComponent->GetLightParam().m_Diffuse;
	this->pointLightCBufferStruct.m_Ambient[index] = pointlightComponent->GetLightParam().m_Ambient;
	this->pointLightCBufferStruct.m_Attenuation[index] = pointlightComponent->GetLightParam().m_Attenuation;
	this->pointLightCBufferStruct.m_intensity[index] = pointlightComponent->GetLightParam().m_intensity;
	this->pointLightCBufferStruct.m_Enable[index].x = pointlightComponent->GetLightParam().m_Enable;

	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(pointLightBuffer, 0, NULL, &pointLightCBufferStruct, 0, 0);

}
void LightManager::SetDirecLight(DirectionalLightComponent* direclightComponent, int index)
{
	this->direcLightCBufferStruct.m_Direction[index] = direclightComponent->GetLightParam().m_Direction;
	this->direcLightCBufferStruct.m_Diffuse[index] = direclightComponent->GetLightParam().m_Diffuse;
	this->direcLightCBufferStruct.m_Ambient[index] = direclightComponent->GetLightParam().m_Ambient;
	this->direcLightCBufferStruct.m_Enable[index].x = direclightComponent->GetLightParam().m_Enable;

	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(direcLightBuffer, 0, NULL, &direcLightCBufferStruct, 0, 0);

}

DirectionalLightComponent* LightManager::GetMainLight(void)
{
	for (DirectionalLightComponent* com:direcLightList)
	{
		if (com->GetActive())
		{
			return com;
		}
	}
	return nullptr;

}

void LightManager::SetAllEnable(BOOL flag)
{
	direcLightCBufferStruct.allEnable.x = flag;
	pointLightCBufferStruct.allEnable.x = flag;
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(pointLightBuffer, 0, NULL, &pointLightCBufferStruct, 0, 0);
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(direcLightBuffer, 0, NULL, &direcLightCBufferStruct, 0, 0);

}
