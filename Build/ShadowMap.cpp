#include "ShadowMap.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "RenderTexture.h"
#include "renderer.h"
#include "CBufferManager.h"
#include "primitivecomponent.h"
#include "DirectionalLightComponent.h"
#include "LightManager.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "gameobject.h"
#include "ShadowShader.h"
#include "transformcomponent.h"
#include "GausianBlurShader.h"
#include "SkinMeshShadowShader.h"
#include "ShadowShader.h"
#include "Material.h"
#include "World.h"
#include "DirectionalLightComponent.h"
#include "EditerCamera.h"

ShadowMap::ShadowMap(World* world)
{
	pWorld = world;
	pGameEngine = world->GetGameEngine();
	pAssetsManager = pGameEngine->GetAssetsManager();
	pRenderer = pGameEngine->GetRenderer();
	pCBufferManager = pGameEngine->GetCBufferManager();
	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(ShadowCbuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pRenderer->GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &this->shadowBuffer);
	pCBufferManager->SetShadowBuffer(this->shadowBuffer);
	quality = ShadowQuality::High;
	vhwn = 1024.0f;
	vhwf = 1024.0f;
	variance = TRUE;

	vNear = 0.0f;
	vFar = 1024.0f*2.0f;

}

ShadowMap::~ShadowMap()
{
	if (shadowBuffer) shadowBuffer->Release();
	delete this->blurShader;
}

void ShadowMap::CreateShadowMap(ShadowQuality quality)
{
	int w = 64;
	int h = 64;

	switch (quality)
	{
	case ShadowQuality::Low:
		w = 1024;
		h = 1024;
		break;

	case ShadowQuality::Middle:
		w = 2048;
		h = 2048;
		break;

	case ShadowQuality::High:
		w = 4096;
		h = 4096;
		break;

	}

	this->shadowNearTexture = pAssetsManager->CreateRenderTexture(w, h, "ShadowMapNear");
	this->shadowNearResultTexture = pAssetsManager->CreateRenderTexture(w, h, "ShadowMapNearResult");
	this->shadowFarTexture = pAssetsManager->CreateRenderTexture(w, h, "ShadowMapFar");

	// ビューポートの設定
	ViewPortShadowMap.TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	ViewPortShadowMap.TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	ViewPortShadowMap.Width = (float)w ;	// ビューポート領域の幅
	ViewPortShadowMap.Height = (float)h;	// ビューポート領域の高さ
	ViewPortShadowMap.MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	ViewPortShadowMap.MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値

	blurShader = new GausianBlurShader(pRenderer);
	blurShader->SetTexSize((float)w, (float)h);
	blurShader->Init();


}

