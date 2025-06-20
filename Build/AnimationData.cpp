#include "AnimationData.h"
#include "AssetsManager.h"

MtxNode::MtxNode()
{
}

MtxNode::~MtxNode()
{
}

#define ANIMATION_PATH "data/Animation/"


void MtxNode::LoadAnimation(FbxNode* node, MtxNode* parent, AnimationData* animdata)
{
	using namespace fbxsdk;
	name = node->GetName();
	pAnimData = animdata;
	this->parent = parent;
	if (parent == nullptr)
	{
		this->isRoot = TRUE;
		this->attribute = Attribute::Root;

		
	}
	else
	{

		if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			this->attribute = Attribute::Mesh;
		}
		else if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			this->attribute = Attribute::Bone;
		}
		else
		{
			this->attribute = Attribute::Null;
		}
		this->isRoot = FALSE;


		CreateFrameMtxArray(pAnimData->GetFrameNum());

		for (int i = 0; i < pAnimData->GetFrameNum(); i++)
		{
			FbxMatrix framemtx = node->EvaluateLocalTransform(pAnimData->GetStartTime() + pAnimData->GetOneFrameValue() * i);

			XMMATRIX frameMtx = FbxMatrixConvertToXMMATRIX(framemtx);

			this->frameMtxArray[i] = frameMtx;
		}
	}


	for (int i = 0; i < node->GetChildCount(); i++)
	{
		FbxNode* child = node->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			MtxNode* childNode = new MtxNode();
			childNode->LoadAnimation(child, this, pAnimData);
			childArray.push_back(childNode);
		}
		else if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			MtxNode* childNode = new MtxNode();
			childNode->LoadAnimation(child, this, pAnimData);
			childArray.push_back(childNode);
		}
		else
		{
			MtxNode* childNode = new MtxNode();
			childNode->LoadAnimation(child, this, pAnimData);
			childArray.push_back(childNode);

		}
	}




}


void MtxNode::CreateFrameMtxArray(int n)
{
	this->frameMtxArray = new XMMATRIX[n];

}

XMMATRIX MtxNode::GetFrameMtx(float time)
{

	float frame = time * 60.0f;
	float frame2weight = frame - (float)(int)frame;
	float frame1weight = 1.0f - frame2weight;;



	int frame1 = int(frame);
	
	if (frame1 >= (float)pAnimData->GetFrameNum()-1)
	{
		frame1 %= pAnimData->GetFrameNum();
	}
	int frame2 = frame1 + 1;


	if (frame2 >= pAnimData->GetFrameNum()-1)
	{
		frame2 = 0;
	}

	XMMATRIX mtx1 = this->frameMtxArray[frame1];
	XMMATRIX mtx2 = this->frameMtxArray[frame2];

	return (mtx1 * frame1weight) + (mtx2 * frame2weight);

}

XMMATRIX MtxNode::GetFrameMtxLerp(float f)
{
	float frame = f * pAnimData->GetFrameNum();
	float frame2weight = frame - (float)(int)frame;
	float frame1weight = 1.0f - frame2weight;;



	int frame1 = int(frame);

	if (frame1 >= (float)pAnimData->GetFrameNum() - 1)
	{
		frame1 %= pAnimData->GetFrameNum();
	}
	int frame2 = frame1 + 1;


	if (frame2 >= pAnimData->GetFrameNum() - 1)
	{
		frame2 = 0;
	}

	XMMATRIX mtx1 = this->frameMtxArray[frame1];
	XMMATRIX mtx2 = this->frameMtxArray[frame2];

	return (mtx1 * frame1weight) + (mtx2 * frame2weight);
}

XMMATRIX MtxNode::GetFrameMtx(float time, float weight)
{
	float frame = time * 60.0f;
	float frame2weight = frame - (float)(int)frame;
	float frame1weight = 1.0f - frame2weight;;



	int frame1 = int(frame);

	if (frame1 >= (float)pAnimData->GetFrameNum() - 1)
	{
		frame1 %= pAnimData->GetFrameNum();
	}
	int frame2 = frame1 + 1;


	if (frame2 >= pAnimData->GetFrameNum() - 1)
	{
		frame2 = 0;
	}

	XMMATRIX mtx1 = this->frameMtxArray[frame1];
	XMMATRIX mtx2 = this->frameMtxArray[frame2];

	return (mtx1 * frame1weight) + (mtx2 * frame2weight);
}

