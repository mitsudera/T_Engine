#include "transformcomponent.h"
#include "math.h"
#include"gameobject.h"
#include "Scene.h"

TransformComponent::TransformComponent()
{
	this->attribute = Attribute::Transform;
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	//this->forward = { 0.0f,0.0f,-1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	//this->mtxrot = XMMatrixIdentity();
	this->quaternion = XMQuaternionIdentity();
	this->lMtx = XMMatrixIdentity();
	//this->fDirection = { 0.0f,0.0f,1.0f };
	isMtxUpdate = TRUE;
}

TransformComponent::TransformComponent(GameObject* gameObject)
{
	attribute = Attribute::Transform;
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	//this->forward = { 0.0f,0.0f,1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	//this->mtxrot = XMMatrixIdentity();
	this->quaternion = XMQuaternionIdentity();

	this->lMtx = XMMatrixIdentity();
	//this->fDirection = { 0.0f,0.0f,1.0f };

	this->pGameObject = gameObject;
	isMtxUpdate = TRUE;

}

TransformComponent::~TransformComponent()
{


}
void TransformComponent::Awake(void)
{
	Component::Awake();
	pGameObject->GetScene()->AddSceneTransformComponent(this);
	this->TypeName = "Transform";
	attribute = Attribute::Transform;


	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	//this->forward = { 0.0f,0.0f,1.0f };
	//this->fDirection= { 0.0f,0.0f,1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	//this->mtxrot = XMMatrixIdentity();
	this->quaternion = XMQuaternionIdentity();

	this->lMtx = XMMatrixIdentity();
	isMtxUpdate = TRUE;
	TypeName = typeid(TransformComponent).name();;

}

//void TransformComponent::Init(void)
//{
//	this->mtxpos = initPosMtx;
//	this->mtxrot = initRotMtx;
//	this->mtxscl = initRotMtx;
//}

void TransformComponent::Uninit(void)
{
	Component::Uninit();
	pGameObject->GetScene()->RemoveSceneTransformComponent(this);

}

void TransformComponent::Update(void)
{
	Component::Update();


	
	

	this->oldPos = pos;
	this->oldRot = rot;


}

void TransformComponent::LateUpdate(void)
{
	Component::LateUpdate();

	lastLocalMtx = lMtx;
}



void TransformComponent::Draw(void)
{
	Component::Draw();

}

void TransformComponent::UpdateMatrix(void)
{
	//if (!this->isMtxUpdate)
	//	return;
	lMtx = XMMatrixIdentity();
	lMtx = XMMatrixMultiply(lMtx, mtxscl);
	mtxrot = XMMatrixRotationQuaternion(this->quaternion);
	lMtx = XMMatrixMultiply(lMtx, mtxrot);
	lMtx = XMMatrixMultiply(lMtx, mtxpos);

}

XMFLOAT3 TransformComponent::GetPosition(void)
{
	return this->pos;
}

XMFLOAT3 TransformComponent::GetOldPosition(void)
{
	return this->oldPos;
}

XMFLOAT3 TransformComponent::GetRotation(void)
{
	return this->rot;
}

XMVECTOR TransformComponent::GetRotQton(void)
{
	return this->quaternion;
}

XMFLOAT3 TransformComponent::GetOldRotation(void)
{
	return this->oldRot;
}

XMFLOAT3 TransformComponent::GetScale(void)
{
	return this->scl;
}


XMVECTOR TransformComponent::GetAxisX(void)
{
	return this->axisX;
}

XMVECTOR TransformComponent::GetAxisY(void)
{
	return this->axisY;
}

XMVECTOR TransformComponent::GetAxisZ(void)
{
	return this->axisZ;
}


XMMATRIX TransformComponent::GetMtxRot(void)
{
	return this->mtxrot;
}

XMMATRIX TransformComponent::GetWorldMtx(XMMATRIX mtx)
{
	XMMATRIX ans = XMMatrixMultiply(mtx,lMtx);


	if (pGameObject->GetParent() != nullptr)
	{
		ans = pGameObject->GetParent()->GetTransFormComponent()->GetWorldMtx(ans);

	}
	return ans;
}

XMMATRIX TransformComponent::GetWorldMtx(void)
{
	return GetWorldMtx(XMMatrixIdentity());
}

