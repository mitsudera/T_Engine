#pragma once
#include "transformcomponent.h"
#include "Coreminimal.h"
#include "gameobject.h"



struct HitResult
{
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

	BOOL GetHitTag(string* tag);
	GameObject* GetHitTagObject(string* tag);


	Shape GetShape(void);
	void SetShape(Shape shape);



	void SetHitObject(GameObject* gameObject);
	BOOL GetHitObject(GameObject* gameObject);

	vector<pair<GameObject*, XMFLOAT4>>& GetHitRigidObject(void);

	void OnCollider(void);
	void OffCollider(void);
	void Clear(void);

	void SetPivot(XMFLOAT3 pivot);
	void SetPivot(XMVECTOR pivot);
	XMVECTOR GetPivot(void);

	XMFLOAT3 GetCenter(void);
	float GetCheckRadius(void);
	void SetCheckRadius(float r);

	void UpdateCenter(void);

	void SetIsRigid(BOOL b);
	BOOL GetIsRigid(void);

	void SetRigidObject(GameObject* obj, XMFLOAT4 depth);

	BOOL GetEnable(void);

protected:
	HitResult result;
	Shape shape;
	BOOL enable;

	XMFLOAT3 center;//íÜêS
	XMVECTOR pivot;
	float checkRadius;//ëÂÇ‹Ç©Ç»ìñÇΩÇËîªíËÇçsÇ§ÇΩÇﬂÇÃîºåa
	BOOL isRigid;
};

