#pragma once
#include "Component.h"
#include "gameobject.h"

class Renderer;

class MeshComponent;

class PostEffectShader;

class RenderTexture;

class PrimitiveComponent;

class ProjectSetting;

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
	int GetViewPort(void);

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
	void SetRenderTargetBackBuffer(void);

	void SetClearMode(ClearMode mode);
	void SetClearColor(XMFLOAT4 color);


	void SetMainCamera(void);

	void SetPostEffect(PostEffectShader* shader);
	void SetPostEffectEnable(BOOL enable);

	void SetTrackingMode(TrackingMode mode);
	TrackingMode GetTrackingMode(void);
	ClearMode GetClearMode(void);

	XMFLOAT3 GetAtPos(void);

	void SetNear(float f);
	void SetFar(float f);

	void SetProjectionMtx(void);

	BOOL FrustumCulling(XMVECTOR min, XMVECTOR max);


	list<string*>& GetLayerCulling(void);
	bool GetLayerCulling(string* layer);
	void SetLayerCulling(string* layer, bool b);
	void SetLayerCulling(string layer, bool b);

	float GetAngle(void);
	float GetAspect(void);
	float GetNear(void);
	float GetFar(void);

	void SetAngle(float angle);
	void SetAspect(float aspect);

	GameObject* GetLookObject(void);

	int GetPostEffectIndex(void);
	BOOL GetPostEffectEnable(void);

	int GetRT(void);

	XMFLOAT4 GetClearColor(void);

private:

	int rt;

	RenderTexture* rttex;

	ProjectSetting* pProjectSetting;

	D3D11_VIEWPORT vp;

	int viewPortType;

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


	list<string*> layerCulling;

	GameObject*			lookObject;
	Renderer* pRenderer;

	TrackingMode				mode;
	ID3D11Buffer* cameraBuffer;

	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthTarget;
	XMFLOAT4 clearColor;	// 背景色

	ClearMode clearMode;


	vector<PostEffectShader*> shaderArray;
	int postEffectIndex;
	BOOL postEffectEnable;
	RenderTexture* renderTexture;

	void SetFrustumPlanes(void);
	void SetFrustumCorners(void);


	XMVECTOR planes[6];
	XMVECTOR points[8];
};