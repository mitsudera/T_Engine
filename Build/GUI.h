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
class TransformComponent;
class Component;
class CameraComponent;
class ProjectSetting;
class AssetsManager;
class TerrainMaterial;
class Material;
class Assets;
class TerrainComponent;
class AnimationControlerComponent;
class EditerCamera;
class SaveSystem;
class SceneManager;
class SceneAssetsData;

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

	

};

