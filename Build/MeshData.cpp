//=============================================================================
//
// モデルの処理 [meshdata.cpp]
// Author : 三ツ寺貴紀
//
//=============================================================================
#include "meshdata.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "ShaderSet.h"
#include "ShadowMappingMaterial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PATH	"data/TEXTURE/"




MeshData::MeshData()
{
	this->VertexBuffer = nullptr;
	this->indexnum = 0;
	this->IndexBuffer = nullptr;
	this->worldOffset = XMMatrixIdentity();
	this->childcnt = 0;
	this->pAssetsManager = nullptr;
}

MeshData::~MeshData()
{
	if(VertexBuffer) VertexBuffer->Release();
	if(IndexBuffer) IndexBuffer->Release();


}





void MeshData::CreateVertexBuffer(int VertNum)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * VertNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->VertexBuffer);



}

ID3D11Buffer* MeshData::GetVertexBuffer(void)
{
	return VertexBuffer;
}


void MeshData::SetIndexNum(unsigned int n)
{
	this->indexnum = n;
}
unsigned int MeshData::GetIndexNum(void)
{
	return this->indexnum;
}


void MeshData::CreateIndexBuffer(int IndexNum)
{
	// インデックスバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * IndexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->IndexBuffer);


}
ID3D11Buffer* MeshData::GetIndexBuffer(void)
{
	return IndexBuffer;
}

int MeshData::GetChildCount(void)
{
	return (int)this->childArray.size();
}

vector<MeshData*>& MeshData::GetChild(void)
{
	return this->childArray;
}


void MeshData::BufferSetVertex(void)
{

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT worldOffset = 0;

	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->VertexBuffer, &stride, &worldOffset);
}

void MeshData::BufferSetIndex(void)
{

	// プリミティブトポロジ設定
	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// インデックスバッファ設定
	this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetIndexBuffer(this->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

}


AssetsManager* MeshData::GetpAssetsManager(void)
{
	return this->pAssetsManager;
}

Material* MeshData::GetMaterial(void)
{
	return this->material;
}

Material* MeshData::GetShadowMaterial(void)
{
	return shadowMaterial;
}


BOOL MeshData::GetIsRoot(void)
{
	return this->isRoot;
}

XMMATRIX MeshData::GetLocalOffset(void)
{
	return localOffset;
}

XMVECTOR MeshData::GetBoxMin(void)
{
	return boxMin;
}

XMVECTOR MeshData::GetBoxMax(void)
{
	return boxMax;
}








void MeshData::LoadFbxFile(string fileName, AssetsManager* p)
{
	this->pAssetsManager = p;
	this->fileName = fileName;
	this->name = fileName;
	FbxManager* manager;
	FbxIOSettings* ioSettings;
	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	// Importerを生成
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(fileName.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// インポートエラー
	}

	//SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	FbxNode* root = scene->GetRootNode();
	this->isRoot = TRUE;
	this->index = pAssetsManager->AddMesh(this);

	for (int i = 0; i < root->GetChildCount(); i++)
	{
		FbxNode* child = root->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			MeshData* childData = new MeshData();
			childData->LoadFbxMesh(child->GetMesh(), p, this);
			childArray.push_back(childData);
			
		}



	}





	scene->Destroy();
	importer->Destroy(); // シーンを流し込んだらImporterは解放
	manager->Destroy();//fbxファイルの読み込みが終わったらマネージャーは解放する

}


