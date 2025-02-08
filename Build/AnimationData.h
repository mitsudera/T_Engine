#pragma once
#include "Coreminimal.h"

class AssetsManager;
class AnimationData;


class MtxNode
{
public:
	enum class Attribute :int
	{
		Root,
		Null,
		Mesh,
		Bone,
	};

	MtxNode();
	~MtxNode();

	void LoadAnimation(FbxNode* node, MtxNode* parent, AnimationData* animdata);

	

	virtual void CreateFrameMtxArray(int n);

	XMMATRIX GetFrameMtx(float time);
	XMMATRIX GetFrameMtxLerp(float f);
	virtual XMMATRIX GetFrameMtx(float time, float weight);

	int GetChildCnt(void);
	MtxNode* GetChild(int n);

	Attribute GetAttribute(void);


	string GetName(void);
protected:

	XMMATRIX* frameMtxArray;
	vector<MtxNode*> childArray;
	MtxNode* parent;

	Attribute attribute;

	string name;
	BOOL isRoot;

	AnimationData* pAnimData;

};

class BlendMtxNode:public MtxNode
{
public:
	BlendMtxNode();
	~BlendMtxNode();

	void LoadAnimation(MtxNode* node1, MtxNode* node2, MtxNode* parent, AnimationData* animdata);



	virtual XMMATRIX GetFrameMtx(float time,float weight) override;

private:
	MtxNode* mtx1;
	MtxNode* mtx2;


};
class AngleBlendMtxNode:public MtxNode
{
public:
	AngleBlendMtxNode();
	~AngleBlendMtxNode();

	void LoadAnimation(MtxNode* node1, MtxNode* node2, MtxNode* node3, MtxNode* node4, MtxNode* parent, AnimationData* animdata);


	//angle=0.0~XM_2PI
	virtual XMMATRIX GetFrameMtx(float time,float angle) override;

private:
	MtxNode* mtxFront;
	MtxNode* mtxRight;
	MtxNode* mtxBack;
	MtxNode* mtxLeft;


};

class AnimationData
{
public:

	enum class Attribute:int
	{
		Standard,
		Blend,
	};

	AnimationData();
	~AnimationData();

	int GetFrameNum(void);
	int GetFrameRate(void);

	LONGLONG GetStartTime(void);
	LONGLONG GetOneFrameValue(void);

	string GetFileName(void);
	string GetFileNameSecond(void);
	string GetFileName3(void);
	string GetFileName4(void);
	MtxNode* GetMtxTreeRoot(void);

	void LoadAnimation(string fileName, AssetsManager* assetsManager);
	void LoadAnimation(string fileName1, string fileName2, AssetsManager* assetsManager);
	void LoadAnimation(string fileName1, string fileName2,string fileName3, string fileName4, AssetsManager* assetsManager);


private:
	int frameNum;
	int frameRate;
	LONGLONG startTime;
	LONGLONG oneFrameValue;


	string fileName;
	string fileNameSecond;
	string fileName3;
	string fileName4;
	
	AssetsManager* pAssetsManager;
	MtxNode* mtxTreeRoot;

};

//class BlendAnimationData:public AnimationData
//{
//public:
//	BlendAnimationData();
//	~BlendAnimationData();
//
//	string GetFileName1(void);
//	string GetFileName2(void);
//
//
//
//private:
//
//	AnimationData* animData1;
//	AnimationData* animData2;
//
//	string fileName1;
//	string fileName2;
//	
//
//};
//
