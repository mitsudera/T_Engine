#include "AssetsManager.h"
#include "Assets.h"
#include "MeshData.h"
#include "GameEngine.h"
//#include "SkinMeshDataList.h"
//#include "SkeletonAnimData.h"
#include "DX11Texture.h"
#include "LambartShader.h"
#include "PhongShader.h"
#include "UIShader.h"
#include "Material.h"
#include "AnimationData.h"
#include "RenderTexture.h"
#include "ShadowShader.h"
#include "GausianBlurShader.h"
#include "LambartMaterial.h"
#include "FadeShader.h"
#include "UIMaterial.h"
#include "TerrainShader.h"
#include "NotEffectShader.h"
#include "SkinMeshTreeData.h"
#include "SkinMeshComputeShader.h"
#include "SkinMeshPhongShader.h"
#include "SkinMeshShadowShader.h"
#include "DebugLineShader.h"
#include "StandardMaterial.h"
#include "StandardShader.h"
#include "SceneAssetsData.h"

#define MESH_PATH "data/MODEL/mesh/"
#define SKINMESH_PATH "data/MODEL/skinmesh/"



AssetsManager::AssetsManager()
{
	this->pGameEngine = nullptr;
	this->skinMeshCompute = nullptr;
}

AssetsManager::AssetsManager(GameEngine* pGameEngine)
{
	this->pGameEngine = pGameEngine;
	this->skinMeshCompute = nullptr;

}

AssetsManager::~AssetsManager()
{


}

void AssetsManager::Awake(void)
{
	//pGameEngine->GetRenderer()->CreateCSFile("shaders/CSskinmesh.hlsl", "CSFunc", &skinMeshCompute);
	
	CreateAllShader();
	CreateDefaultMaterial();
}

void AssetsManager::Uninit(void)
{
	for (MeshData* data:MeshDataList)
	{
		delete data;

	}
	this->MeshDataList.clear();

	for (SkinMeshTreeData* data: SkinMeshTreeDataList)
	{
		delete data;

	}
	this->SkinMeshTreeDataList.clear();


	for (AnimationData* data: AnimDataList)
	{
		if (data) delete data;
	}
	AnimDataList.clear();

	for (ShaderSet* data: ShaderSetList)
	{
		data->Uninit();
		if (data) delete data;
	}
	ShaderSetList.clear();

	for (PostEffectShader* data:PostEffectShaderList)
	{
		if (data) delete data;
	}
	PostEffectShaderList.clear();

	for (ComputeShader* data:ComputeShaderList)
	{
		data->Destroy();
		if (data) delete data;
	}
	ComputeShaderList.clear();

	for (Material* data:MaterialArray)
	{
		if (data) delete data;
	}
	MaterialArray.clear();


	for (RenderTexture* data : RenderTextureList)
	{
		if (data) delete data;
	}
	RenderTextureList.clear();



	for (DX11Texture* data : TextureList)
	{
		delete data;

	}
	this->TextureList.clear();


}

list<Assets*>& AssetsManager::GetAssetsList(void)
{
	return assetsList;
}


MeshData* AssetsManager::LoadMeshFileFbx(string fileName)
{
	//既にロードしているデータか調べる
	for (MeshData* mesh : MeshDataTree)
	{
		string filePath = mesh->GetPath();
		if ((MESH_PATH + fileName) == filePath)
		{
			return mesh;
		}
	}
	//ロードしてなかったら追加
	MeshData* meshdata = new MeshData;
	string path = MESH_PATH + fileName;
	meshdata->LoadFbxFile(path, this);
	this->MeshDataTree.push_back(meshdata);
	return meshdata;
}

AnimationData* AssetsManager::LoadAnimationData(string fileName)
{

	for (AnimationData* data : AnimDataList)
	{

		string filePath = data->GetFileName();
		if ((fileName) == filePath)
		{
			return data;
		}
	}

	
	AnimationData* animdata = new AnimationData;
	string path = fileName;
	animdata->LoadAnimation(path, this);
	this->AnimDataList.push_back(animdata);
	return animdata;
}

AnimationData* AssetsManager::LoadAnimationData(string fileName1,string fileName2)
{

	for (AnimationData* data : AnimDataList)
	{
		string filePath1 = data->GetFileName();
		string filePath2 = data->GetFileNameSecond();
		if (((fileName1) == filePath1)&&((fileName2) == filePath2))
		{
			return data;
		}
	}

	
	AnimationData* animdata = new AnimationData;
	string path1 = fileName1;
	string path2 = fileName2;
	animdata->LoadAnimation(path1, path2, this);
	this->AnimDataList.push_back(animdata);
	return animdata;
}

