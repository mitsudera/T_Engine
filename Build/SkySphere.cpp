#include "SkySphere.h"
#include "transformcomponent.h"
#include "MeshComponent.h"
#include "renderer.h"
#include "Scene.h"
#include "GameEngine.h"


SkySphere::SkySphere(Scene* scene)
{
	pScene = scene;

}

SkySphere::~SkySphere()
{
}

void SkySphere::Awake(void)
{
	GameObject::Awake();

	SetName("SkySphere");
	this->transformComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	this->transformComponent->SetScale(XMFLOAT3(1000.0f, 1000.0f, 1000.0f));


	this->LoadFbxFileMesh("SkySphere.fbx");
	
	this->SerchAllChild("pSphere1")->GetComponent<PrimitiveComponent>()->SetDrawShadow(FALSE);
	this->SerchAllChild("pSphere1")->GetComponent<PrimitiveComponent>()->SetHasShadow(FALSE);

	SetLayer(Layer::Sky);

}
