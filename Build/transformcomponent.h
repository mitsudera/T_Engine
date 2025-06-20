#pragma once
#include "component.h"
#include "Coreminimal.h"

class TransformComponent:public Component
{
public:




	TransformComponent();
	TransformComponent(GameObject* gameObject);
	~TransformComponent();

	virtual void Awake(void) override;

	//virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;
	virtual void LateUpdate(void) override;

	virtual void Draw(void) override;

	void UpdateMatrix(void);//描画処理の直前で行列を更新

	virtual XMFLOAT3 GetPosition(void);


	XMFLOAT3 GetOldPosition(void);
	XMFLOAT3 GetRotation(void);
	XMVECTOR GetRotQton(void);
	XMFLOAT3 GetOldRotation(void);
	XMFLOAT3 GetScale(void);
	//XMFLOAT3 GetForward(void);
	XMVECTOR GetAxisX(void);
	XMVECTOR GetAxisY(void);
	XMVECTOR GetAxisZ(void);
	XMMATRIX GetMtxRot(void);



	XMMATRIX GetWorldMtx(XMMATRIX mtx);
	XMMATRIX GetWorldMtx(void);
	XMMATRIX GetLocalMtx(void);

	void SetPosition(XMFLOAT3 pos);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMVECTOR pos);
	void SetRotation(XMVECTOR qton);
	void SetRotation(XMFLOAT3 rot);
	void SetRotation(float x, float y, float z);
	void SetScale(XMFLOAT3 scl);
	void SetScale(float x, float y, float z);
	void SetPositionMtx(XMMATRIX pos);
	void SetRotationMtx(XMMATRIX rot);
	void SetScaleMtx(XMMATRIX scl);
	//void RotForward(XMFLOAT3 forward);
	void SetWorldMtx(XMMATRIX mtx);
	void SetLocalMtx(XMMATRIX mtx);
	void SetLocalMtxNotScaling(XMMATRIX mtx);
	void SetLocalMtx(XMMATRIX mtx1, float weight1, XMMATRIX mtx2, float weight2);
	void SetLocalMtxNotScaling(XMMATRIX mtx1, float weight1, XMMATRIX mtx2, float weight2);
	void SetTransForm(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl);

	void SetPosX(float f);
	void SetPosY(float f);
	void SetPosZ(float f);

	void SetWorldPosX(float f);
	void SetWorldPosY(float f);
	void SetWorldPosZ(float f);

	void SetWorldPosition(XMFLOAT3 pos);
	void SetWorldPosition(XMVECTOR pos);

	XMFLOAT3 GetWorldPos(void);

	void MoveX(float f);
	void MoveY(float f);
	void MoveZ(float f);

	//このトランスフォームのX軸方向に動かす
	void MoveXAxis(float f);
	//このトランスフォームのY軸方向に動かす
	void MoveYAxis(float f);
	//このトランスフォームのZ軸方向に動かす
	void MoveZAxis(float f);

	void MoveVelocity(XMVECTOR velo);

	//void MoveForward(float f);


	void PosUpdate(void);

	//void SetForwardDiection(XMFLOAT3 dir);

	void RotRoll(float f);
	void RotPitch(float f);
	void RotYaw(float f);

	void RotWorldRoll(float f);

	void RotWorldPitch(float f);

	void RotWorldYaw(float f);

	void Rotate(float f, XMVECTOR axis);

	void RotAxis(XMVECTOR qton);
	void SetRotAxis(XMVECTOR qton);

	void RotAxisAngle(XMVECTOR axis, float angle);
	
	float Length(TransformComponent* transform);

	void SetMtxUpdate(BOOL flag);

	void RotQuaternion(XMVECTOR qton);


	XMMATRIX GetLastLocalMtx(void);

	void Reset(void);

protected:

	

	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		oldPos;
	XMFLOAT3		rot;		// ポリゴンの向き
	XMFLOAT3		oldRot;
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)
	//XMFLOAT3		forward;
	//XMFLOAT3		fDirection;

	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;


	XMMATRIX		mtxpos;
	XMMATRIX		mtxscl;

	XMMATRIX		mtxrot;			//回転行列
	XMVECTOR		quaternion;		//クオータニオン

	XMMATRIX		lMtx;

	BOOL			isMtxUpdate;

	XMMATRIX		initPosMtx;
	XMMATRIX		initRotMtx;
	XMMATRIX		initSclMtx;

	XMMATRIX		lastLocalMtx;

};

