#include "SaveSystem.h"
#include "World.h"
#include "SceneManager.h"
#include "Scene.h"
#include "gameobject.h"
#include "transformcomponent.h"
#include "SceneAssetsData.h"
#include "AssetsManager.h"
#include "Assets.h"
#include "Material.h"
#include "DX11Texture.h"
#include "MeshData.h"
#include "SkinMeshTreeData.h"
#include "AnimationData.h"

#define ProjectSavePath "ProjectSave/"
#define SceneSavePath "ProjectSave/SceneSave/"

SaveSystem::SaveSystem(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;
	pAssetsManager = pGameEngine->GetAssetsManager();
}

SaveSystem::~SaveSystem()
{
}

void SaveSystem::SaveProject(void)
{
	json jsonData;

	for (Scene* scene : pGameEngine->GetWorld()->GetActiveSceneList())
	{
		SaveScene(scene);
	}

	for (Assets* assets: pAssetsManager->GetAssetsList())
	{
		json assetsData;
		assetsData["type"] = assets->GetAssetsType();
		assetsData["name"] = assets->GetName();
		assetsData["path"] = assets->GetPath();
		assetsData["ID"] = assets->GetID();

		if (assets->GetAssetsType() == Assets::AssetsType::Material)
		{
			Material* mat = static_cast<Material*>(assets);

			assetsData["diffuse"] = { mat->diffuse.x,mat->diffuse.y,mat->diffuse.z,mat->diffuse.w };
			assetsData["ambient"] = { mat->ambient.x,mat->ambient.y,mat->ambient.z,mat->ambient.w };
			assetsData["specular"] = { mat->specular.x,mat->specular.y,mat->specular.z,mat->specular.w };
			assetsData["emission"] = { mat->emission.x,mat->emission.y,mat->emission.z,mat->emission.w };
			assetsData["shininess"] = mat->shininess;
			assetsData["nodiftex"] = mat->noDiffuseTex;
			assetsData["nonortex"] = mat->noNormalTex;
			assetsData["nospetex"] = mat->noSpecularTex;


		}
		if (assets->GetAssetsType() == Assets::AssetsType::MeshData)
		{
			MeshData* meshData = static_cast<MeshData*>(assets);


		}
		if (assets->GetAssetsType() == Assets::AssetsType::Scene)
		{
			SceneAssetsData* sceneData = static_cast<SceneAssetsData*>(assets);


		}
		if (assets->GetAssetsType() == Assets::AssetsType::Scene)
		{
			SceneAssetsData* sceneData = static_cast<SceneAssetsData*>(assets);


		}
		if (assets->GetAssetsType() == Assets::AssetsType::Scene)
		{
			SceneAssetsData* sceneData = static_cast<SceneAssetsData*>(assets);


		}
	}

}

void SaveSystem::SaveScene(Scene* scene)
{
	json jsonData;

	jsonData["Name"] = scene->GetName();
	jsonData["idCnt"] = scene->GetIDCnt();
	for (unsigned long nid : scene->GetNotUseIDList())
	{
		jsonData["nIDlist"].push_back((json)nid);


	}


	for (GameObject* gameObject : scene->GetAllGameObject())
	{
		json goData;
		goData["Name"] = gameObject->GetName();
		goData["ID"] = gameObject->GetID();


		unsigned long pID = 0;
		if (gameObject->GetParent())
		{
			pID = gameObject->GetParent()->GetID();

		}

		goData["PID"] = pID;


		for (Component* com : gameObject->GetComponentList())
		{
			json comData;

			comData["typeName"] = com->GetTypeName();

			if (TransformComponent* tCom = dynamic_cast<TransformComponent*>(com))
			{
				XMFLOAT4X4 lmtx;
				XMStoreFloat4x4(&lmtx, tCom->GetLocalMtx());
				comData["lMtx_11"] = lmtx._11;
				comData["lMtx_12"] = lmtx._12;
				comData["lMtx_13"] = lmtx._13;
				comData["lMtx_14"] = lmtx._14;
				comData["lMtx_21"] = lmtx._21;
				comData["lMtx_22"] = lmtx._22;
				comData["lMtx_23"] = lmtx._23;
				comData["lMtx_24"] = lmtx._24;
				comData["lMtx_31"] = lmtx._31;
				comData["lMtx_32"] = lmtx._32;
				comData["lMtx_33"] = lmtx._33;
				comData["lMtx_34"] = lmtx._34;
				comData["lMtx_41"] = lmtx._41;
				comData["lMtx_42"] = lmtx._42;
				comData["lMtx_43"] = lmtx._43;
				comData["lMtx_44"] = lmtx._44;
			}



			goData["Components"].push_back(comData);

		}

		jsonData["goData"].push_back(goData);

	}

	// JSONデータをファイルに保存
	ofstream outputFile(SceneSavePath + scene->GetName() + ".json");
	if (outputFile.is_open()) {
		outputFile << jsonData.dump(4); // 4はインデントのスペース数
		outputFile.close();
	}

}

Scene* SaveSystem::LoadScene(SceneAssetsData* aData)
{
	// JSONデータから復元
	World* world = pGameEngine->GetWorld();

	Scene* sceneData = new Scene(world);
	sceneData->Awake();
	// ファイルからJSONデータを読み込む
	ifstream inputFile(SceneSavePath + aData->GetName() + ".json");
	if (!inputFile.is_open()) return nullptr;
	json loadData;
	inputFile >> loadData;

	sceneData->SetSceneAssetsData(aData);

	sceneData->SetName(loadData["Name"]);
	sceneData->SetIDCnt(loadData["idCnt"]);

	for (json data:loadData["nIDlist"])
	{
		sceneData->SetNotUseID(data);
	}

	// 各GameObject読み込みループ
	for (json data : loadData["goData"])
	{
		unsigned long pid = data["PID"];

		if (pid == 0)
		{
			GameObject* go = sceneData->CreateGameObject();
			go->SetName(data["Name"]);
			go->SetID(data["ID"]);


		}
		else
		{
			GameObject* parent = sceneData->GetGameObject(pid);
			GameObject* go = parent->CreateChild();

			go->SetName(data["Name"]);
			go->SetID(data["ID"]);

		}

		for (json comData : data["Components"])
		{
			
		}

	}


	return sceneData;
}

TransformComponentLoadData::TransformComponentLoadData(TransformComponent* com)
{
	lMtx = com->GetLocalMtx();
}