XMMATRIX TransformComponent::GetLocalMtx(void)
{
	return this->lMtx;
}


void TransformComponent::SetPosition(XMFLOAT3 pos)
{
	this->pos = pos;
	this->mtxpos = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->mtxpos = XMMatrixTranslation(pos.x, pos.y, pos.z);

}
void TransformComponent::SetPosition(XMVECTOR pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);

}

void TransformComponent::SetRotation(XMVECTOR qton)
{
	this->quaternion = qton;
	SetRotAxis(quaternion);

}

void TransformComponent::SetRotation(XMFLOAT3 rot)
{
	this->rot = rot;
	this->quaternion = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	SetRotAxis(quaternion);

}

void TransformComponent::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->quaternion = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	SetRotAxis(quaternion);
}

void TransformComponent::SetScale(XMFLOAT3 scl)
{
	this->scl = scl;
	this->mtxscl = XMMatrixScaling(scl.x, scl.y, scl.z);
}
void TransformComponent::SetScale(float x,float y,float z)
{
	this->scl = XMFLOAT3(x, y, z);
	this->mtxscl = XMMatrixScaling(scl.x, scl.y, scl.z);
}

void TransformComponent::SetPositionMtx(XMMATRIX pos)
{
	this->mtxpos = pos;
}

void TransformComponent::SetRotationMtx(XMMATRIX rot)
{
	this->quaternion = XMQuaternionRotationMatrix(rot);
}

void TransformComponent::SetScaleMtx(XMMATRIX scl)
{
	this->mtxscl = scl;
}


//void TransformComponent::RotForward(XMFLOAT3 forward)
//{
//	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&forward));
//	XMVECTOR d = XMVector3Normalize(XMLoadFloat3(&this->forward));
//
//	float dot;
//	XMStoreFloat(&dot, XMVector3Dot(f, d));
//
//	if (dot == -1.0f)
//	{
//
//		XMVECTOR qton = XMQuaternionRotationAxis(axisX, XM_PI);
//		this->RotAxis(qton);
//		mtxrot = XMMatrixRotationQuaternion(qton);
//		return;
//	}
//	if (XMVector3Equal(f, d))
//	{
//		return;
//	}
//
//	XMVECTOR c = XMVector3Cross(f, d);
//
//	XMVECTOR a = XMVector3AngleBetweenNormals(f, d);
//	float angle;
//	XMStoreFloat(&angle, a);
//
//
//
//	XMVECTOR q = XMQuaternionRotationAxis(c, angle);
//	XMMATRIX mtx = XMMatrixRotationQuaternion(q);
//
//	this->RotAxis(q);
//
//	this->mtxrot = mtx;
//
//}




void TransformComponent::SetWorldMtx(XMMATRIX mtx)
{
	if (pGameObject->GetParent() != nullptr)
	{
		XMMATRIX invmtx = XMMatrixInverse(nullptr, pGameObject->GetParent()->GetTransFormComponent()->GetWorldMtx());
		mtx = XMMatrixMultiply(mtx, invmtx);
		SetLocalMtx(mtx);

	}
	else
	{
		SetLocalMtx(mtx);
	}

}

void TransformComponent::SetLocalMtx(XMMATRIX mtx)
{
	XMVECTOR scl;
	XMVECTOR rot;
	XMVECTOR pos;

	bool b= XMMatrixDecompose(&scl, &rot, &pos, mtx);

	if (!b)
	{
		return;
	}

	XMStoreFloat3(&this->scl, scl);
	XMStoreFloat3(&this->pos, pos);


	this->mtxscl = XMMatrixScaling(this->scl.x, this->scl.y, this->scl.z);
	this->SetRotation(rot);
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);


	
}
void TransformComponent::SetLocalMtxNotScaling(XMMATRIX mtx)
{
	XMVECTOR scl;
	XMVECTOR rot;
	XMVECTOR pos;

	bool b= XMMatrixDecompose(&scl, &rot, &pos, mtx);

	if (!b)
	{
		return;
	}

	//XMStoreFloat3(&this->scl, scl);
	XMStoreFloat3(&this->pos, pos);


	//this->mtxscl = XMMatrixScaling(this->scl.x, this->scl.y, this->scl.z);
	this->SetRotation(rot);

	this->quaternion = rot;
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);


	
}

