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

	for (int i = 0; i < this->MeshDataArray.size(); i++)
	{
		if(this->MeshDataArray[i]) delete this->MeshDataArray[i];

	}
	this->MeshDataArray.clear();

	for (int i = 0; i < this->SkinMeshTreeDataArray.size(); i++)
	{
		if(this->SkinMeshTreeDataArray[i]) delete this->SkinMeshTreeDataArray[i];

	}
	this->SkinMeshTreeDataArray.clear();


	for (AnimationData* data: AnimDataArray)
	{
		if (data) delete data;
	}
	AnimDataArray.clear();

	for (ShaderSet* data: ShaderSetArray)
	{
		data->Uninit();
		if (data) delete data;
	}
	ShaderSetArray.clear();

	for (PostEffectShader* data:PostEffectShaderArray)
	{
		if (data) delete data;
	}
	PostEffectShaderArray.clear();

	for (ComputeShader* data:ComputeShaderArray)
	{
		data->Destroy();
		if (data) delete data;
	}
	ComputeShaderArray.clear();

	for (Material* data:MaterialArray)
	{
		if (data) delete data;
	}
	MaterialArray.clear();


	for (RenderTexture* data : RenderTextureArray)
	{
		if (data) delete data;
	}
	RenderTextureArray.clear();




	for (int i = 0; i < this->TextureArray.size(); i++)
	{
		delete this->TextureArray[i];
	}
	this->TextureArray.clear();


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
		string filePath = mesh->GetFileName();
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

	for (int i = 0; i < AnimDataArray.size(); i++)
	{

		string filePath = AnimDataArray[i]->GetFileName();
		if ((fileName) == filePath)
		{
			return AnimDataArray[i];
		}
	}

	
	AnimationData* animdata = new AnimationData;
	string path = fileName;
	animdata->LoadAnimation(path, this);
	this->AnimDataArray.push_back(animdata);
	return animdata;
}

AnimationData* AssetsManager::LoadAnimationData(string fileName1,string fileName2)
{

	for (int i = 0; i < AnimDataArray.size(); i++)
	{

		string filePath1 = AnimDataArray[i]->GetFileName();
		string filePath2 = AnimDataArray[i]->GetFileNameSecond();
		if (((fileName1) == filePath1)&&((fileName2) == filePath2))
		{
			return AnimDataArray[i];
		}
	}

	
	AnimationData* animdata = new AnimationData;
	string path1 = fileName1;
	string path2 = fileName2;
	animdata->LoadAnimation(path1, path2, this);
	this->AnimDataArray.push_back(animdata);
	return animdata;
}

AnimationData* AssetsManager::LoadAnimationData(string fileName1, string fileName2, string fileName3, string fileName4)
{

	for (int i = 0; i < AnimDataArray.size(); i++)
	{

		string filePath1 = AnimDataArray[i]->GetFileName();
		string filePath2 = AnimDataArray[i]->GetFileNameSecond();
		string filePath3 = AnimDataArray[i]->GetFileName3();
		string filePath4 = AnimDataArray[i]->GetFileName4();
		if (((fileName1) == filePath1) 
			&& ((fileName2) == filePath2)
			&& ((fileName3) == filePath3)
			&& ((fileName4) == filePath4))
		{
			return AnimDataArray[i];
		}
	}


	AnimationData* animdata = new AnimationData;
	string path1 = fileName1;
	string path2 = fileName2;
	string path3 = fileName3;
	string path4 = fileName4;
	animdata->LoadAnimation(path1, path2, path3, path4, this);
	this->AnimDataArray.push_back(animdata);
	return animdata;
}




int AssetsManager::AddMesh(MeshData* data)
{
	MeshDataArray.push_back(data);

	return (int)MeshDataArray.size()-1;
}
MeshData* AssetsManager::GetMeshData(int n)
{
	return this->MeshDataArray[n];
}

SkinMeshTreeData* AssetsManager::LoadSkinMeshFileFbx(string fileName)
{
	//既にロードしているデータか調べる
	for (SkinMeshTreeData* skinMesh : SkinMeshTreeDataArray)
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
	this->SkinMeshTreeDataArray.push_back(skinMeshdata);
	return skinMeshdata;
}



GameEngine* AssetsManager::GetGameEngine(void)
{
	return this->pGameEngine;
}


DX11Texture* AssetsManager::LoadTexture(string filepath)
{
	for (DX11Texture* tex : TextureArray)
	{
		if (filepath == tex->GetPath())
		{
			return tex;
		}
	}


	DX11Texture* tex = new DX11Texture(this);
	tex->CreateSRV(filepath);
	this->TextureArray.push_back(tex);
	return tex;
}


void AssetsManager::CreateAllShader(void)
{
	//
	lambartShader = new LambartShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(lambartShader);

	phongShader = new PhongShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(phongShader);

	uiShader = new UIShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(uiShader);

	terrainShader = new TerrainShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(terrainShader);

	skinMeshPhongShader = new SkinMeshPhongShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(skinMeshPhongShader);
	
	standardShader = new StandardShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(standardShader);
	
	//shadow
	shadowShader = new ShadowShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(shadowShader);

	skinMeshShadowShader = new SkinMeshShadowShader(this->pGameEngine->GetRenderer());
	ShaderSetArray.push_back(skinMeshShadowShader);

	//posteffect
	gausianBlur = new GausianBlurShader(this->pGameEngine->GetRenderer());
	PostEffectShaderArray.push_back(gausianBlur);

	fadeShader = new FadeShader(this->pGameEngine->GetRenderer());
	PostEffectShaderArray.push_back(fadeShader);

	notEffectShader = new NotEffectShader(this->pGameEngine->GetRenderer());
	PostEffectShaderArray.push_back(notEffectShader);

	//cs
	skinMeshCompute = new SkinMeshComputeShader(this->pGameEngine->GetRenderer());
	ComputeShaderArray.push_back(skinMeshCompute);

	
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



int AssetsManager::CreateRenderTexture(int widht, int height, string name)
{
	RenderTexture* rtex = new RenderTexture(this);
	rtex->CreateRenderTexture(widht, height, name);
	RenderTextureArray.push_back(rtex);



	return (int)RenderTextureArray.size() - 1;
}

int AssetsManager::GetRenderTextureIndex(string name)
{

	for (int i = 0; i < RenderTextureArray.size(); i++)
	{
		if (RenderTextureArray[i]->GetName() == name)
		{
			return i;
		}
	}

	return -1;
}

RenderTexture* AssetsManager::GetRenderTexture(int index)
{
	return this->RenderTextureArray[index];
}

ShaderSet* AssetsManager::GetShader(ShaderSet::ShaderIndex index)
{
	return ShaderSetArray[index];
}

void AssetsManager::DeleteRenderTexture(int index)
{
	if(this->RenderTextureArray[index]) delete this->RenderTextureArray[index];
	this->RenderTextureArray[index] = nullptr;
}

SkinMeshComputeShader* AssetsManager::GetSkinMeshComputeShader(void)
{
	return skinMeshCompute;
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