void ShadowMap::ShadowMapping(void)
{


	this->shadowBufferStruct.mode = 1;
	this->shadowBufferStruct.facter = (0.01f);
	this->shadowBufferStruct.bias = (-0.005f);

	XMMATRIX view;
	XMMATRIX proj;

	XMVECTOR mapPos;
	XMVECTOR camPos;
	XMVECTOR lDir;
	XMVECTOR lUp;
	XMVECTOR camv;
	CameraComponent* mainCam = pWorld->GetMainCamera();
	DirectionalLightComponent* mainLight = pWorld->GetLightmanager()->GetMainLight();

	if (pGameEngine->GetEditerMode())
	{
		camv = pWorld->GetEditerCamera()->GetAxisZ();
		camv.m128_f32[1] = 0.0f;
		camv = XMVector3Normalize(camv);

		camPos = pWorld->GetEditerCamera()->GetPosition();

	}
	else
	{
		if (mainCam)
		{
			camv = XMVector3TransformNormal(pWorld->GetMainCamera()->GetTransFormComponent()->GetAxisZ(), pWorld->GetMainCamera()->GetTransFormComponent()->GetWorldMtx());
			camv.m128_f32[1] = 0.0f;
			camv = XMVector3Normalize(camv);

			camPos = XMLoadFloat3(&pWorld->GetMainCamera()->GetWorldPos());
			if (pWorld->GetMainCamera()->GetTrackingMode() != CameraComponent::TrackingMode::NONE)
			{
				camPos = XMLoadFloat3(&pWorld->GetMainCamera()->GetAtPos());
			}

		}
		else
		{
			camPos = XMVectorZero();
			camv = zonevec();
		}

	}

	if (mainLight)
	{
		lDir = pWorld->GetLightmanager()->GetMainLight()->GetTransFormComponent()->GetAxisZ();
		lUp = pWorld->GetLightmanager()->GetMainLight()->GetTransFormComponent()->GetAxisY();
		lDir = XMVector3Normalize(lDir);

	}
	else
	{
		lDir = -yonevec();
		lUp = zonevec();

	}





	mapPos = camPos - (lDir * vFar * 0.7f) + (camv * 250.0f);




	view = XMMatrixLookToLH(mapPos, lDir, lUp);

	proj = XMMatrixOrthographicLH(vhwn, vhwn, vNear, vFar);
	//proj = XMMatrixPerspectiveFovLH(90.0f, 1.0f, vNear, vFar);
	this->shadowNearTexture->ClearRTV(XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f));
	this->shadowNearTexture->ClearDSV(1.0f);


	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	pRenderer->GetDeviceContext()->PSSetShaderResources(3, 1, nullSRV);
	pRenderer->GetDeviceContext()->PSSetShaderResources(4, 1, nullSRV);

	this->shadowNearTexture->SetRTV(RenderTexture::BindMode::BOTH, 0);


	pRenderer->GetDeviceContext()->RSSetViewports(1,&ViewPortShadowMap);

	
	pCBufferManager->SetViewMtx(&view);
	pCBufferManager->SetProjectionMtx(&proj);

	this->shadowBufferStruct.wvpn = XMMatrixTranspose(XMMatrixIdentity() * view * proj);
	pAssetsManager->GetShadowShader()->SetShaderRenderer();

	//シェーダー毎に描画
	for (int i = 0; i < ShaderSet::ShadowShaderIndex::MAXShadowShader; i++)
	{
		pGameEngine->GetAssetsManager()->SetShadowShader((ShaderSet::ShadowShaderIndex)i);

		for  (Scene* scene:pWorld->GetActiveSceneList())
		{
			for (PrimitiveComponent* com : scene->GetAllPrimitiveComponent())
			{
				if (!com->GetActive() || !com->GetHasShadow())
					continue;


				Material* matShadow = com->GetShadowMaterial();

				if (matShadow == nullptr)
				{
					com->ShadowMapping();
					continue;
				}

				if (matShadow->GetShaderSet()->GetShadowShaderIndex() != i)
					continue;


				if (com->GetIsFrustumCulling())
				{
					XMVECTOR oPos = XMLoadFloat3(&com->GetWorldPos());
					XMVECTOR op = XMVector3Normalize(oPos - mapPos);

					float dot = XMVectorGetX(XMVector3Dot(op, lDir));
					if (dot < 0.0f)
						continue;


				}


				com->ShadowMapping();

			}

		}

	}
	pRenderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);



	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, NULL, &shadowBufferStruct, 0, 0);

	if (variance)
	{
		//分散シャドウマップならブラーをかける


		blurShader->PostEffectDraw(this->shadowNearTexture->GetSRV(), this->shadowNearResultTexture->GetRenderTargetView());
		this->shadowNearResultTexture->SetPSSRV(RenderTexture::BindMode::Texture, 3);

	}
	else
	{
		this->shadowNearTexture->SetPSSRV(RenderTexture::BindMode::DepthTexture, 3);

	}

}


void ShadowMap::SetFar(float vFar)
{
	this->vFar = vFar;
}

void ShadowMap::SetNear(float vNear)
{
	this->vNear = vNear;
}

void ShadowMap::SetEnable(BOOL enable)
{
	this->shadowBufferStruct.enable = enable;
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, NULL, &shadowBufferStruct, 0, 0);

}

void ShadowMap::SetVariance(BOOL enable)
{
	this->variance = enable;
	this->shadowBufferStruct.mode = variance;
	pGameEngine->GetRenderer()->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, NULL, &shadowBufferStruct, 0, 0);

}