void TransformComponent::SetLocalMtx(XMMATRIX mtx1,float weight1,XMMATRIX mtx2,float weight2)
{
	XMVECTOR scl1;
	XMVECTOR rot1;
	XMVECTOR pos1;

	XMVECTOR scl2;
	XMVECTOR rot2;
	XMVECTOR pos2;

	XMMatrixDecompose(&scl1, &rot1, &pos1, mtx1);
	XMMatrixDecompose(&scl2, &rot2, &pos2, mtx2);

	XMStoreFloat3(&this->scl, (scl1 * weight1) + (scl2 * weight2));
	XMStoreFloat3(&this->pos, (pos1 * weight1) + (pos2 * weight2));


	XMMATRIX rotMtx = (XMMatrixRotationQuaternion(rot1) * weight1) + (XMMatrixRotationQuaternion(rot2) * weight2);




	this->mtxscl = XMMatrixScaling(this->scl.x, this->scl.y, this->scl.z);
	this->SetRotation(XMQuaternionRotationMatrix(rotMtx));

	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);


	
}
void TransformComponent::SetLocalMtxNotScaling(XMMATRIX mtx1,float weight1,XMMATRIX mtx2,float weight2)
{
	XMVECTOR scl1;
	XMVECTOR rot1;
	XMVECTOR pos1;

	XMVECTOR scl2;
	XMVECTOR rot2;
	XMVECTOR pos2;

	XMMatrixDecompose(&scl1, &rot1, &pos1, mtx1);
	XMMatrixDecompose(&scl2, &rot2, &pos2, mtx2);

	XMStoreFloat3(&this->scl, (scl1 * weight1) + (scl2 * weight2));
	XMStoreFloat3(&this->pos, (pos1 * weight1) + (pos2 * weight2));


	XMMATRIX rotMtx = (XMMatrixRotationQuaternion(rot1) * weight1) + (XMMatrixRotationQuaternion(rot2) * weight2);




	this->mtxscl = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	this->SetRotation(XMQuaternionRotationMatrix(rotMtx));
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);


	
}

void TransformComponent::SetTransForm(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	this->SetPosition(pos);
	this->SetRotation(rot);
	this->SetScale(scl);
}

void TransformComponent::SetPosX(float f)
{
	this->pos.x = f;
	PosUpdate();
}

void TransformComponent::SetPosY(float f)
{
	this->pos.y = f;
	PosUpdate();


}

void TransformComponent::SetPosZ(float f)
{
	this->pos.z = f;
	PosUpdate();


}

void TransformComponent::SetWorldPosX(float f)
{
	XMFLOAT3 pos = GetWorldPos();
	pos.x = f;
	SetWorldPosition(pos);
}

void TransformComponent::SetWorldPosY(float f)
{
	XMFLOAT3 pos = GetWorldPos();
	pos.y = f;
	SetWorldPosition(pos);

}

void TransformComponent::SetWorldPosZ(float f)
{
	XMFLOAT3 pos = GetWorldPos();
	pos.z = f;
	SetWorldPosition(pos);

}

void TransformComponent::SetWorldPosition(XMFLOAT3 pos)
{
	XMVECTOR posv = XMLoadFloat3(&pos);
	posv = XMVector3Transform(posv, XMMatrixMultiply(XMMatrixInverse(nullptr, GetWorldMtx()), lMtx));
	XMFLOAT3 p;
	XMStoreFloat3(&p, posv);
	SetPosition(p);
}

void TransformComponent::SetWorldPosition(XMVECTOR pos)
{
	XMFLOAT3 p;
	XMStoreFloat3(&p, pos);
	SetWorldPosition(p);

}


XMFLOAT3 TransformComponent::GetWorldPos(void)
{
	UpdateMatrix();
	XMFLOAT3 lPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMVECTOR wPos = XMLoadFloat3(&lPos);
	wPos = XMVector3Transform(wPos,GetWorldMtx());

	XMStoreFloat3(&lPos, wPos);
	XMMATRIX mtx = GetWorldMtx();

	return lPos;


}

void TransformComponent::MoveX(float f)
{
	this->pos.x += f;
	this->PosUpdate();

}

void TransformComponent::MoveY(float f)
{
	this->pos.y += f;
	this->PosUpdate();

}

void TransformComponent::MoveZ(float f)
{
	this->pos.z += f;
	this->PosUpdate();

}