AnimationData* AssetsManager::LoadAnimationData(string fileName1, string fileName2, string fileName3, string fileName4)
{

	for (AnimationData* data : AnimDataList)
	{

		string filePath1 = data->GetFileName();
		string filePath2 = data->GetFileNameSecond();
		string filePath3 = data->GetFileName3();
		string filePath4 = data->GetFileName4();
		if (((fileName1) == filePath1) 
			&& ((fileName2) == filePath2)
			&& ((fileName3) == filePath3)
			&& ((fileName4) == filePath4))
		{
			return data;
		}
	}


	AnimationData* animdata = new AnimationData;
	string path1 = fileName1;
	string path2 = fileName2;
	string path3 = fileName3;
	string path4 = fileName4;
	animdata->LoadAnimation(path1, path2, path3, path4, this);
	this->AnimDataList.push_back(animdata);
	return animdata;
}




void AssetsManager::AddMesh(MeshData* data)
{
	MeshDataList.push_back(data);

	return ;
}

SkinMeshTreeData* AssetsManager::LoadSkinMeshFileFbx(string fileName)
{
	//既にロードしているデータか調べる
	for (SkinMeshTreeData* skinMesh : SkinMeshTreeDataList)
	{
		string filePath = skinMesh->GetFileName();
		if ((SKINMESH_PATH + fileName) == filePath)
		{
			return skinMesh;
		}
	}
	//ロードしてなかったら追加
	SkinMeshTreeData* skinMeshdata = new SkinMeshTreeData(this);
	string path = SKINMESH_PATH + fileName;
	skinMeshdata->LoadFbxFile(path);
	this->SkinMeshTreeDataList.push_back(skinMeshdata);
	return skinMeshdata;
}



GameEngine* AssetsManager::GetGameEngine(void)
{
	return this->pGameEngine;
}


DX11Texture* AssetsManager::LoadTexture(string filepath)
{
	for (DX11Texture* tex : TextureList)
	{
		if (filepath == tex->GetPath())
		{
			return tex;
		}
	}


	DX11Texture* tex = new DX11Texture(this);
	tex->CreateSRV(filepath);
	this->TextureList.push_back(tex);
	return tex;
}


void AssetsManager::CreateAllShader(void)
{
	//
	lambartShader = new LambartShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(lambartShader);

	phongShader = new PhongShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(phongShader);

	uiShader = new UIShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(uiShader);

	terrainShader = new TerrainShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(terrainShader);

	skinMeshPhongShader = new SkinMeshPhongShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(skinMeshPhongShader);
	
	standardShader = new StandardShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(standardShader);
	
	//shadow
	shadowShader = new ShadowShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(shadowShader);

	skinMeshShadowShader = new SkinMeshShadowShader(this->pGameEngine->GetRenderer());
	ShaderSetList.push_back(skinMeshShadowShader);

	//posteffect
	gausianBlur = new GausianBlurShader(this->pGameEngine->GetRenderer());
	PostEffectShaderList.push_back(gausianBlur);

	fadeShader = new FadeShader(this->pGameEngine->GetRenderer());
	PostEffectShaderList.push_back(fadeShader);

	notEffectShader = new NotEffectShader(this->pGameEngine->GetRenderer());
	PostEffectShaderList.push_back(notEffectShader);

	//cs
	skinMeshCompute = new SkinMeshComputeShader(this->pGameEngine->GetRenderer());
	ComputeShaderList.push_back(skinMeshCompute);

	
}

void AssetsManager::CreateDefaultMaterial(void)
{
	UIMaterial* uiMat = new UIMaterial(this);
	uiMat->SetName("UIMaterial");
	MaterialArray.push_back(uiMat);
	assetsList.push_back(uiMat);

	StandardMaterial* standard = new StandardMaterial(this);
	standard->SetName("StandardMaterial");
	MaterialArray.push_back(standard);
	assetsList.push_back(standard);
}

LambartShader* AssetsManager::GetLambartShader(void)
{
	return this->lambartShader;
}

PhongShader* AssetsManager::GetPhongShader(void)
{
	return this->phongShader;
}