int MtxNode::GetChildCnt(void)
{
	return (int)this->childArray.size();
}

MtxNode* MtxNode::GetChild(int n)
{
	return this->childArray[n];
}

MtxNode::Attribute MtxNode::GetAttribute(void)
{
	return this->attribute;
}

string MtxNode::GetName(void)
{
	return this->name;
}


AnimationData::AnimationData()
{
	this->assetsType = AssetsType::AnimationData;

}

AnimationData::~AnimationData()
{
}

int AnimationData::GetFrameNum(void)
{
	return frameNum;
}

int AnimationData::GetFrameRate(void)
{
	return frameRate;
}

LONGLONG AnimationData::GetStartTime(void)
{
	return startTime;
}

LONGLONG AnimationData::GetOneFrameValue(void)
{
	return oneFrameValue;
}

string AnimationData::GetFileName(void)
{
	return fileName;
}

string AnimationData::GetFileNameSecond(void)
{
	return fileNameSecond;
}

string AnimationData::GetFileName3(void)
{
	return fileName3;
}

string AnimationData::GetFileName4(void)
{
	return fileName4;
}

MtxNode* AnimationData::GetMtxTreeRoot(void)
{
	return mtxTreeRoot;
}

void AnimationData::LoadAnimation(string fileName, AssetsManager* assetsManager)
{
	this->pAssetsManager = assetsManager;
	fileNum = 1;
	this->fileName = fileName;
	
	SetName(fileName);

	string path = ANIMATION_PATH + fileName;

	FbxManager* manager;
	FbxIOSettings* ioSettings;

	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);



	// Importerを生成
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(path.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// インポートエラー
	}


	//SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	

	int meshcount = scene->GetSrcObjectCount<FbxMesh>();

	//アニメーション名一覧を格納する配列の宣言
	FbxArray<FbxString*> animation_names;

	//シーンからアニメーション一覧を取得＆格納
	scene->FillAnimStackNameArray(animation_names);

	int animcount = animation_names.GetCount();
	int framenum = 0;
	FbxTime start_time = 0;
	FbxTime end_time = 0;
	FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	FbxTime totalframe = 0;

	if (animcount > 0)
	{
		//アニメ番号0番を元にFbxTakeInfoを取得
		auto take_info = scene->GetTakeInfo(animation_names[0]->Buffer());
		//開始時間
		start_time = take_info->mLocalTimeSpan.GetStart();
		//終了時間
		end_time = take_info->mLocalTimeSpan.GetStop();

		oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

		totalframe = (end_time - start_time) / oneFrameValue;

		framenum = (int)totalframe.Get();//アニメーションの合計フレーム数



	}

	this->startTime = start_time.Get();
	this->oneFrameValue = oneFrameValue;

	this->frameNum = framenum;

	this->mtxTreeRoot = new MtxNode();

	this->mtxTreeRoot->LoadAnimation(scene->GetRootNode(), nullptr, this);

	scene->Destroy();
	importer->Destroy();
	ioSettings->Destroy();
	manager->Destroy();
}

void AnimationData::LoadAnimation(string fileName1, string fileName2, AssetsManager* assetsManager)
{
	pAssetsManager = assetsManager;

	this->fileName = fileName1;
	this->fileNameSecond = fileName2;

	fileNum = 2;
	SetName("blend" + fileName1 + "&" + fileName2);

	AnimationData* animData1 = pAssetsManager->LoadAnimationData(fileName1);
	AnimationData* animData2 = pAssetsManager->LoadAnimationData(fileName2);

	this->oneFrameValue = animData1->GetOneFrameValue();

	this->frameNum = animData1->GetFrameNum();

	BlendMtxNode* bmtxTreeRoot = new BlendMtxNode();

	bmtxTreeRoot->LoadAnimation(animData1->GetMtxTreeRoot(),animData2->GetMtxTreeRoot(), nullptr, this);

	mtxTreeRoot = bmtxTreeRoot;

}

