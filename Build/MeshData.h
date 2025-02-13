//=============================================================================
//
// メッシュの処理 [meshdata.h]
// Author :三ツ寺貴紀
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"
#include "Renderer.h"
#include "Assets.h"
//*********************************************************
//クラス定義
//*********************************************************

class DX11Texture;
class DX11_SUBSET;
class MeshData;
class MeshDataList;
class AssetsManager;
class Material;




 class MeshData:public Assets
 {
 public:
	 MeshData();
	 ~MeshData();


	 void CreateVertexBuffer(int n);
	 ID3D11Buffer* GetVertexBuffer(void);

	 void SetIndexNum(unsigned int n);
	 unsigned int GetIndexNum(void);

	 void CreateIndexBuffer(int n);
	 ID3D11Buffer* GetIndexBuffer(void);

	 int GetChildCount(void);

	 vector <MeshData*> &GetChild(void);

	 void BufferSetVertex(void);
	 void BufferSetIndex(void);


	 void LoadFbxFile(string fileName, AssetsManager* p);

	 void LoadFbxMesh(FbxMesh* mesh, AssetsManager* p,MeshData* parent);


	 void SetBoxMinMax(vector<XMFLOAT3> vertices);

	 AssetsManager* GetpAssetsManager(void);

	 Material* GetMaterial(void);
	 Material* GetShadowMaterial(void);

	 BOOL GetIsRoot(void);
	 XMMATRIX GetLocalOffset(void);

	 XMVECTOR GetBoxMin(void);
	 XMVECTOR GetBoxMax(void);

 private:
	 AssetsManager* pAssetsManager;

	 int index;

	 ID3D11Buffer* VertexBuffer;
	 unsigned int indexnum;
	 ID3D11Buffer* IndexBuffer;
	 XMMATRIX	worldOffset;
	 XMMATRIX   localOffset;
	 XMFLOAT3	posOffset;
	 XMFLOAT3	sclOffset;
	 XMFLOAT3	rotOffset;

	 MeshData* parent;
	 int childcnt;
	 vector <MeshData*> childArray;

	 Material* material;
	 Material* shadowMaterial;
	 BOOL isRoot;

	 XMVECTOR boxMin;
	 XMVECTOR boxMax;

 };

