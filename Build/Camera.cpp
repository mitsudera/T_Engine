#include "Camera.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"
#include"CameraControllerComponent.h"

Camera::Camera(Scene* scene)
{
	this->pScene = scene;
	this->parent = nullptr;
}

Camera::Camera(GameObject* gameObject)
{
	this->parent = gameObject;
	this->pScene = gameObject->GetScene();

}

Camera::~Camera()
{
}

void Camera::Awake(void)
{
	GameObject::Awake();
	SetName("Camera");
	CameraComponent* cameraComponent = AddComponent<CameraComponent>();

	cameraComponent->SetRenderTarget(pScene->GetGameEngine()->GetRenderer()->GetBackBuffer());
	cameraComponent->SetDepthStencilView(pScene->GetGameEngine()->GetRenderer()->GetBackBufferDSV());



}

