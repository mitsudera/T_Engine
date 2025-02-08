#pragma once
#include "Coreminimal.h"
#include "gameobject.h"

class World;
class Renderer;
class GameEngine;
class Input;
class PrimitiveComponent;

class EditerCamera
{
public:
	EditerCamera(World* world);
	~EditerCamera();

	void Update(void);
	void Render(void);

	void FocusObject(GameObject* gameObject);

	XMMATRIX GetProjectionMatrix(void);
	XMMATRIX GetViewMatrix(void);

private:
	World* pWorld;
	Renderer* pRenderer;
	GameEngine* pGameEngine;
	Input* input;

	XMVECTOR		pos;

	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;


	XMMATRIX		mtxpos;
	XMVECTOR		quaternion;		//クオータニオン


	D3D11_VIEWPORT vp;

	XMMATRIX mtxView;
	XMMATRIX mtxProj;
	XMMATRIX viewProj;
	XMFLOAT4 clearColor;

	BOOL layerCulling[(int)GameObject::Layer::LayerMax];


	float				angle;			// 視野角
	float				aspect;			// アスペクト比
	float				len;			// カメラの視点と注視点の距離
	float				nearZ;			// カメラのクリッピング最小値Z
	float				farZ;			// カメラのクリッピング最大値Z

	vector<PrimitiveComponent*> skyComArray;

	

	void Move(XMVECTOR vec, float value);

	void Rotate(float f, XMVECTOR axis);

	void RotAxis(XMVECTOR qton);

	void SetSkyCom(GameObject* sky);


};

