#pragma once
#include "transformcomponent.h"
#include "Coreminimal.h"
#include "gameobject.h"



struct HitResult
{
	BOOL isHit[(int)GameObject::ObjectTag::ObjectTagMax];
	vector<GameObject*> hitObject;
	vector<pair<GameObject*,XMFLOAT4>> hitRigidObject;
};




class ColliderComponent :public Component
{
public:

	enum class Shape: int
	{

		Point,
		Line,
		Sphere,
		Capsule,
		Box,
		RotBox,
		Terrain,


	};



	ColliderComponent();
	ColliderComponent(GameObject* gameObject);
	~ColliderComponent();


	virtual void Awake(void) override;
	virtual void Init(void)override;
	virtual void Uninit(void) override;

	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;

	virtual void OnEnable(void)override;
	virtual void OnDisable(void)override;

	BOOL GetHitTag(GameObject::ObjectTag tag);

	void SetHitTag(GameObject::ObjectTag tag, BOOL isHit);

	Shape GetShape(void);
	void SetShape(Shape shape);

	GameObject::ObjectTag GetTag(void);


	void SetHitObject(GameObject* gameObject);
	BOOL GetHitObject(GameObject* gameObject);

	GameObject* GetHitTagObject(GameObject::ObjectTag tag);

	vector<GameObject*> GetHitTagObjectAll(GameObject::ObjectTag tag);

	vector<pair<GameObject*, XMFLOAT4>>& GetHitRigidObject(void);

	void OnCollider(void);
	void OffCollider(void);
	void Clear(void);

	void SetPivot(XMFLOAT3 pivot);
	void SetPivot(XMVECTOR pivot);

	XMFLOAT3 GetCenter(void);
	float GetCheckRadius(void);
	void SetCheckRadius(float r);

	void UpdateCenter(void);

	void SetIsRigid(BOOL b);
	BOOL GetIsRigid(void);

	void SetRigidObject(GameObject* obj, XMFLOAT4 depth);

protected:
	HitResult result;
	Shape shape;
	BOOL enable;

	XMFLOAT3 center;//íÜêS
	XMVECTOR pivot;
	float checkRadius;//ëÂÇ‹Ç©Ç»ìñÇΩÇËîªíËÇçsÇ§ÇΩÇﬂÇÃîºåa
	BOOL isRigid;
};

