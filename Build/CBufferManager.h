#pragma once
#include "Coreminimal.h"



class Renderer;
class GameEngine;

class CBufferManager
{
public:
	CBufferManager(GameEngine* gameEngine);
	~CBufferManager();

	enum class BufferSlot :unsigned int
	{
		World,
		View,
		Projection,
		Material,
		DirectionalLight,
		PointLight,
		Camera,
		Shadow,
		Bone,
		Free1,
		Free2,
		Free3,
		Free4,
		Free5,
	};

	void SetCBufferOtherCS(ID3D11Buffer* buffer, BufferSlot slot);//コンピュートシェーダー以外の定数バッファ
	void SetCBufferVSPS(ID3D11Buffer* buffer, BufferSlot slot);//頂点シェーダーとピクセルシェーダー
	void SetCBufferCS(ID3D11Buffer* buffer, BufferSlot slot);//コンピュートシェーダーの定数バッファ
	void SetCBufferHSDS(ID3D11Buffer* buffer, BufferSlot slot);//コンピュートシェーダーの定数バッファ

	void SetCBufferVS(ID3D11Buffer* buffer, BufferSlot slot);
	void SetCBufferHS(ID3D11Buffer* buffer, BufferSlot slot);
	void SetCBufferDS(ID3D11Buffer* buffer, BufferSlot slot);
	void SetCBufferPS(ID3D11Buffer* buffer, BufferSlot slot);

	void SetWorldMtx(XMMATRIX* world);
	void SetViewMtx(XMMATRIX* view);
	void SetProjectionMtx(XMMATRIX* projection);
	void SetCameraBuffer(XMFLOAT3* pos);


	//バッファを直接セットする
	void SetWorldBuffer(ID3D11Buffer* world);
	void SetViewBuffer(ID3D11Buffer* view);
	void SetProjectionBuffer(ID3D11Buffer* projection);
	void SetMaterialBuffer(ID3D11Buffer* material);
	void SetDirectionalLightBuffer(ID3D11Buffer* light);
	void SetPointLightBuffer(ID3D11Buffer* light);
	void SetCameraBuffer(ID3D11Buffer* camera);
	void SetShadowBuffer(ID3D11Buffer* shadow);

private:
	GameEngine* pGameEngine;
	Renderer* pRenderer;
	ID3D11DeviceContext* pDeviceContext;

	ID3D11Buffer* WorldBuffer;
	ID3D11Buffer* ViewBuffer;
	ID3D11Buffer* ProjectionBuffer;
	ID3D11Buffer* CameraBuffer;


};

