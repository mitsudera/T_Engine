#pragma once
#include "Coreminimal.h"
class GameEngine;
class TransformComponent;
class Input;
class GameObject;
class Scene;
class World;
class Component
{
public:


	enum class Attribute
	{
		Component,
		Transform,
		Collider,
		Primitive,
		Light,
		Manager,
		Camera,
	};

	Component();
	Component(GameObject* gameObject);
	~Component();

	virtual void Awake(void);//オブジェクトの生成時
	virtual void Init(void);//シーンが始まった時と動的に生成された時他オブジェクトやコンポーネント等を参照する場合はここで入れる
	virtual void Uninit(void);
	virtual void FixedUpdate(void);
	virtual void FixedLateUpdate(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Draw(void);
	virtual void DebugDraw(void);
	virtual void OnEnable(void);
	virtual void OnDisable(void);


	GameObject* GetGameObject(void);

	Component::Attribute GetAttribute(void);

	void SetActive(BOOL b);
	BOOL GetActive(void);

	template<class T>
	T* GetComponent(void);

	XMMATRIX GetWorldMtx(XMMATRIX world);
	XMMATRIX GetWorldMtx(void);
	XMFLOAT3 GetWorldPos(void);

	TransformComponent* GetTransFormComponent(void);

	string GetTypeName(void);

protected:
	GameObject* pGameObject;
	Attribute attribute;
	GameEngine* pGameEngine;
	World* pWorld;
	Scene* pScene;
	Input* input;
	string TypeName;

private:
	BOOL	isActive;

};

template<class T>
T* Component::GetComponent(void)
{
	return pGameObject->GetComponent<T>();
}