void AnimationData::LoadAnimation(string fileName1, string fileName2, string fileName3, string fileName4, AssetsManager* assetsManager)
{
	pAssetsManager = assetsManager;

	this->fileName = fileName1;
	this->fileNameSecond = fileName2;
	this->fileName3 = fileName3;
	this->fileName4 = fileName4;

	fileNum = 4;
	SetName("blend" + fileName1 + "&" + fileName2 + "&" + fileName3 + "&" + fileName4);



	AnimationData* animData1 = pAssetsManager->LoadAnimationData(fileName1);
	AnimationData* animData2 = pAssetsManager->LoadAnimationData(fileName2);
	AnimationData* animData3 = pAssetsManager->LoadAnimationData(fileName3);
	AnimationData* animData4 = pAssetsManager->LoadAnimationData(fileName4);

	this->oneFrameValue = animData1->GetOneFrameValue();

	this->frameNum = animData1->GetFrameNum();

	AngleBlendMtxNode* bmtxTreeRoot = new AngleBlendMtxNode();

	bmtxTreeRoot->LoadAnimation(animData1->GetMtxTreeRoot(), animData2->GetMtxTreeRoot(), animData3->GetMtxTreeRoot(), animData4->GetMtxTreeRoot(), nullptr, this);

	mtxTreeRoot = bmtxTreeRoot;

}

int AnimationData::GetFileNum(void)
{
	return fileNum;
}

BlendMtxNode::BlendMtxNode()
{
}

BlendMtxNode::~BlendMtxNode()
{
}

void BlendMtxNode::LoadAnimation(MtxNode* node1, MtxNode* node2,MtxNode* parent, AnimationData* animdata)
{

	name = node1->GetName();
	pAnimData = animdata;
	this->parent = parent;
	if (parent == nullptr)
	{
		this->isRoot = TRUE;
		this->attribute = Attribute::Root;


	}
	else
	{

		if (node1->GetAttribute() == Attribute::Mesh)
		{
			this->attribute = Attribute::Mesh;
		}
		else if (node1->GetAttribute() == Attribute::Bone)
		{
			this->attribute = Attribute::Bone;
		}
		else
		{
			this->attribute = Attribute::Null;
		}
		this->isRoot = FALSE;

		mtx1 = node1;
		mtx2 = node2;

	}


	for (int i = 0; i < node1->GetChildCnt(); i++)
	{
		MtxNode* child1 = node1->GetChild(i);
		MtxNode* child2 = node2->GetChild(i);

		if (child1->GetAttribute()== Attribute::Mesh)
		{
			BlendMtxNode* childNode = new BlendMtxNode();
			childNode->LoadAnimation(child1, child2, this, pAnimData);
			childArray.push_back(childNode);
		}
		else if (child1->GetAttribute() == Attribute::Bone)
		{
			BlendMtxNode* childNode = new BlendMtxNode();
			childNode->LoadAnimation(child1, child2, this, pAnimData);
			childArray.push_back(childNode);
		}
		else
		{
			BlendMtxNode* childNode = new BlendMtxNode();
			childNode->LoadAnimation(child1, child2, this, pAnimData);
			childArray.push_back(childNode);

		}
	}


}

XMMATRIX BlendMtxNode::GetFrameMtx(float time, float weight)
{

	float f = time / (pAnimData->GetFrameNum() / 60.0f);



	return 	(mtx1->GetFrameMtxLerp(f) * weight) + (mtx2->GetFrameMtxLerp(f) * (1.0f - weight));
}

AngleBlendMtxNode::AngleBlendMtxNode()
{
}

AngleBlendMtxNode::~AngleBlendMtxNode()
{
}

