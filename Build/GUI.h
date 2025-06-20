#pragma once
#include "Coreminimal.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"

class World;
class GameEngine;
class Renderer;
class Scene;
class GameObject;
class Object;
class Component;
class ProjectSetting;
class AssetsManager;
class TerrainMaterial;
class Material;
class Assets;
class EditerCamera;
class SaveSystem;
class SceneManager;
class SceneAssetsData;
class TransformComponent;
class CameraComponent;
class DirectionalLightComponent;
class AnimationControlerComponent;
class ColliderComponent;
class PointLightComponent;
class PrimitiveComponent;
class MeshComponent;
class QuadComponent;
class SpriteComponent;
class TerrainColliderComponent;
class TerrainComponent;
class TextMeshComponent;
class TextWriteComponent;
class RigidBodyComponent;
class SkinMeshLinkerComponent;
class SkinMeshComponent;
class BoneComponent;
class SoundSpeakerComponent;
class PointColliderComponent;
class LineColliderComponent;
class SphereColliderComponent;
class BoxColliderComponent;
class CapsuleColliderComponent;
class RotBoxColliderComponent;



class GUI
{
public:
	static BOOL GuiWndPro(HWND g_hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	GUI(World* world);
	~GUI();
	void Update(void);
	void Draw(void);

private:

	void UpdateToolWindow(void);
	void UpdateHierarchyWindow(void);
	void UpdateInspectorWindow(void);
	void UpdateAssetsWindow(void);
	void UpdateGizmo(void);
	void ShowAllGameObject(Scene* scene);
	void ShowAllGameObject(GameObject* gameObject);
	void ShowObject(Object* object);
	void ShowComponent(Component* com);
	void ShowColliderComponent(ColliderComponent* com);
	void ShowRigidBodyComponent(RigidBodyComponent* com);
	void ShowBoneComponent(BoneComponent* com);
	void ShowTransformCom(TransformComponent* com);
	void ShowCamCom(CameraComponent* com);
	void ShowTerrainComponent(TerrainComponent* com);
	void ShowAnimationControlerComponent(AnimationControlerComponent* com);

	void ShowAssets(Assets* assets);
	void ShowMaterial(Material* mat);
	void ShowSceneAssets(SceneAssetsData* sad);
	void ShowTerrainMaterial(TerrainMaterial* mat);


	World* pWorld;
	GameEngine* pGameEngine;
	Renderer* pRenderer;
	ProjectSetting* pProjectSetting;
	AssetsManager* pAssetsManager;
	EditerCamera* pEditerCamera;
	SaveSystem* pSaveSystem;
	SceneManager* pSceneManager;

	BOOL showGui;
	ImGuiIO guiIO;
	ImVec4 clear_color; 
	Object* selectedObject;  // 選択されたオブジェクトを保存するメンバ変数
	ImGuizmo::OPERATION gizmoOperation;// ギズモのモード

	string loadFileName;
	string loadSceneName;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 view;

	Component* remCom;

	Object* lastSelectSceneOrGameObject;

};

