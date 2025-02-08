#pragma once
#include "CoreMinimal.h"
#include "ShaderSet.h"

class Assets;

class MeshData;

class SkinMeshTreeData;

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
	int AddMesh(MeshData* data);
	MeshData* GetMeshData(int n);

	SkinMeshTreeData* LoadSkinMeshFileFbx(string fileName);


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


	int CreateRenderTexture(int widht, int height, string name);
	int GetRenderTextureIndex(string name);
	RenderTexture* GetRenderTexture(int index);

	ShaderSet* GetShader(ShaderSet::ShaderIndex index);

	void DeleteRenderTexture(int index);

	SkinMeshComputeShader* GetSkinMeshComputeShader(void);

private:

	GameEngine* pGameEngine;

	list<Assets*> assetsList;

	vector<MeshData*> MeshDataTree;//メッシュデータのルートの配列
	vector<MeshData*> MeshDataArray;
	vector<AnimationData*> AnimDataArray;
	vector<RenderTexture*> RenderTextureArray;
	vector<ShaderSet*> ShaderSetArray;
	vector<PostEffectShader*> PostEffectShaderArray;
	vector<ComputeShader*> ComputeShaderArray;
	vector<SkinMeshTreeData*> SkinMeshTreeDataArray;

	//vector<KeyFrameAnimData*>  KeyFrameAnimDataArray;
	//vector<SkinMeshDataList*> SkinMeshDataListArray;
	//vector<SkeletonAnimData*> SkeletonAnimDataArray;
	vector<Material*> MaterialArray;
	vector<Material*> ShadowMaterialArray;

	vector<DX11Texture*> TextureArray;


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


};

