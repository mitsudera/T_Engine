#pragma once
#include "CoreMinimal.h"
#include "ShaderSet.h"

class Assets;
class SceneAssetsData;

class MeshData;

class SkinMeshTreeData;
class SkinMeshTreeNode;

class DX11Texture;

class GameEngine;

class Material;

class AnimationData;

class RenderTexture;
//shader
class LambartShader;
class PhongShader;
class UIShader;
class TerrainShader;
class SkinMeshPhongShader;
class StandardShader;
//shadow
class ShadowShader;
class SkinMeshShadowShader;


//posteffect
class PostEffectShader;
class GausianBlurShader;
class FadeShader;
class NotEffectShader;

//cs
class ComputeShader;
class SkinMeshComputeShader;




//debug



class AssetsManager
{


public:
	AssetsManager();
	AssetsManager(GameEngine* pGameEngine);
	~AssetsManager();

	void Awake(void);
	void Uninit(void);

	list<Assets*>& GetAssetsList(void);

	MeshData* LoadMeshFileFbx(string fileName);
	void AddMesh(MeshData* data);

	MeshData* GetMesh(string fileName, int index);

	SkinMeshTreeData* LoadSkinMeshFileFbx(string fileName);

	SkinMeshTreeNode* GetSkinMeshTreeNode(string fileName, int index);

	AnimationData* LoadAnimationData(string fileName);

	AnimationData* LoadAnimationData(string fileName1, string fileName2);

	AnimationData* LoadAnimationData(string fileName1, string fileName2, string fileName3, string fileName4);


	

	GameEngine* GetGameEngine(void);

	Material* LoadMaterial(Material* material);
	Material* LoadShadowMaterial(Material* material);


	Material* GetMaterial(string name);



	//DX11Texture* GetTexture(int n);
	DX11Texture* LoadTexture(string filepath);


	void CreateAllShader(void);
	void CreateDefaultMaterial(void);

	LambartShader* GetLambartShader(void);
	PhongShader* GetPhongShader(void);
	UIShader* GetUIShader(void);
	TerrainShader* GetTerrainShader(void);
	SkinMeshPhongShader* GetSkinMeshShader(void);
	StandardShader* GetStandardShader(void);

	ShadowShader* GetShadowShader(void);
	SkinMeshShadowShader* GetSkinMeshShadowShader(void);


	GausianBlurShader* GetGausianBlurShader(void);
	FadeShader* GetFadeShader(void);
	NotEffectShader* GetNotEffectShader(void);

	void SetShader(ShaderSet::ShaderIndex index);
	void SetShadowShader(ShaderSet::ShadowShaderIndex index);


	RenderTexture* CreateRenderTexture(int widht, int height, string name);

	ShaderSet* GetShader(ShaderSet::ShaderIndex index);

	void DeleteRenderTexture(RenderTexture* rt);

	SkinMeshComputeShader* GetSkinMeshComputeShader(void);


	list<SceneAssetsData*>& GetSceneAssetsDataList(void);

	SceneAssetsData* CreateNewSceneAssets(string name);

	SceneAssetsData* LoadSceneAssets(SceneAssetsData* loadData);

	void AddAssets(Assets* assets);
	void AddLoadAssets(Assets* assets);

private:

	GameEngine* pGameEngine;
	list<string> sceneNameList;

	list<Assets*> assetsList;
	list<SceneAssetsData*> sceneAssetsList;

	list<MeshData*> MeshDataTree;//メッシュデータのルートの配列
	list<MeshData*> MeshDataList;
	list<AnimationData*> AnimDataList;
	list<RenderTexture*> RenderTextureList;
	list<ShaderSet*> ShaderSetList;
	list<PostEffectShader*> PostEffectShaderList;
	list<ComputeShader*> ComputeShaderList;
	list<SkinMeshTreeData*> SkinMeshTreeDataList;
	list<Material*> MaterialArray;
	list<Material*> ShadowMaterialArray;
	list<DX11Texture*> TextureList;


	LambartShader* lambartShader;
	StandardShader* standardShader;
	PhongShader* phongShader;
	UIShader* uiShader;
	TerrainShader* terrainShader;
	SkinMeshPhongShader* skinMeshPhongShader;

	ShadowShader* shadowShader;
	SkinMeshShadowShader* skinMeshShadowShader;


	GausianBlurShader* gausianBlur;
	FadeShader* fadeShader;
	NotEffectShader* notEffectShader;
	//cs
	SkinMeshComputeShader* skinMeshCompute;

	unsigned long idCnt;
	list<unsigned long> notUseIDList;

};

