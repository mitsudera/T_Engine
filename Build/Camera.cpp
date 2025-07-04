#include "Camera.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "GameEngine.h"
#include "renderer.h"

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

	cameraComponent->SetRenderTargetBackBuffer();



}

