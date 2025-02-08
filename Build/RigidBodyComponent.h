#pragma once
#include "component.h"

class ColliderComponent;

class RigidBodyComponent :public Component
{
public:
	RigidBodyComponent(GameObject* gameObject);
	~RigidBodyComponent();

	virtual void Awake(void) override;
	virtual void Init(void) override;
	virtual void FixedUpdate(void) override;
	virtual void FixedLateUpdate(void) override;
	virtual void LateUpdate(void) override;
	virtual void Uninit(void) override;
	virtual void OnEnable(void)override;
	virtual void OnDisable(void)override;


	void SetMass(float f);
	void SetDrag(float f);
	void SetFriction(float f);
	void SetAngularDrag(float f);
	void SetUseGarvity(BOOL b);
	void SetIsKinematic(BOOL b);
	void AddForce(XMVECTOR force);
	void RotVelocityY(float f);
	void MovePosition(XMVECTOR vec, float moveValue);
	void MovePosition(XMFLOAT3 vec, float moveValue);

	float GetOffGroundTime(void);
	float GetGroundLength(void);

	BOOL GetOnGround(void);

	void SetIsStatic(BOOL b);

	void SetIsFixTerrain(BOOL b);

private:
	ColliderComponent* collider;
	TransformComponent* transform;
	XMVECTOR velocity;
	XMVECTOR accel;
	XMVECTOR worldPos;
	XMVECTOR move;
	float mass;
	float drag;
	float angularDrag;
	float friction;
	float offGroundTime;
	BOOL useGravity;
	BOOL isKinematic;
	BOOL onGround;
	BOOL isStatic;
	BOOL isFixTerrain;
	float groundLen;
};