void AngleBlendMtxNode::LoadAnimation(MtxNode* nodeFront, MtxNode* nodeRight, MtxNode* nodeBack, MtxNode* nodeLeft, MtxNode* parent, AnimationData* animdata)
{
	name = nodeFront->GetName();
	pAnimData = animdata;
	this->parent = parent;
	if (parent == nullptr)
	{
		this->isRoot = TRUE;
		this->attribute = Attribute::Root;


	}
	else
	{

		if (nodeFront->GetAttribute() == Attribute::Mesh)
		{
			this->attribute = Attribute::Mesh;
		}
		else if (nodeFront->GetAttribute() == Attribute::Bone)
		{
			this->attribute = Attribute::Bone;
		}
		else
		{
			this->attribute = Attribute::Null;
		}
		this->isRoot = FALSE;

		mtxFront = nodeFront;
		mtxRight = nodeRight;
		mtxBack = nodeBack;
		mtxLeft = nodeLeft;

	}


	for (int i = 0; i < nodeFront->GetChildCnt(); i++)
	{
		MtxNode* childf = nodeFront->GetChild(i);
		MtxNode* childr = nodeRight->GetChild(i);
		MtxNode* childb = nodeBack->GetChild(i);
		MtxNode* childl = nodeLeft->GetChild(i);

		if (childf->GetAttribute() == Attribute::Mesh)
		{
			AngleBlendMtxNode* childNode = new AngleBlendMtxNode();
			childNode->LoadAnimation(childf, childr, childb, childl , this, pAnimData);
			childArray.push_back(childNode);
		}
		else if (childf->GetAttribute() == Attribute::Bone)
		{
			AngleBlendMtxNode* childNode = new AngleBlendMtxNode();
			childNode->LoadAnimation(childf, childr, childb, childl, this, pAnimData);
			childArray.push_back(childNode);
		}
		else
		{
			AngleBlendMtxNode* childNode = new AngleBlendMtxNode();
			childNode->LoadAnimation(childf, childr, childb, childl, this, pAnimData);
			childArray.push_back(childNode);

		}
	}

}

XMMATRIX AngleBlendMtxNode::GetFrameMtx(float time, float angle)
{
	float f = time / (pAnimData->GetFrameNum() / 60.0f);

	while (angle > XM_2PI)
	{
		angle -= XM_2PI;
	}

	XMMATRIX frameMtx = XMMatrixIdentity();

	XMVECTOR scl;
	XMVECTOR pos;

	XMVECTOR scl1;
	XMVECTOR rot1;
	XMVECTOR pos1;

	XMVECTOR scl2;
	XMVECTOR rot2;
	XMVECTOR pos2;



	if (0.0f <= angle && angle < XM_PIDIV2)
	{
		XMMatrixDecompose(&scl1, &rot1, &pos1, mtxFront->GetFrameMtxLerp(f));
		XMMatrixDecompose(&scl2, &rot2, &pos2, mtxRight->GetFrameMtxLerp(f));

	}
	else if (XM_PIDIV2 <= angle && angle < XM_PIDIV2 * 2)
	{
		angle -= XM_PIDIV2 * 1.0f;

		XMMatrixDecompose(&scl1, &rot1, &pos1, mtxRight->GetFrameMtxLerp(f));
		XMMatrixDecompose(&scl2, &rot2, &pos2, mtxBack->GetFrameMtxLerp(f));

	}
	else if (XM_PIDIV2 * 2 <= angle && angle < XM_PIDIV2 * 3)
	{
		angle -= XM_PIDIV2 * 2.0f;

		XMMatrixDecompose(&scl1, &rot1, &pos1, mtxBack->GetFrameMtxLerp(f));
		XMMatrixDecompose(&scl2, &rot2, &pos2, mtxLeft->GetFrameMtxLerp(f));

	}
	else if (XM_PIDIV2 * 3 <= angle && angle < XM_PIDIV2 * 4)
	{
		angle -= XM_PIDIV2 * 3.0f;
		XMMatrixDecompose(&scl1, &rot1, &pos1, mtxLeft->GetFrameMtxLerp(f));
		XMMatrixDecompose(&scl2, &rot2, &pos2, mtxFront->GetFrameMtxLerp(f));
	}
	else
	{
		return XMMatrixIdentity();
	}

	if (angle < 0.0f)
		angle = 0.0f;
	float weight2 = angle / XM_PIDIV2;
	float weight1 = 1.0f - weight2;;


	//scl = (scl1 * weight1) + (scl2 * weight2);
	scl = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	pos = (pos1 * weight1) + (pos2 * weight2);

	XMMATRIX rotMtx = (XMMatrixRotationQuaternion(rot1) * weight1) + (XMMatrixRotationQuaternion(rot2) * weight2);

	frameMtx = XMMatrixScalingFromVector(scl) * rotMtx * XMMatrixTranslationFromVector(pos);

	return frameMtx;
}
