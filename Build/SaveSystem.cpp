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
#include "ProjectSetting.h"
#include "SkinMeshTreeData.h"
#include "AnimationData.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "TerrainMaterial.h"
#include "SkinMeshPhongMaterial.h"
#include "ShadowMappingMaterial.h"
#include "SkinMeshShadowMappingMaterial.h"
#include "StandardMaterial.h"
#include "UIMaterial.h"
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

#define ProjectSavePath "ProjectSave/ProjectSave.json"
#define SceneSavePath "ProjectSave/SceneSave/"
#define AssetsSavePath "ProjectSave/AssetsSave/"

SaveSystem::SaveSystem(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;
	pAssetsManager = pGameEngine->GetAssetsManager();
	pProjectSetting = pGameEngine->GetProjectSetting();
}

SaveSystem::~SaveSystem()
{
}

void SaveSystem::SaveProject(void)
{
	json jsonData;

	jsonData["ProjectName"] = pProjectSetting->GetProjectName();
	jsonData["DefaulScene"] = pProjectSetting->GetDefaultScene();
	for (string* tag : pProjectSetting->GetTagList())
	{
		jsonData["tag"].push_back(*tag);
	}

	for (string* layer : pProjectSetting->GetLayerList())
	{
		jsonData["layer"].push_back(*layer);

	}



	for (Scene* scene : pGameEngine->GetWorld()->GetActiveSceneList())
	{
		SaveScene(scene);
	
		json oscene;

		oscene["name"] = scene->GetName();
		jsonData["openScene"].push_back(oscene);


	}



	for (Assets* assets: pAssetsManager->GetAssetsList())
	{
		SaveAssets(assets);
	}
	 
	// JSONデータをファイルに保存
	ofstream outputFile(ProjectSavePath);
	if (outputFile.is_open()) {
		outputFile << jsonData.dump(4); // 4はインデントのスペース数
		outputFile.close();
	}

}