void TransformComponent::MoveXAxis(float f)
{

	XMFLOAT3 a;

	XMStoreFloat3(&a, this->axisX);

	MoveX(a.x*f);
	MoveY(a.y*f);
	MoveZ(a.z*f);

}

void TransformComponent::MoveYAxis(float f)
{
	XMFLOAT3 a;

	XMStoreFloat3(&a, this->axisY);

	MoveX(a.x * f);
	MoveY(a.y * f);
	MoveZ(a.z * f);

}

void TransformComponent::MoveZAxis(float f)
{
	XMFLOAT3 a;

	XMStoreFloat3(&a, this->axisZ);

	MoveX(a.x * f);
	MoveY(a.y * f);
	MoveZ(a.z * f);

}

void TransformComponent::MoveVelocity(XMVECTOR velo)
{
	XMFLOAT3 v;
	XMStoreFloat3(&v, velo);
	MoveX(v.x);
	MoveY(v.y);
	MoveZ(v.z);
}


//void TransformComponent::MoveForward(float f)
//{
//	float x, y, z;
//	x = this->forward.x * f;
//	y = this->forward.y * f;
//	z = this->forward.z * f;
//
//	MoveX(x);
//	MoveY(y);
//	MoveZ(z);
//
//}

void TransformComponent::PosUpdate(void)
{
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	UpdateMatrix();
}

//void TransformComponent::SetForwardDiection(XMFLOAT3 dir)
//{
//	this->fDirection = XMFLOAT3Normalize(dir);
//	this->forward = XMFLOAT3Normalize(dir);
//}




void TransformComponent::RotRoll(float f)
{
	Rotate(f, axisZ);
}

void TransformComponent::RotPitch(float f)
{
	Rotate(f, axisX);
}

void TransformComponent::RotYaw(float f)
{
	
	Rotate(f, axisY);
}

void TransformComponent::RotWorldRoll(float f)
{
	Rotate(f, zonevec());
}

void TransformComponent::RotWorldPitch(float f)
{
	Rotate(f, xonevec());
}

void TransformComponent::RotWorldYaw(float f)
{
	
	Rotate(f, yonevec());
}

void TransformComponent::Rotate(float f, XMVECTOR axis)
{
	XMVECTOR qton = XMQuaternionRotationAxis(axis, f);
	qton = XMQuaternionNormalize(qton); // クォータニオンを正規化
	this->quaternion = XMQuaternionMultiply(this->quaternion, qton);
	RotAxis(qton);
}



void TransformComponent::RotAxis(XMVECTOR qton)
{
	this->axisX = XMVector3Rotate(this->axisX, qton);
	this->axisX = XMVector3Normalize(this->axisX);
	this->axisY = XMVector3Rotate(this->axisY, qton);
	this->axisY = XMVector3Normalize(this->axisY);
	this->axisZ = XMVector3Rotate(this->axisZ, qton);
	this->axisZ = XMVector3Normalize(this->axisZ);

}

void TransformComponent::SetRotAxis(XMVECTOR qton)
{
	this->axisX = XMVector3Rotate(xonevec(), qton);
	this->axisX = XMVector3Normalize(this->axisX);
	this->axisY = XMVector3Rotate(yonevec(), qton);
	this->axisY = XMVector3Normalize(this->axisY);
	this->axisZ = XMVector3Rotate(zonevec(), qton);
	this->axisZ = XMVector3Normalize(this->axisZ);
}




void TransformComponent::RotAxisAngle(XMVECTOR axis, float angle)
{
	XMVECTOR qton = XMQuaternionRotationAxis(axis, angle);
	RotAxis(qton);

	quaternion = XMQuaternionMultiply(quaternion, qton);
}

float TransformComponent::Length(TransformComponent* transform)
{
	return XMFLOAT3Length(this->GetWorldPos(), transform->GetWorldPos());
}

void TransformComponent::SetMtxUpdate(BOOL flag)
{
	this->isMtxUpdate = flag;
}

void TransformComponent::RotQuaternion(XMVECTOR qton)
{
	qton = XMQuaternionNormalize(qton); // クォータニオンを正規化
	quaternion = XMQuaternionMultiply(quaternion, qton);
}

XMMATRIX TransformComponent::GetLastLocalMtx(void)
{
	return lastLocalMtx;
}
