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
#include "CameraComponent.h"
#include "AnimationControlerComponent.h"
#include "ColliderComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "primitivecomponent.h"
#include "MeshComponent.h"
#include "QuadComponent.h"
#include "SpriteComponent.h"
#include "TerrainColliderComponent.h"
#include "TerrainComponent.h"
#include "TextMeshComponent.h"
#include "TextWriteComponent.h"
#include "RigidBodyComponent.h"
#include "SkinMeshLinkerComponent.h"
#include "SkinMeshComponent.h"
#include "BoneComponent.h"
#include "SpriteComponent.h"
#include "SoundSpeakerComponent.h"
#include "TerrainColliderComponent.h"
#include "PointColliderComponent.h"
#include "LineColliderComponent.h"
#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "RotBoxColliderComponent.h"


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
		componentTypeNameArray.push_back(typeid(ColliderComponent).name());
		componentTypeNameArray.push_back(typeid(PointLightComponent).name());
		componentTypeNameArray.push_back(typeid(PrimitiveComponent).name());
		componentTypeNameArray.push_back(typeid(MeshComponent).name());
		componentTypeNameArray.push_back(typeid(QuadComponent).name());
		componentTypeNameArray.push_back(typeid(SpriteComponent).name());
		componentTypeNameArray.push_back(typeid(TerrainColliderComponent).name());
		componentTypeNameArray.push_back(typeid(TerrainComponent).name());
		componentTypeNameArray.push_back(typeid(TextMeshComponent).name());
		componentTypeNameArray.push_back(typeid(TextWriteComponent).name());
		componentTypeNameArray.push_back(typeid(RigidBodyComponent).name());
		componentTypeNameArray.push_back(typeid(SkinMeshLinkerComponent).name());
		componentTypeNameArray.push_back(typeid(SkinMeshComponent).name());
		componentTypeNameArray.push_back(typeid(BoneComponent).name());
		componentTypeNameArray.push_back(typeid(SoundSpeakerComponent).name());
		componentTypeNameArray.push_back(typeid(PointColliderComponent).name());
		componentTypeNameArray.push_back(typeid(LineColliderComponent).name());
		componentTypeNameArray.push_back(typeid(SphereColliderComponent).name());
		componentTypeNameArray.push_back(typeid(BoxColliderComponent).name());
		componentTypeNameArray.push_back(typeid(CapsuleColliderComponent).name());
		componentTypeNameArray.push_back(typeid(RotBoxColliderComponent).name());
	}
}

ProjectSetting::~ProjectSetting()
{
	for (string* tag: objectTagList)
	{
		delete tag;
	}
	objectTagList.clear();

	for (string* layer : layerList)
	{
		delete layer;
	}
	layerList.clear();
}