bool SaveSystem::LoadProject(void)
{
	SceneManager* pSceneManager = pGameEngine->GetWorld()->GetSceneManager();

	// ファイルからJSONデータを読み込む
	ifstream inputFile(ProjectSavePath);
	if (!inputFile.is_open()) return false;
	json loadData;
	inputFile >> loadData;


	pProjectSetting->SetProjectName(loadData["ProjectName"]);
	pProjectSetting->SetDefaultScene(loadData["DefaulScene"]);


	for (json tagData : loadData["tag"])
	{
		pProjectSetting->AddNewTag(tagData);
	}
	for (json layerData : loadData["layer"])
	{
		pProjectSetting->AddNewLayer(layerData);
	}


	LoadAllAssets();

	int loadSceneCnt = 0;
	for (json data : loadData["openScene"])
	{
		if (pSceneManager->LoadScene(data["name"]))
		{
			loadSceneCnt++;
		}
		
	}

	if (loadSceneCnt == 0)
	{
		pSceneManager->CreateNewScene();
	}


	return true;

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
		string layer = *gameObject->GetLayer();
		goData["Layer"] = layer;
		string tag = *gameObject->GetTag();
		goData["Tag"] = tag;

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

			if (CameraComponent* cCom = dynamic_cast<CameraComponent*>(com))
			{

				for (string* layer: cCom->GetLayerCulling())
				{

					string str = *layer;
					comData["layerCull"].push_back(str);

				}




			}
			if (DirectionalLightComponent* lCom = dynamic_cast<DirectionalLightComponent*>(com))
			{
				DIREC_LIGHT_PARAM lParam = lCom->GetLightParam();

				comData["lightDif"] = { lParam.m_Diffuse.x,lParam.m_Diffuse.y,lParam.m_Diffuse.z,lParam.m_Diffuse.w };
				comData["lightAmb"] = { lParam.m_Ambient.x,lParam.m_Ambient.y,lParam.m_Ambient.z,lParam.m_Ambient.w };
				comData["enable"] = lParam.m_Enable;
			}

			if (PointLightComponent* lCom = dynamic_cast<PointLightComponent*>(com))
			{
				POINT_LIGHT_PARAM lParam = lCom->GetLightParam();

				comData["lightDif"] = { lParam.m_Diffuse.x,lParam.m_Diffuse.y,lParam.m_Diffuse.z,lParam.m_Diffuse.w };
				comData["lightAmb"] = { lParam.m_Ambient.x,lParam.m_Ambient.y,lParam.m_Ambient.z,lParam.m_Ambient.w };
				comData["lightAtt"] = lParam.m_Attenuation.x;
				comData["lightInt"] = lParam.m_intensity.x;
				comData["enable"] = lParam.m_Enable;

			}
			if (PrimitiveComponent* pCom = dynamic_cast<PrimitiveComponent*>(com))
			{
				comData["hasShadow"] = pCom->GetHasShadow();
				comData["drawShadow"] = pCom->GetDrawShadow();
				comData["alphaTest"] = pCom->GetAlphaTest();
				comData["cullMode"] = pCom->GetCullingMode();
				comData["isFrustumCulling"] = pCom->GetIsFrustumCulling();

				if (Material* mat = pCom->GetMaterial())
				{
					comData["hasMaterial"] = true;
					comData["matName"] = mat->GetName();

				}
				else
				{
					comData["hasMaterial"] = false;

				}
				if (Material* mat = pCom->GetShadowMaterial())
				{
					comData["hasShadowMaterial"] = true;
					comData["shadowMatName"] = mat->GetName();

				}
				else
				{
					comData["hasShadowMaterial"] = false;

				}

				if (MeshComponent* mCom = dynamic_cast<MeshComponent*>(pCom))
				{
					comData["meshFileName"] = mCom->GetMeshData()->GetPath();
					comData["meshIndex"] = mCom->GetMeshData()->GetIndex();
				}

				if (SkinMeshLinkerComponent* sCom = dynamic_cast<SkinMeshLinkerComponent*>(pCom))
				{
				}
				if (SkinMeshComponent* sCom = dynamic_cast<SkinMeshComponent*>(pCom))
				{
					comData["linkerID"] = sCom->GetLinker()->GetGameObject()->GetID();
					comData["skinMeshFileName"] = sCom->GetSkinMeshData()->GetSkinMeshTree()->GetPath();
					comData["skinMeshIndex"] = sCom->GetSkinMeshData()->GetIndex();
				}

				if (TerrainComponent* tCom = dynamic_cast<TerrainComponent*>(pCom))
				{
					comData["resolution"] = { tCom->GetResolution().x,tCom->GetResolution().y };
					comData["size"] = { tCom->GetHW().x, tCom->GetHW().y };
					comData["hightMapPath"] = tCom->GetHeightMapPath();
				}

			}


			if (ColliderComponent* cCom = dynamic_cast<ColliderComponent*>(com))
			{
				comData["shape"] = cCom->GetShape();
				comData["enable"] = cCom->GetEnable();
				XMFLOAT3 pivot;
				XMStoreFloat3(&pivot, cCom->GetPivot());
				comData["pivot"] = { pivot.x,pivot.y,pivot.z };
				comData["radius"] = cCom->GetCheckRadius();
				comData["isRigid"] = cCom->GetIsRigid();
				if (TerrainColliderComponent* tCom = dynamic_cast<TerrainColliderComponent*>(cCom))
				{

				}

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

	SaveAssets(scene->GetSceneAssetsData());

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
		GameObject* go;
		if (pid == 0)
		{
			go = sceneData->LoadGameObject();
			go->SetName(data["Name"]);
			go->SetID(data["ID"]);
			go->SetLayer(pProjectSetting->GetLayer(data["Layer"]));
			go->SetTag(pProjectSetting->GetTag(data["Tag"]));
		}
		else
		{
			GameObject* parent = sceneData->GetGameObject(pid);
			go = parent->LoadChild();

			go->SetName(data["Name"]);
			go->SetID(data["ID"]);
			go->SetLayer(pProjectSetting->GetLayer(data["Layer"]));
			go->SetTag(pProjectSetting->GetTag(data["Tag"]));

		}

		for (json comData : data["Components"])
		{
			string typeName = comData["typeName"];


			if (typeName == typeid(TransformComponent).name())
			{

				TransformComponent* com = go->GetComponent<TransformComponent>();



				XMFLOAT4X4 lmtx;
				lmtx._11 = comData["lMtx_11"];
				lmtx._12 = comData["lMtx_12"];
				lmtx._13 = comData["lMtx_13"];
				lmtx._14 = comData["lMtx_14"];
				lmtx._21 = comData["lMtx_21"];
				lmtx._22 = comData["lMtx_22"];
				lmtx._23 = comData["lMtx_23"];
				lmtx._24 = comData["lMtx_24"];
				lmtx._31 = comData["lMtx_31"];
				lmtx._32 = comData["lMtx_32"];
				lmtx._33 = comData["lMtx_33"];
				lmtx._34 = comData["lMtx_34"];
				lmtx._41 = comData["lMtx_41"];
				lmtx._42 = comData["lMtx_42"];
				lmtx._43 = comData["lMtx_43"];
				lmtx._44 = comData["lMtx_44"];

				com->SetLocalMtx(XMLoadFloat4x4(&lmtx));
			}


			if (typeName == typeid(CameraComponent).name())
			{
				CameraComponent* com = go->AddComponent<CameraComponent>();

				for (string layer : comData["layerCull"])
				{

					com->SetLayerCulling(layer, true);

				}



			}

			if (typeName == typeid(DirectionalLightComponent).name())
			{
				DirectionalLightComponent* com= go->AddComponent<DirectionalLightComponent>();

				DIREC_LIGHT_PARAM lParam;

				lParam.m_Diffuse = { comData["lightDif"][0],comData["lightDif"][1],comData["lightDif"][2],comData["lightDif"][3] };
				lParam.m_Ambient = { comData["lightAmb"][0],comData["lightAmb"][1],comData["lightAmb"][2],comData["lightAmb"][3] };
				com->SetDiffuse(lParam.m_Diffuse);
				com->SetAmbient(lParam.m_Ambient);
				com->SetEnable(comData["enable"]);
			}
			if (typeName == typeid(PointLightComponent).name())
			{
				PointLightComponent* com = go->AddComponent<PointLightComponent>();
				POINT_LIGHT_PARAM lParam;
				lParam.m_Diffuse = { comData["lightDif"][0],comData["lightDif"][1],comData["lightDif"][2],comData["lightDif"][3] };
				lParam.m_Ambient = { comData["lightAmb"][0],comData["lightAmb"][1],comData["lightAmb"][2],comData["lightAmb"][3] };
				lParam.m_Attenuation.x = comData["lightAtt"];
				lParam.m_intensity.x = comData["lightInt"];
				lParam.m_Enable = true;

				com->SetLightParam(lParam);

			}
			if (typeName == typeid(MeshComponent).name())
			{
				MeshComponent* com = go->AddComponent<MeshComponent>();
				com->SetHasShadow(comData["hasShadow"]);
				com->SetDrawShadow(comData["drawShadow"]);
				com->SetAlphaTest(comData["alphaTest"]);
				com->SetIsFrustumCulling(comData["isFrustumCulling"]);
				com->SetCullingMode(comData["cullMode"]);

				if (comData["hasMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["matName"]);
					com->SetMaterial(mat);
				}

				if (comData["hasShadowMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["shadowMatName"]);
					com->SetShadowMaterial(mat);

				}


				
				com->SetMeshData(comData["meshFileName"], comData["meshIndex"]);


			}
			if (typeName == typeid(SkinMeshLinkerComponent).name())
			{
				SkinMeshLinkerComponent* com = go->AddComponent<SkinMeshLinkerComponent>();
				com->SetHasShadow(comData["hasShadow"]);
				com->SetDrawShadow(comData["drawShadow"]);
				com->SetAlphaTest(comData["alphaTest"]);
				com->SetIsFrustumCulling(comData["isFrustumCulling"]);
				com->SetCullingMode(comData["cullMode"]);

				if (comData["hasMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["matName"]);
					com->SetMaterial(mat);
				}

				if (comData["hasShadowMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["shadowMatName"]);
					com->SetShadowMaterial(mat);

				}

			}
			if (typeName == typeid(BoneComponent).name())
			{
				BoneComponent* com = go->AddComponent<BoneComponent>();

			}
			if (typeName == typeid(SkinMeshComponent).name())
			{
				SkinMeshComponent* com = go->AddComponent<SkinMeshComponent>();
				com->SetHasShadow(comData["hasShadow"]);
				com->SetDrawShadow(comData["drawShadow"]);
				com->SetAlphaTest(comData["alphaTest"]);
				com->SetIsFrustumCulling(comData["isFrustumCulling"]);
				com->SetCullingMode(comData["cullMode"]);

				if (comData["hasMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["matName"]);
					com->SetMaterial(mat);
				}

				if (comData["hasShadowMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["shadowMatName"]);
					com->SetShadowMaterial(mat);

				}


				
				com->SetSkinMeshData(comData["skinMeshFileName"], comData["skinMeshIndex"]);



				com->SetLinker(comData["linkerID"]);

			}
			if (typeName == typeid(TerrainComponent).name())
			{
				TerrainComponent* com = go->AddComponent<TerrainComponent>();
				com->SetHasShadow(comData["hasShadow"]);
				com->SetDrawShadow(comData["drawShadow"]);
				com->SetAlphaTest(comData["alphaTest"]);
				com->SetIsFrustumCulling(comData["isFrustumCulling"]);
				com->SetCullingMode(comData["cullMode"]);

				if (comData["hasMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["matName"]);
					com->SetMaterial(mat);
				}

				if (comData["hasShadowMaterial"])
				{
					Material* mat = pAssetsManager->GetMaterial(comData["shadowMatName"]);
					com->SetShadowMaterial(mat);

				}

				com->LoadHeightMap(comData["hightMapPath"]);
				com->CreateVetexBuffer(XMINT2(comData["resolution"][0], comData["resolution"][1]), XMFLOAT2(comData["size"][0], comData["size"][1]));
			}


			if (typeName == typeid(TerrainColliderComponent).name())
			{
				TerrainColliderComponent* com = go->AddComponent<TerrainColliderComponent>();
				com->SetShape(comData["shape"]);

				com->SetPivot(XMFLOAT3(comData["pivot"][0], comData["pivot"][1], comData["pivot"][2]));
				com->SetCheckRadius(comData["radius"]);
				com->SetIsRigid(comData["isRigid"]);
				int enable = comData["enable"];

				if (enable)
				{
					com->OnCollider();
				}

			}



		}

	}


	return sceneData;
}

void SaveSystem::SaveAssets(Assets* assets)
{
	json assetsData;
	assetsData["type"] = assets->GetAssetsType();
	assetsData["name"] = assets->GetName();
	assetsData["path"] = assets->GetPath();
	assetsData["ID"] = assets->GetID();

	string typePath = "";

	if (assets->GetAssetsType() == Assets::AssetsType::Material)
	{
		Material* mat = static_cast<Material*>(assets);

		string matType = "";
		assetsData["diffuse"] = { mat->diffuse.x,mat->diffuse.y,mat->diffuse.z,mat->diffuse.w };
		assetsData["ambient"] = { mat->ambient.x,mat->ambient.y,mat->ambient.z,mat->ambient.w };
		assetsData["specular"] = { mat->specular.x,mat->specular.y,mat->specular.z,mat->specular.w };
		assetsData["emission"] = { mat->emission.x,mat->emission.y,mat->emission.z,mat->emission.w };
		assetsData["shininess"] = mat->shininess;
		assetsData["nodiftex"] = mat->noDiffuseTex;
		assetsData["nonortex"] = mat->noNormalTex;
		assetsData["nospetex"] = mat->noSpecularTex;
		assetsData["difPath"] = mat->GetDifPath();
		assetsData["norPath"] = mat->GetNorPath();
		assetsData["spePath"] = mat->GetSpePath();

		typePath = "Material/";

		if (StandardMaterial* stmat = dynamic_cast<StandardMaterial*>(mat))
		{
			matType = typeid(StandardMaterial).name();
		}

		if (LambartMaterial* lamat = dynamic_cast<LambartMaterial*>(mat))
		{
			matType = typeid(LambartMaterial).name();
		}

		if (PhongMaterial* phmat = dynamic_cast<PhongMaterial*>(mat))
		{
			matType = typeid(PhongMaterial).name();
		}


		if (UIMaterial* uimat = dynamic_cast<UIMaterial*>(mat))
		{
			matType = typeid(UIMaterial).name();

		}

		if (TerrainMaterial* temat = dynamic_cast<TerrainMaterial*>(mat))
		{
			matType = typeid(TerrainMaterial).name();

			assetsData["edgeFacter"] = temat->GetEdgeFacter();
			assetsData["insideFacter"] = temat->GetInsideFacter();
			assetsData["heightFacter"] = temat->GetHeightFacter();
			assetsData["uvScaleX"] = temat->GetUVscale().x;
			assetsData["uvScaleY"] = temat->GetUVscale().y;
			assetsData["mapUVScaleX"] = temat->GetMapUVscale().x;
			assetsData["mapUVScaleY"] = temat->GetMapUVscale().y;
		}

		if (SkinMeshPhongMaterial* smpMat = dynamic_cast<SkinMeshPhongMaterial*>(mat))
		{
			matType = typeid(SkinMeshPhongMaterial).name();

		}

		if (ShadowMappingMaterial* smMat = dynamic_cast<ShadowMappingMaterial*>(mat))
		{
			matType = typeid(ShadowMappingMaterial).name();

		}

		if (SkinMeshShadowMappingMaterial* smsmMat = dynamic_cast<SkinMeshShadowMappingMaterial*>(mat))
		{
			matType = typeid(SkinMeshShadowMappingMaterial).name();

		}

		assetsData["matType"] = matType;

	}
	if (assets->GetAssetsType() == Assets::AssetsType::Texture)
	{
		DX11Texture* texData = static_cast<DX11Texture*>(assets);

		typePath = "Texture/";


	}

	if (assets->GetAssetsType() == Assets::AssetsType::MeshData)
	{
		MeshData* meshData = static_cast<MeshData*>(assets);

		typePath = "MeshData/";

	}
	if (assets->GetAssetsType() == Assets::AssetsType::SkinMeshData)
	{
		SkinMeshTreeData* skinMeshData = static_cast<SkinMeshTreeData*>(assets);

		typePath = "SkinMeshData/";

	}
	if (assets->GetAssetsType() == Assets::AssetsType::AnimationData)
	{
		AnimationData* animData = static_cast<AnimationData*>(assets);

		int animFileNum= animData->GetFileNum();

		assetsData["fileNum"] = animFileNum;
		if (animFileNum >= 1)
		{
			assetsData["fileName1"] = animData->GetFileName();
		}

		if (animFileNum >= 2)
		{
			assetsData["fileName2"] = animData->GetFileNameSecond();
		}

		if (animFileNum == 4)
		{
			assetsData["fileName3"] = animData->GetFileName3();
			assetsData["fileName4"] = animData->GetFileName4();

		}



		typePath = "AnimationData/";

	}
	if (assets->GetAssetsType() == Assets::AssetsType::Scene)
	{
		SceneAssetsData* sceneData = static_cast<SceneAssetsData*>(assets);

		typePath = "Scene/";
	}

	// JSONデータをファイルに保存
	ofstream outputFile(AssetsSavePath + typePath + assets->GetName() + ".json");
	if (outputFile.is_open()) {
		outputFile << assetsData.dump(4); // 4はインデントのスペース数
		outputFile.close();
		return;
	}

}

void SaveSystem::LoadAssets(string fileName)
{
	// ファイルからJSONデータを読み込む
	ifstream inputFile(fileName);
	if (!inputFile.is_open()) return;
	json loadData;
	inputFile >> loadData;


	Assets::AssetsType assetsType = loadData["type"];
	unsigned long id = loadData["ID"];


	switch (assetsType)
	{
	case Assets::AssetsType::Material:
	{
		string matType = loadData["matType"];
		if (pAssetsManager->GetMaterial(loadData["name"]))
		{
			return;
		}
		if (matType == typeid(StandardMaterial).name())
		{
			StandardMaterial* mat = new StandardMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			pAssetsManager->LoadMaterial(mat);
		}
		if (matType == typeid(LambartMaterial).name())
		{
			LambartMaterial* mat = new LambartMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			pAssetsManager->LoadMaterial(mat);
		}
		if (matType == typeid(PhongMaterial).name())
		{
			PhongMaterial* mat = new PhongMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			pAssetsManager->LoadMaterial(mat);
		}
		if (matType == typeid(UIMaterial).name())
		{
			UIMaterial* mat = new UIMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			pAssetsManager->LoadMaterial(mat);
		}

		if (matType == typeid(TerrainMaterial).name())
		{
			TerrainMaterial* mat = new TerrainMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			mat->SetTessEdgeFacter(loadData["edgeFacter"]);
			mat->SetTessInsideFacter(loadData["insideFacter"]);
			mat->SetHeightFacter(loadData["heightFacter"]);
			mat->SetUVScale(XMFLOAT2(loadData["uvScaleX"], loadData["uvScaleY"]));
			mat->SetHeghtMapUVScale(XMFLOAT2(loadData["mapUVScaleX"], loadData["mapUVScaleY"]));

			pAssetsManager->LoadMaterial(mat);
		}
		if (matType == typeid(SkinMeshPhongMaterial).name())
		{
			SkinMeshPhongMaterial* mat = new SkinMeshPhongMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			

			pAssetsManager->LoadMaterial(mat);
		}
		if (matType == typeid(ShadowMappingMaterial).name())
		{
			ShadowMappingMaterial* mat = new ShadowMappingMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			

			pAssetsManager->LoadMaterial(mat);
		}
		if (matType == typeid(SkinMeshShadowMappingMaterial).name())
		{
			SkinMeshShadowMappingMaterial* mat = new SkinMeshShadowMappingMaterial(pAssetsManager);

			mat->SetName(loadData["name"]);

			mat->diffuse = { loadData["diffuse"][0], loadData["diffuse"][1], loadData["diffuse"][2], loadData["diffuse"][3] };
			mat->ambient = { loadData["ambient"][0], loadData["ambient"][1], loadData["ambient"][2], loadData["ambient"][3] };
			mat->specular = { loadData["specular"][0], loadData["specular"][1], loadData["specular"][2], loadData["specular"][3] };
			mat->emission = { loadData["emission"][0], loadData["emission"][1], loadData["emission"][2], loadData["emission"][3] };
			mat->shininess = loadData["shininess"];
			mat->noDiffuseTex = loadData["nodiftex"];
			mat->noNormalTex = loadData["nonortex"];
			mat->noSpecularTex = loadData["nospetex"];

			if (!mat->noDiffuseTex)
			{
				mat->LoadDiffuseTex(loadData["difPath"]);
			}
			if (!mat->noNormalTex)
			{
				mat->LoadNormalTex(loadData["norPath"]);
			}
			if (!mat->noSpecularTex)
			{
				mat->LoadSpeculerTex(loadData["spePath"]);
			}

			

			pAssetsManager->LoadMaterial(mat);
		}

	}

		break;
	case Assets::AssetsType::Texture:
	{
		DX11Texture* tex = pAssetsManager->LoadTexture(loadData["path"]);
		tex->SetName(loadData["name"]);


	}
		break;
	case Assets::AssetsType::MeshData:
	{


		string fileName= loadData["path"];
		// 最後の '/' の位置を見つける
		size_t pos = fileName.rfind('/');

		// 最後の '/' が見つかった場合
		if (pos != string::npos) {
			// '/' より後の部分を切り出す
			fileName = fileName.substr(pos + 1);
		}

		MeshData* mesh = pAssetsManager->LoadMeshFileFbx(fileName);
		mesh->SetName(loadData["name"]);
	}
		break;
	case Assets::AssetsType::SkinMeshData:
	{
		string fileName = loadData["path"];
		// 最後の '/' の位置を見つける
		size_t pos = fileName.rfind('/');

		// 最後の '/' が見つかった場合
		if (pos != string::npos) {
			// '/' より後の部分を切り出す
			fileName = fileName.substr(pos + 1);
		}

		SkinMeshTreeData* skinMesh = pAssetsManager->LoadSkinMeshFileFbx(fileName);
		skinMesh->SetName(loadData["name"]);
	}
		break;
	case Assets::AssetsType::AnimationData:
	{
		int fnum = loadData["fileNum"];

		AnimationData* animData = new AnimationData;

		if (fnum == 1)
		{
			animData->LoadAnimation(loadData["fileName1"], pAssetsManager);
		}
		if (fnum == 2)
		{
			animData->LoadAnimation(loadData["fileName1"],loadData["fileName2"], pAssetsManager);

		}
		if (fnum == 4)
		{
			animData->LoadAnimation(loadData["fileName1"],loadData["fileName2"],loadData["fileName3"],loadData["fileName4"], pAssetsManager);

		}

	}
		break;
	case Assets::AssetsType::Scene:
	{
		SceneAssetsData* sceneAssets = new SceneAssetsData(pAssetsManager);

		sceneAssets->SetPath(loadData["path"]);
		sceneAssets->SetName(loadData["name"]);

		pAssetsManager->LoadSceneAssets(sceneAssets);

	}
		break;
	default:
		break;
	}

}

void SaveSystem::LoadAllAssets(void)
{
	for (const auto& entry : std::filesystem::recursive_directory_iterator(AssetsSavePath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json")
		{
			LoadAssets(entry.path().string());
		}
	}
}

TransformComponentLoadData::TransformComponentLoadData(TransformComponent* com)
{
	lMtx = com->GetLocalMtx();
}
