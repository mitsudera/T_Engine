#pragma once
#include "Coreminimal.h"
#include "gameobject.h"

class World;
class Renderer;
class GameEngine;
class Input;
class PrimitiveComponent;
class ProjectSetting;

class EditerCamera
{
public:
	EditerCamera(World* world);
	~EditerCamera();

	void Init(void);
	void Update(void);
	void Render(void);

	void FocusObject(GameObject* gameObject);

	XMMATRIX GetProjectionMatrix(void);
	XMMATRIX GetViewMatrix(void);

	XMVECTOR GetAxisZ(void);
		
	XMVECTOR GetPosition(void);
private:
	World* pWorld;
	Renderer* pRenderer;
	GameEngine* pGameEngine;
	ProjectSetting* pProjectSetting;
	Input* input;

	XMVECTOR		pos;

	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;


	XMMATRIX		mtxpos;
	XMVECTOR		quaternion;		//�N�I�[�^�j�I��


	D3D11_VIEWPORT vp;

	XMMATRIX mtxView;
	XMMATRIX mtxProj;
	XMMATRIX viewProj;
	XMFLOAT4 clearColor;

	list<string*>layerCulling;


	float				angle;			// ����p
	float				aspect;			// �A�X�y�N�g��
	float				len;			// �J�����̎��_�ƒ����_�̋���
	float				nearZ;			// �J�����̃N���b�s���O�ŏ��lZ
	float				farZ;			// �J�����̃N���b�s���O�ő�lZ

	vector<PrimitiveComponent*> skyComArray;

	

	void Move(XMVECTOR vec, float value);

	void Rotate(float f, XMVECTOR axis);

	void RotAxis(XMVECTOR qton);

	void SetSkyCom(GameObject* sky);

};

