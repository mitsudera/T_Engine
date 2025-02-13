#include "ProjectSetting.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "MeshField.h"

#include "transformcomponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "AnimationControlerComponent.h"

ProjectSetting::ProjectSetting(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;

	{
		defaultGameObjectTypeNameArray.push_back(typeid(GameObject).name());
		defaultGameObjectTypeNameArray.push_back(typeid(Camera).name());
		defaultGameObjectTypeNameArray.push_back(typeid(DirectionalLight).name());
		defaultGameObjectTypeNameArray.push_back(typeid(MeshField).name());
	}

	{
		componentTypeNameArray.push_back(typeid(CameraComponent).name());
		componentTypeNameArray.push_back(typeid(DirectionalLightComponent).name());
		componentTypeNameArray.push_back(typeid(AnimationControlerComponent).name());
	}
}

ProjectSetting::~ProjectSetting()
{
}

vector<string>& ProjectSetting::GetDefaultGameObjectTypeNameArray(void)
{
	return defaultGameObjectTypeNameArray;
}

vector<string>& ProjectSetting::GetComponentNameArray(void)
{
	return componentTypeNameArray;
}

GameObject* ProjectSetting::CreateGameObjectByTypeName(string typeName, Scene* scene)
{

	if (typeName == typeid(GameObject).name())
	{
		return CreateGameObject<GameObject>("GameObject", scene);
	}
	else if (typeName == typeid(Camera).name())
	{
		return CreateGameObject<Camera>("Camera", scene);
	}
	else if (typeName == typeid(DirectionalLight).name())
	{
		return CreateGameObject<DirectionalLight>("DirectionalLight", scene);
	}
	else if (typeName == typeid(MeshField).name())
	{
		return CreateGameObject<MeshField>("MeshField", scene);
	}

	return nullptr;

}

GameObject* ProjectSetting::DynamicCreateGameObjectByTypeName(string typeName, Scene* scene)
{

	if (typeName == typeid(GameObject).name())
	{
		return DynamicCreateGameObject<GameObject>("GameObject", scene);
	}
	else if (typeName == typeid(Camera).name())
	{
		return DynamicCreateGameObject<Camera>("Camera", scene);
	}
	else if (typeName == typeid(DirectionalLight).name())
	{
		return DynamicCreateGameObject<DirectionalLight>("DirectionalLight", scene);
	}
	else if (typeName == typeid(MeshField).name())
	{
		return DynamicCreateGameObject<MeshField>("MeshField", scene);
	}

	return nullptr;
}

Component* ProjectSetting::AddComponentByTypeName(string typeName, GameObject* gameObject)
{

	if (typeName == typeid(CameraComponent).name())
	{
		return gameObject->AddComponent<CameraComponent>();
	}
	else if (typeName == typeid(DirectionalLightComponent).name())
	{
		return gameObject->AddComponent<DirectionalLightComponent>();
	}
	else if (typeName == typeid(AnimationControlerComponent).name())
	{
		return gameObject->AddComponent<AnimationControlerComponent>();
	}

	return nullptr;
}

Component* ProjectSetting::DynamicAddComponentByTypeName(string typeName, GameObject* gameObject)
{
	if (typeName == typeid(CameraComponent).name())
	{
		return gameObject->DynamicAddComponent<CameraComponent>();
	}
	else if (typeName == typeid(DirectionalLightComponent).name())
	{
		return gameObject->DynamicAddComponent<DirectionalLightComponent>();
	}
	else if (typeName == typeid(AnimationControlerComponent).name())
	{
		return gameObject->DynamicAddComponent<AnimationControlerComponent>();
	}

	return nullptr;
}


