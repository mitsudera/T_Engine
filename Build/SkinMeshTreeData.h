#pragma once
#include "Coreminimal.h"
#include "SkinMeshPhongShader.h"
#include "Assets.h"

class AssetsManager;
class SkinMeshTreeData;
class BoneData;
class Material;
class Renderer;

struct CONTROLPOINT
{
	XMFLOAT3 pos;
	float* weight;

};

class SkinMeshTreeNode
{
public:
	enum class Attribute :int
	{
		Root,
		Null,
		Mesh,
		Bone,
	};

	SkinMeshTreeNode();
	~SkinMeshTreeNode();

	virtual void LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree);
	void LoadChild(FbxNode* node);

	string GetName(void);
	vector<SkinMeshTreeNode*>& GetChildArray(void);

	Attribute GetAttribute(void);

	virtual void Destroy(void);

	SkinMeshTreeNode* GetParent(void);

	unsigned int GetIndex(void);
	void SetIndex(unsigned int index);
	SkinMeshTreeData* GetSkinMeshTree(void);


protected:
	unsigned int index;
	AssetsManager* pAssetsManager;
	Renderer* pRenderer;

	SkinMeshTreeData* skinMeshTree;

	vector <SkinMeshTreeNode*> childArray;
	string name;
	SkinMeshTreeNode* parent;
	string fileName;
	XMMATRIX	worldOffset;
	XMMATRIX	localOffset;
	XMFLOAT3	posOffset;
	XMFLOAT3	sclOffset;
	XMFLOAT3	rotOffset;
	Attribute nodeAttribute;
};

class SkinMeshData:public SkinMeshTreeNode
{
public:
	SkinMeshData(AssetsManager* p);
	~SkinMeshData();
	//void LoadSkinMesh(FbxMesh* mesh, SkinMeshData* parent,SkinMeshTreeData* skinMeshTree);
	virtual void LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree) override;


	virtual void Destroy(void)override;


	ID3D11Buffer* GetIndexBuffer(void);

	int GetCotrolPointNum(void);
	CONTROLPOINT* GetControlPoint(void);

	int GetVertNum(void);
	SkinMeshVertex* GetVertexArray(void);
	int* GetCPIndexArray(void);

	void BufferSetIndex(void);

	int GetIndexNum(void);

	Material* GetMaterial(void);
	Material* GetShadowMaterial(void);

	XMFLOAT3 GetPosOffset(void);
	XMFLOAT3 GetSclOffset(void);
	XMFLOAT3 GetRotOffset(void);

private:



	SkinMeshVertex* vertexArray;
	int vertNum;
	int* cpIndexArray;
	void CreateVertexArray(int n);
	
	CONTROLPOINT* cpArray;
	int cpNum;
	void CreateCpArray(int n);

	ID3D11Buffer* indexBuffer;
	int indexNum;
	void CreateIndexBuffer(int n);


	Material* material;
	Material* shadowMaterial;

	int clusterNum;

	int boneNum;

};


class BoneData :public SkinMeshTreeNode
{
public:
	BoneData(AssetsManager* p);
	~BoneData();
	//void LoadBone(FbxSkeleton* bone, BoneData* parent, SkinMeshTreeData* skinMeshTree);
	virtual void LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree) override;

	

	XMMATRIX GetWorldOffset(void);
	XMMATRIX GetLocalOffset(void);

	XMFLOAT3 GetPosOffset(void);
	XMFLOAT3 GetSclOffset(void);
	XMFLOAT3 GetRotOffset(void);

private:

};
class NullData :public SkinMeshTreeNode
{
public:
	NullData(AssetsManager* p);
	~NullData();
	virtual void LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree) override;

	


private:

};

class SkinMeshTreeData:public Assets
{
public:
	SkinMeshTreeData(AssetsManager* p);
	~SkinMeshTreeData();
	void LoadFbxFile(string fileName);

	AssetsManager* GetAssetsMnager(void);

	int GetBoneCnt(void);
	vector<SkinMeshTreeNode*>& GetNodeArray(void);


	XMMATRIX GetInitMtx(string name);
	void SetLinkMtx(pair<XMMATRIX, string> link);

	int GetBoneNumber(string name);

	void AddNode(SkinMeshTreeNode* node);

	SkinMeshTreeNode* GetNode(unsigned int index);

private:
	vector<SkinMeshTreeNode*> nodeArray;
	vector<SkinMeshTreeNode*> allNodeArray;
	int skinCnt;
	int* clusterCnt;
	int boneCnt;
	pair<XMMATRIX, string>* linkMtxArray;

};

