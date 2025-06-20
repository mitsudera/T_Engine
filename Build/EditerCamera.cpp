#include "EditerCamera.h"
#include "World.h"
#include "GameEngine.h"
#include "transformcomponent.h"
#include "renderer.h"
#include "input.h"
#include "CBufferManager.h"
#include "primitivecomponent.h"
#include "SkySphere.h"
#include "Scene.h"
#include "Material.h"
#include "AssetsManager.h"
#include "ShaderSet.h"
#include "DebugUtility.h"
#include "ProjectSetting.h"

EditerCamera::EditerCamera(World* world)
{
	pWorld = world;
	pGameEngine = world->GetGameEngine();
	pRenderer = pGameEngine->GetRenderer();
	input = pGameEngine->GetInput();
	pProjectSetting = pGameEngine->GetProjectSetting();
	pos = XMVectorZero();
	axisX = xonevec();
	axisY = yonevec();
	axisZ = zonevec();
	mtxpos = XMMatrixIdentity();
	quaternion = XMQuaternionIdentity();


	//camerasetting




	// 仮
	this->aspect = 16.0f / 9.0f;	// アスペクト比 
	this->angle = XMConvertToRadians(90.0f);	// 視野角
	this->nearZ = 1.0f;
	this->farZ = 10000.0f;

	this->len = 50.0f;


	this->clearColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	// ビューポートタイプの初期化
	XMFLOAT2 screenSize = pGameEngine->GetWindowSize();


	// ビューポート設定
	vp.Width = (FLOAT)screenSize.x;
	vp.Height = (FLOAT)screenSize.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->mtxProj = XMMatrixPerspectiveFovLH(this->angle, this->aspect, this->nearZ, this->farZ);


}

EditerCamera::~EditerCamera()
{
}

void EditerCamera::Init(void)
{
	layerCulling.push_back(pProjectSetting->GetLayer("Sky"));
	layerCulling.push_back(pProjectSetting->GetLayer("Text"));

}

void EditerCamera::Update(void)
{

	float dt = pGameEngine->GetDeltaTime();
	if (input->GetKeyboardPress(DIK_W))
	{
		Move(axisZ, 100.0f * dt);
	}
	if (input->GetKeyboardPress(DIK_S))
	{
		Move(axisZ, -100.0f * dt);

	}
	if (input->GetKeyboardPress(DIK_D))
	{
		Move(axisX, 100.0f * dt);

	}
	if (input->GetKeyboardPress(DIK_A))
	{
		Move(axisX, -100.0f * dt);

	}
	if (input->GetKeyboardPress(DIK_E))
	{
		Move(axisY, 100.0f * dt);

	}
	if (input->GetKeyboardPress(DIK_Q))
	{
		Move(axisY, -100.0f * dt);

	}
	if (input->IsMouseRightPressed())
	{
		float x, y;

		x = (float)input->GetMouseX();
		y = (float)input->GetMouseY();


		x *= dt*0.1f;
		y *= dt*0.1f;

		Rotate(x, yonevec());
		Rotate(y, axisX);

	}
	else if (input->IsMouseCenterPressed())
	{
		float x, y;

		x = (float)input->GetMouseX();
		y = (float)input->GetMouseY();


		x *= dt;
		y *= dt;

		Move(axisX, -x * 5.0f);
		Move(axisY, y * 5.0f);

	}

	{
		float wheel = (float)(input->GetMouseZ()) * 2.0f;
		{
			Move(axisZ, wheel * dt);
		}
	}

	//skyComArray.clear();
	//for (Scene* scene : pWorld->GetActiveSceneList())
	//{

	//	for (PrimitiveComponent* com : scene->GetAllPrimitiveComponent())
	//	{
	//		if (com->GetGameObject()->GetLayer() == GameObject::Layer::Sky)
	//		{
	//			SetSkyCom(scene->GetGameObject<SkySphere>());

	//		}
	//	}

	//}


}