void MeshData::LoadFbxMesh(FbxMesh* mesh,AssetsManager* ap,MeshData* parent)
{
	using namespace fbxsdk;
	this->pAssetsManager = ap;
	FbxNode* node = mesh->GetNode();
	name = mesh->GetNode()->GetName();

	this->parent = parent;
	this->index = pAssetsManager->AddMesh(this);
	this->isRoot = FALSE;
	this->fileName = parent->GetFileName();

	int PolygonNum = mesh->GetPolygonCount();               //総ポリゴン数
	if (PolygonNum==0)
	{

		return;
	}
	
	int PolygonVertexNum = mesh->GetPolygonVertexCount();   //ポリゴン頂点インデックス数
	int controlNum = mesh->GetControlPointsCount();         // 頂点座標数
	FbxVector4* src = mesh->GetControlPoints();             // 頂点座標配列
	VERTEX_3D* VertexArray = nullptr;
	VertexArray = new VERTEX_3D[PolygonVertexNum];
	ZeroMemory(VertexArray, sizeof(VERTEX_3D) * PolygonVertexNum);
	unsigned int indexnum = PolygonNum * 3;
	this->SetIndexNum(indexnum);
	
	//頂点配列を埋める
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);
	FbxVector2* uv = new FbxVector2[PolygonVertexNum];

	vector<XMFLOAT3> vPosArray;
	for (int i = 0; i < controlNum; i++)
	{
		XMFLOAT3 vPos;
		vPos.x = (float)src[i][0];
		vPos.y = (float)src[i][1];
		vPos.z = (float)src[i][2];

		vPosArray.push_back(vPos);

	}

	SetBoxMinMax(vPosArray);

	bool bIsUnmapped = false;

	unsigned int* IndexArry = nullptr;
	IndexArry = new unsigned int[indexnum];

	ZeroMemory(IndexArry, sizeof(unsigned int) * indexnum);
	int ic = 0;


	int vcnt = 0;

	for (int p = 0; p < PolygonNum; p++)
	{



		int IndexNumInPolygon = mesh->GetPolygonSize(p);  // p番目のポリゴンの頂点数



		if (IndexNumInPolygon==3)
		{
			FbxVector4 positions[3];
			FbxVector4 normals[3];
			FbxVector2 uvs[3];

			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// ポリゴン番号
					n,				// 何番目の頂点
					nor);			// 法線の
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = (float)nor[2];

				normals[n] = nor;

				// UV値取得。
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];

				uvs[n] = uv[vcnt];


				// ポリゴンpを構成するn番目の頂点のインデックス番号
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = (float)src[IndexNumber][2];

				positions[n] = src[IndexNumber];

				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				IndexArry[ic] = vcnt;
				ic++;
				vcnt++;

			}


			// タンジェントベクトルの計算
			FbxVector4 edge1 = positions[1] - positions[0];
			FbxVector4 edge2 = positions[2] - positions[0];
			FbxVector2 deltaUV1 = uvs[1] - uvs[0];
			FbxVector2 deltaUV2 = uvs[2] - uvs[0];

			float f = 1.0f / (float)(deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

			FbxVector4 tangent;
			tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
			tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
			tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
			tangent[3] = 0.0f;

			// 各頂点にタンジェントベクトルを設定
			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent.x = (float)tangent[0];
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent.y = (float)tangent[1];
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent.z = (float)tangent[2];
				VertexArray[vcnt - IndexNumInPolygon + n].Tangent = XMFLOAT3Normalize(VertexArray[vcnt - IndexNumInPolygon + n].Tangent);
				


				XMVECTOR nv = XMLoadFloat3(&VertexArray[vcnt - IndexNumInPolygon + n].Normal);
				XMVECTOR tv = XMLoadFloat3(&VertexArray[vcnt - IndexNumInPolygon + n].Tangent);

				XMVECTOR binv = XMVector3Cross(nv, tv);
				binv = XMVector3Normalize(binv);

				XMFLOAT3 binor;

				XMStoreFloat3(&binor, binv);

				VertexArray[vcnt - IndexNumInPolygon + n].BiNormal=binor;
			
			}

		}
		else if (IndexNumInPolygon==4)
		{
			for (int n = 0; n < 3; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = (float)nor[2];



				// UV値取得。
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];


				// ポリゴンpを構成するn番目の頂点のインデックス番号
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = (float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };


				IndexArry[ic] = vcnt;
				ic++;

				vcnt++;

			}
			for (int n = 1; n < 4; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// int pPolyIndex
					n,				// int pVertexIndex
					nor);        // FbxVector4& pNormal
				VertexArray[vcnt].Normal.x = (float)nor[0];
				VertexArray[vcnt].Normal.y = (float)nor[1];
				VertexArray[vcnt].Normal.z = -(float)nor[2];


				


				// UV値取得。
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				VertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				VertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];






				// ポリゴンpを構成するn番目の頂点のインデックス番号
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				VertexArray[vcnt].Position.x = (float)src[IndexNumber][0];
				VertexArray[vcnt].Position.y = (float)src[IndexNumber][1];
				VertexArray[vcnt].Position.z = (float)src[IndexNumber][2];


				VertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				IndexArry[ic] = vcnt;
				ic++;


				vcnt++;

			}

		}

	}

	// 頂点バッファ生成
	this->CreateVertexBuffer(ic);

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, VertexArray, sizeof(VERTEX_3D) * PolygonVertexNum);

		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->GetVertexBuffer(), 0);
	}


	// インデックスバッファ生成
	this->CreateIndexBuffer(indexnum);

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->GetIndexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned int* pIdx = (unsigned int*)msr.pData;

		//memcpy(pIdx, fbxIndexAry, sizeof(unsigned int) * indexnum);

		int n;
		for (unsigned int i = 0; i < indexnum; i++)
		{
			pIdx[i] = IndexArry[i];
			n = i;
		}

		this->GetpAssetsManager()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->GetIndexBuffer(), 0);
	}



	FbxMatrix worldOffset = node->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);//オフセット行列の取得
	this->worldOffset = FbxMatrixConvertToXMMATRIX(worldOffset);

	FbxMatrix localOffset = node->EvaluateLocalTransform(FBXSDK_TIME_INFINITE);
	this->localOffset = FbxMatrixConvertToXMMATRIX(localOffset);


	// マテリアルの数

	int mtlcnt = node->GetMaterialCount();


	// マテリアル情報を取得
	for (int i = 0; i < mtlcnt; i++)
	{
		FbxSurfaceMaterial* fbxmaterial = node->GetMaterial(i);
		
		FbxClassId id = fbxmaterial->GetClassId();

		if (fbxmaterial != 0)
		{
			// マテリアル解析
			// LambertかPhongか
			//lambart

			

			if (id == (FbxSurfaceLambert::ClassId))
			{

				LambartMaterial* lambart = new LambartMaterial(this->GetpAssetsManager());

				lambart->LoadFbxMaterial(fbxmaterial);

				lambart->SetName(this->fileName + fbxmaterial->GetName());

				this->material = pAssetsManager->LoadMaterial(lambart);


			}
			//phong
			else if (id == (FbxSurfacePhong::ClassId))
			{

				PhongMaterial* phong = new PhongMaterial(this->GetpAssetsManager());

				phong->LoadFbxMaterial(fbxmaterial);

				phong->SetName(this->fileName + fbxmaterial->GetName());


				this->material = pAssetsManager->LoadMaterial(phong);


			}
			else
			{

			}
			ShadowMappingMaterial* shadowMat = new ShadowMappingMaterial(this->GetpAssetsManager());

			shadowMat->SetShadowMaterial(this->material);

			

			this->shadowMaterial = pAssetsManager->LoadShadowMaterial(shadowMat);



		}
	}
	delete[] uv;
	delete[] IndexArry;
	delete[] VertexArray;


	for (int i = 0; i < node->GetChildCount(); i++)
	{
		FbxNode* child = node->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			MeshData* childData = new MeshData();
			childData->LoadFbxMesh(child->GetMesh(), ap, this);
			childArray.push_back(childData);

		}



	}



}

string MeshData::GetName(void)
{
	return this->name;
}

string MeshData::GetFileName(void)
{
	return this->fileName;
}

void MeshData::SetBoxMinMax(vector<XMFLOAT3> vertices)
{
	if (vertices.empty())
	{
		boxMin = XMVectorZero();
		boxMax = XMVectorZero();

		return;
	}

	// 初期化
	XMFLOAT3 minVertex = vertices[0];
	XMFLOAT3 maxVertex = vertices[0];

	// 最小値と最大値を見つける
	for (const auto& vertex : vertices)
	{
		if (vertex.x < minVertex.x) minVertex.x = vertex.x;
		if (vertex.y < minVertex.y) minVertex.y = vertex.y;
		if (vertex.z < minVertex.z) minVertex.z = vertex.z;

		if (vertex.x > maxVertex.x) maxVertex.x = vertex.x;
		if (vertex.y > maxVertex.y) maxVertex.y = vertex.y;
		if (vertex.z > maxVertex.z) maxVertex.z = vertex.z;
	}

	// XMVectorに変換
	boxMin = XMLoadFloat3(&minVertex);
	boxMax = XMLoadFloat3(&maxVertex);
}


