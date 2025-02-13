#include "DebugCamera.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"

DebugCamera::DebugCamera(Scene* scene)
{
	this->pScene = scene;
	this->parent = nullptr;


}

DebugCamera::DebugCamera(GameObject* gameObject)
{
	this->parent = gameObject;
	this->pScene = gameObject->GetScene();
}


DebugCamera::~DebugCamera()
{
}

void DebugCamera::Awake(void)
{
	GameObject::Awake();
	SetName("DebugCamera");

	CameraComponent* cameraComponent= AddComponent<CameraComponent>();
	cameraComponent->SetRenderTarget(pGameEngine->GetRenderer()->GetBackBuffer());
	cameraComponent->SetDepthStencilView(pGameEngine->GetRenderer()->GetBackBufferDSV());


}