void EditerCamera::Render(void)
{
	//レンダリング


	XMFLOAT3 wpos;
	XMStoreFloat3(&wpos, pos);


	this->mtxView = XMMatrixLookToLH(pos, axisZ, axisY);

	pGameEngine->GetCBufferManager()->SetCameraBuffer(&wpos);


	pGameEngine->GetCBufferManager()->SetProjectionMtx(&this->mtxProj);


	//ビューポートセット
	pRenderer->GetDeviceContext()->RSSetViewports(1, &vp);


	//描画ターゲットのセット
	pRenderer->SetRenderTargetBackBuffer();
	const float cc[4] = { clearColor.x,clearColor.y,clearColor.z,clearColor.w };



	//スカイスフィアで背景クリアをする場合

	pRenderer->SetDepthEnable(FALSE);


	XMMATRIX cMtx = this->mtxView;
	cMtx.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	pGameEngine->GetCBufferManager()->SetViewMtx(&cMtx);
	for (Scene* scene : pWorld->GetActiveSceneList())
	{
		for (PrimitiveComponent* com : scene->GetAllPrimitiveComponent())
		{
			if (com->GetGameObject()->GetLayer() == pProjectSetting->GetLayer("Sky"))
			{
				com->GetMaterial()->GetShaderSet()->SetShaderRenderer();
				com->Draw();

			}
		}
	}



	pRenderer->SetDepthEnable(TRUE);






	pGameEngine->GetCBufferManager()->SetViewMtx(&this->mtxView);
	XMFLOAT2 screenSize =pGameEngine->GetWindowSize();
	viewProj = XMMatrixMultiply(this->mtxView, this->mtxProj);
	//シェーダー毎に描画
	for (int i = 0; i < ShaderSet::ShaderIndex::MAXShader; i++)
	{
		pGameEngine->GetAssetsManager()->SetShader((ShaderSet::ShaderIndex)i);

		for (Scene* scene : pWorld->GetActiveSceneList())
		{
			for (PrimitiveComponent* com : scene->GetAllPrimitiveComponent())
			{



				if (!com->GetActive())
					continue;

				if (com->GetAlphaTest())
					continue;

				//レイヤーのカリングチェック
				auto it = find(layerCulling.begin(), layerCulling.end(), com->GetGameObject()->GetLayer());
				if (it!=layerCulling.end())
					continue;

				if (com->GetMaterial() == nullptr)
				{
					com->Draw();
					continue;

				}

				//現在セットしてるシェーダーを使っている場合描画
				if (com->GetMaterial()->GetShaderSet()->GetShaderIndex() != i)
					continue;


				//if (com->GetIsFrustumCulling())
				//{
				//	if (this->FrustumCulling(com->GetBoxMin(),com->GetBoxMax()))
				//	{
				//		continue;
				//	}
				//}



				com->Draw();


			}

		}

	}



#ifdef _DEBUG

	if (pWorld->GetShowGui())
	{
		pGameEngine->GetDebugUtility()->SetDebugLineShader();

		for (Scene* scene : pWorld->GetActiveSceneList())
		{

			for (Component* com : scene->GetAllComponent())
			{
				if (!com->GetActive())
					continue;
				com->DebugDraw();
			}
		}

	}
#endif

	//シェーダー毎に描画
	for (int i = 0; i < ShaderSet::ShaderIndex::MAXShader; i++)
	{
		pGameEngine->GetAssetsManager()->SetShader((ShaderSet::ShaderIndex)i);

		for (Scene* scene : pWorld->GetActiveSceneList())
		{

			for (PrimitiveComponent* com : scene->GetAllPrimitiveComponent())
			{
				if (!com->GetActive())
					continue;

				if (!com->GetAlphaTest())
					continue;


				//レイヤーのカリングチェック
				auto it = find(layerCulling.begin(), layerCulling.end(), com->GetGameObject()->GetLayer());
				if (it != layerCulling.end())
					continue;

				if (com->GetMaterial() == nullptr)
				{
					com->Draw();
					continue;

				}

				//現在セットしてるシェーダーを使っている場合描画
				if (com->GetMaterial()->GetShaderSet()->GetShaderIndex() != i)
					continue;



				if (com->GetIsFrustumCulling())
				{
					XMVECTOR oPos = XMLoadFloat3(&com->GetWorldPos());
					XMVECTOR op = XMVector3Normalize(pos - oPos);

					float dot = XMVectorGetX(XMVector3Dot(op, axisZ));
					if (dot > 0.0f)
						continue;


				}



				com->Draw();


			}
		}
	}






	pRenderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);
	for (Scene* scene : pWorld->GetActiveSceneList())
	{

		for (PrimitiveComponent* com : scene->GetAllPrimitiveComponent())
		{
			if (!com->GetActive())
				continue;

			//レイヤーのカリングチェック
			if (com->GetGameObject()->GetLayer() != pProjectSetting->GetLayer("Text"))
				continue;



			com->Draw();


		}
	}


}

void EditerCamera::FocusObject(GameObject* gameObject)
{
	XMVECTOR focusPos = XMLoadFloat3(&gameObject->GetTransFormComponent()->GetWorldPos());
	
	pos = focusPos + (-axisZ * 300.0f);

}

XMMATRIX EditerCamera::GetProjectionMatrix(void)
{
	return this->mtxProj;
}

XMMATRIX EditerCamera::GetViewMatrix(void)
{
	return this->mtxView;
}

XMVECTOR EditerCamera::GetAxisZ(void)
{
	return axisZ;
}

XMVECTOR EditerCamera::GetPosition(void)
{
	return pos;
}


void EditerCamera::Move(XMVECTOR vec, float value)
{
	pos += (vec * value);
}

void EditerCamera::Rotate(float f, XMVECTOR axis)
{
	XMVECTOR qton = XMQuaternionRotationAxis(axis, f);
	qton = XMQuaternionNormalize(qton); // クォータニオンを正規化
	this->quaternion = XMQuaternionMultiply(this->quaternion, qton);
	RotAxis(qton);
}

void EditerCamera::RotAxis(XMVECTOR qton)
{
	this->axisX = XMVector3Rotate(this->axisX, qton);
	this->axisX = XMVector3Normalize(this->axisX);
	this->axisY = XMVector3Rotate(this->axisY, qton);
	this->axisY = XMVector3Normalize(this->axisY);
	this->axisZ = XMVector3Rotate(this->axisZ, qton);
	this->axisZ = XMVector3Normalize(this->axisZ);

}


void EditerCamera::SetSkyCom(GameObject* sky)
{
	PrimitiveComponent* com = sky->GetComponent<PrimitiveComponent>();
	if (com != nullptr)
		skyComArray.push_back(com);

	for (GameObject* obj : sky->GetChild())
	{
		SetSkyCom(obj);
	}
}