void ProjectSetting::LoadDefaultSeting(void)
{
	AddNewTag("Default");
	AddNewTag("Player");
	AddNewTag("Enemy");
	AddNewTag("PlayerAttack");
	AddNewTag("EnemyAttack");
	AddNewTag("Field");

	AddNewLayer("Default");
	AddNewLayer("Sky");
	AddNewLayer("Field");
	AddNewLayer("Water");
	AddNewLayer("UI");
	AddNewLayer("Text");

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
    else if (typeName == typeid(TransformComponent).name())
    {
        return gameObject->AddComponent<TransformComponent>();
    }
    else if (typeName == typeid(ColliderComponent).name())
    {
        return gameObject->AddComponent<ColliderComponent>();
    }
    else if (typeName == typeid(PointLightComponent).name())
    {
        return gameObject->AddComponent<PointLightComponent>();
    }
    else if (typeName == typeid(PrimitiveComponent).name())
    {
        return gameObject->AddComponent<PrimitiveComponent>();
    }
    else if (typeName == typeid(MeshComponent).name())
    {
        return gameObject->AddComponent<MeshComponent>();
    }
    else if (typeName == typeid(QuadComponent).name())
    {
        return gameObject->AddComponent<QuadComponent>();
    }
    else if (typeName == typeid(SpriteComponent).name())
    {
        return gameObject->AddComponent<SpriteComponent>();
    }
    else if (typeName == typeid(TerrainColliderComponent).name())
    {
        return gameObject->AddComponent<TerrainColliderComponent>();
    }
    else if (typeName == typeid(TerrainComponent).name())
    {
        return gameObject->AddComponent<TerrainComponent>();
    }
    else if (typeName == typeid(TextMeshComponent).name())
    {
        return gameObject->AddComponent<TextMeshComponent>();
    }
    else if (typeName == typeid(TextWriteComponent).name())
    {
        return gameObject->AddComponent<TextWriteComponent>();
    }
    else if (typeName == typeid(RigidBodyComponent).name())
    {
        return gameObject->AddComponent<RigidBodyComponent>();
    }
    else if (typeName == typeid(SkinMeshLinkerComponent).name())
    {
        return gameObject->AddComponent<SkinMeshLinkerComponent>();
    }
    else if (typeName == typeid(SkinMeshComponent).name())
    {
        return gameObject->AddComponent<SkinMeshComponent>();
    }
    else if (typeName == typeid(BoneComponent).name())
    {
        return gameObject->AddComponent<BoneComponent>();
    }
    else if (typeName == typeid(SoundSpeakerComponent).name())
    {
        return gameObject->AddComponent<SoundSpeakerComponent>();
    }
    else if (typeName == typeid(PointColliderComponent).name())
    {
        return gameObject->AddComponent<PointColliderComponent>();
    }
    else if (typeName == typeid(LineColliderComponent).name())
    {
        return gameObject->AddComponent<LineColliderComponent>();
    }
    else if (typeName == typeid(SphereColliderComponent).name())
    {
        return gameObject->AddComponent<SphereColliderComponent>();
    }
    else if (typeName == typeid(BoxColliderComponent).name())
    {
        return gameObject->AddComponent<BoxColliderComponent>();
    }
    else if (typeName == typeid(CapsuleColliderComponent).name())
    {
        return gameObject->AddComponent<CapsuleColliderComponent>();
    }
    else if (typeName == typeid(RotBoxColliderComponent).name())
    {
        return gameObject->AddComponent<RotBoxColliderComponent>();
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
    else if (typeName == typeid(TransformComponent).name())
    {
        return gameObject->DynamicAddComponent<TransformComponent>();
    }
    else if (typeName == typeid(ColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<ColliderComponent>();
    }
    else if (typeName == typeid(PointLightComponent).name())
    {
        return gameObject->DynamicAddComponent<PointLightComponent>();
    }
    else if (typeName == typeid(PrimitiveComponent).name())
    {
        return gameObject->DynamicAddComponent<PrimitiveComponent>();
    }
    else if (typeName == typeid(MeshComponent).name())
    {
        return gameObject->DynamicAddComponent<MeshComponent>();
    }
    else if (typeName == typeid(QuadComponent).name())
    {
        return gameObject->DynamicAddComponent<QuadComponent>();
    }
    else if (typeName == typeid(SpriteComponent).name())
    {
        return gameObject->DynamicAddComponent<SpriteComponent>();
    }
    else if (typeName == typeid(TerrainColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<TerrainColliderComponent>();
    }
    else if (typeName == typeid(TerrainComponent).name())
    {
        return gameObject->DynamicAddComponent<TerrainComponent>();
    }
    else if (typeName == typeid(TextMeshComponent).name())
    {
        return gameObject->DynamicAddComponent<TextMeshComponent>();
    }
    else if (typeName == typeid(TextWriteComponent).name())
    {
        return gameObject->DynamicAddComponent<TextWriteComponent>();
    }
    else if (typeName == typeid(RigidBodyComponent).name())
    {
        return gameObject->DynamicAddComponent<RigidBodyComponent>();
    }
    else if (typeName == typeid(SkinMeshLinkerComponent).name())
    {
        return gameObject->DynamicAddComponent<SkinMeshLinkerComponent>();
    }
    else if (typeName == typeid(SkinMeshComponent).name())
    {
        return gameObject->DynamicAddComponent<SkinMeshComponent>();
    }
    else if (typeName == typeid(BoneComponent).name())
    {
        return gameObject->DynamicAddComponent<BoneComponent>();
    }
    else if (typeName == typeid(SoundSpeakerComponent).name())
    {
        return gameObject->DynamicAddComponent<SoundSpeakerComponent>();
    }
    else if (typeName == typeid(PointColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<PointColliderComponent>();
    }
    else if (typeName == typeid(LineColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<LineColliderComponent>();
    }
    else if (typeName == typeid(SphereColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<SphereColliderComponent>();
    }
    else if (typeName == typeid(BoxColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<BoxColliderComponent>();
    }
    else if (typeName == typeid(CapsuleColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<CapsuleColliderComponent>();
    }
    else if (typeName == typeid(RotBoxColliderComponent).name())
    {
        return gameObject->DynamicAddComponent<RotBoxColliderComponent>();
    }

    return nullptr;
}

GameObject* ProjectSetting::DynamicCroneGameObject(GameObject* gameObject)
{
    GameObject* obj = new GameObject(gameObject);


    obj->Awake();

    obj->InitAllComponent();
    return obj;
}

GameObject* ProjectSetting::DynamicCroneGameObject(GameObject* gameObject,GameObject* parent)
{
    GameObject* obj = new GameObject(gameObject,parent);


    obj->Awake();

    obj->InitAllComponent();
    return obj;
}

void ProjectSetting::SetProjectName(string name)
{
	this->projectName = name;
}

string ProjectSetting::GetProjectName(void)
{
	return this->projectName;
}

void ProjectSetting::SetDefaultScene(string name)
{
	defaultScene = name;
}

string ProjectSetting::GetDefaultScene(void)
{
	return defaultScene;
}

string* ProjectSetting::AddNewTag(string tag)
{
	for (string* atag : objectTagList)
	{
		if (*atag == tag)
		{
			return atag;
		}
	}

	string* newTag = new string(tag);

	objectTagList.push_back(newTag);

	return newTag;
}

string* ProjectSetting::AddNewLayer(string layer)
{
	for (string* alayer : layerList)
	{
		if (*alayer == layer)
		{
			return alayer;
		}
	}

	string* newLayer = new string(layer);

	layerList.push_back(newLayer);
	return newLayer;
}

string* ProjectSetting::GetTag(string tag)
{
	for (string* atag : objectTagList)
	{
		if (*atag == tag)
		{
			return atag;
		}
	}

	return nullptr;
}

string* ProjectSetting::GetLayer(string layer)
{
	for (string* alayer : layerList)
	{
		if (*alayer == layer)
		{
			return alayer;
		}
	}

	return nullptr;
}

list<string*>& ProjectSetting::GetTagList(void)
{
	return objectTagList;
}

list<string*>& ProjectSetting::GetLayerList(void)
{
	return layerList;
}