UIShader* AssetsManager::GetUIShader(void)
{
	return this->uiShader;
}


ShadowShader* AssetsManager::GetShadowShader(void)
{
	return this->shadowShader;
}

SkinMeshShadowShader* AssetsManager::GetSkinMeshShadowShader(void)
{
	return this->skinMeshShadowShader;
}

TerrainShader* AssetsManager::GetTerrainShader(void)
{
	return this->terrainShader;
}

SkinMeshPhongShader* AssetsManager::GetSkinMeshShader(void)
{
	return this->skinMeshPhongShader;
}

StandardShader* AssetsManager::GetStandardShader(void)
{
	return standardShader;
}

GausianBlurShader* AssetsManager::GetGausianBlurShader(void)
{
	return this->gausianBlur;
}

FadeShader* AssetsManager::GetFadeShader(void)
{
	return this->fadeShader;
}

NotEffectShader* AssetsManager::GetNotEffectShader(void)
{
	return this->notEffectShader;
}

void AssetsManager::SetShader(ShaderSet::ShaderIndex index)
{
	switch (index)
	{

	case ShaderSet::Lambart:

		this->lambartShader->SetShaderRenderer();
		break;

	case ShaderSet::Phong:

		this->phongShader->SetShaderRenderer();
		break;

	case ShaderSet::UI:

		this->uiShader->SetShaderRenderer();
		break;
	case ShaderSet::Terrain:
		this->terrainShader->SetShaderRenderer();
		break;
	case ShaderSet::SkinMeshPhong:
		this->skinMeshPhongShader->SetShaderRenderer();
		break;
	case ShaderSet::Standard:
		this->standardShader->SetShaderRenderer();
		break;
	}
}

void AssetsManager::SetShadowShader(ShaderSet::ShadowShaderIndex index)
{
	switch (index)
	{
	case ShaderSet::ShadowShaderIndex::StandardShadow:
		this->shadowShader->SetShaderRenderer();
		break;

	case ShaderSet::ShadowShaderIndex::SkinMeshShadow:
		this->skinMeshShadowShader->SetShaderRenderer();
		break;

	default:
		break;
	}
}



RenderTexture* AssetsManager::CreateRenderTexture(int widht, int height, string name)
{
	RenderTexture* rtex = new RenderTexture(this);
	rtex->CreateRenderTexture(widht, height, name);
	RenderTextureList.push_back(rtex);



	return rtex;
}


ShaderSet* AssetsManager::GetShader(ShaderSet::ShaderIndex index)
{
	for (ShaderSet* shader : ShaderSetList)
	{
		if (shader->GetShaderIndex() == index)
		{
			return shader;
		}
	}

	return nullptr;
}

void AssetsManager::DeleteRenderTexture(RenderTexture* rt)
{
	RenderTextureList.remove(rt);
	delete rt;

}


SkinMeshComputeShader* AssetsManager::GetSkinMeshComputeShader(void)
{
	return skinMeshCompute;
}

list<SceneAssetsData*>& AssetsManager::GetSceneAssetsDataList(void)
{
	return sceneAssetsList;
}

SceneAssetsData* AssetsManager::CreateNewSceneAssets(string name)
{
	SceneAssetsData* newData = new SceneAssetsData(this);

	newData->SetName(name);

	this->sceneAssetsList.push_back(newData);
	this->assetsList.push_back(newData);;


	return newData;
}



Material* AssetsManager::LoadMaterial(Material* material)
{
	if (material->GetName().empty())
	{
		material->SetName("Material");
	}
	for (Material* mat : MaterialArray)
	{
		if (mat->GetName() == material->GetName())
		{

			delete material;
			return mat;
		}
	}



	this->MaterialArray.push_back(material);
	this->assetsList.push_back(material);
	return material;
}

Material* AssetsManager::LoadShadowMaterial(Material* material)
{
	if (material->GetName().empty())
	{
		material->SetName("Material");
	}
	for (Material* mat : ShadowMaterialArray)
	{
		if (mat->GetName() == material->GetName())
		{

			delete material;
			return mat;
		}
	}



	this->ShadowMaterialArray.push_back(material);
	return material;
}

Material* AssetsManager::GetMaterial(string name)
{
	for (Material* mat : MaterialArray)
	{
		if (mat->GetName() == name)
		{
			return mat;
		}
	}
	return nullptr;
}
