//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#pragma once
#include "Component.h"
#include "gameobject.h"

class Renderer;

class MeshComponent;

class PostEffectShader;

class RenderTexture;

class PrimitiveComponent;
//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum VIEWPORT_TYPE
{
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_VIEWPORT,
	TYPE_NONE,

};


class CameraComponent : public Component
{
public:

	enum class ClearMode
	{
		None,
		Color,
		SkySphere,
	};

	struct CameraCBuffer
	{
		XMFLOAT4 camPos;
	};


	enum class TrackingMode
	{
		PARENT,
		SKY,
		NONE,
	};


	CameraComponent();
	CameraComponent(GameObject* gameObject);
	~CameraComponent();


	void SetLookObject(GameObject* gameObject);


	void SetViewPort(int m_type);
	int GetViewPortType(void);

	virtual void Awake(void) override;
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	void Render(void);


	void SetMode(TrackingMode mode);

	XMMATRIX GetView(void);

	ID3D11RenderTargetView* GetRenderTarget(void);
	void SetRenderTarget(ID3D11RenderTargetView* rtv);
	ID3D11DepthStencilView* GetDepthStencilView(void);
	void SetDepthStencilView(ID3D11DepthStencilView* dsv);

	void SetClearMode(ClearMode mode);
	void SetClearColor(XMFLOAT4 color);

	void SetSkyCom(GameObject* sky);

	void SetSky(GameObject* sky);

	void SetMainCamera(void);

	void SetPostEffect(PostEffectShader* shader);
	void SetPostEffectEnable(BOOL enable);

	void SetTrackingMode(TrackingMode mode);
	TrackingMode GetTrackingMode(void);
	XMFLOAT3 GetAtPos(void);

	void SetNear(float f);
	void SetFar(float f);

	void SetProjectionMtx(void);

	BOOL FrustumCulling(XMVECTOR min, XMVECTOR max);



private:

	D3D11_VIEWPORT vp;

	XMMATRIX mtxView;
	XMMATRIX mtxProj;
	XMMATRIX viewProj;

	XMFLOAT3			at;				// カメラの注視点
	XMFLOAT3			up;				// カメラの上方向ベクトル

	float				angle;			// 視野角
	float				aspect;			// アスペクト比
	float				len;			// カメラの視点と注視点の距離
	float				nearZ;			// カメラのクリッピング最小値Z
	float				farZ;			// カメラのクリッピング最大値Z


	BOOL layerCulling[(int)GameObject::Layer::LayerMax];

	GameObject*			lookObject;
	Renderer* pRenderer;

	TrackingMode				mode;
	ID3D11Buffer* cameraBuffer;

	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthTarget;
	XMFLOAT4 clearColor;	// 背景色

	ClearMode clearMode;

	GameObject* sky;
	vector<PrimitiveComponent*> skyComArray;

	vector<PostEffectShader*> shaderArray;
	int postEffectIndex;
	BOOL postEffectEnable;
	RenderTexture* renderTexture;
	int renderTextureIndex;

	void SetFrustumPlanes(void);
	void SetFrustumCorners(void);


	XMVECTOR planes[6];
	XMVECTOR points[8];
};