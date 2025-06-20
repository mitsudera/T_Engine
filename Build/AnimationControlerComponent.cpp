#include "AnimationControlerComponent.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "AnimationData.h"
#include "transformcomponent.h"
#include "input.h"
#include "BoneComponent.h"

AnimationControlerComponent::AnimationControlerComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
	pAssetsManager = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager();

}

AnimationControlerComponent::~AnimationControlerComponent()
{
}

void AnimationControlerComponent::Awake(void)
{
	Component::Awake();
	fTransition = new AnimationForceTransition(this);
	loadFileName[0] = "";
	loadFileName[1] = "";
	loadFileName[2] = "";
	loadFileName[3] = "";
	createConditionName = "";
	TypeName = typeid(AnimationControlerComponent).name();;

}

void AnimationControlerComponent::Init(void)
{
	Component::Init();
	angle = 0.0f;
	timeCnt = 0.0f;
	activeAnim = defaultNode;
}

void AnimationControlerComponent::Update(void)
{
	Component::Update();
	if (!activeAnim)
		return;

	this->activeAnim->UpdateAnimation(pGameObject);

	for (pair<AnimParameter, string>& condition : conditionList)
	{
		if (condition.first.isTrigger)
		{
			condition.first.value = FALSE;
		}
	}
}

void AnimationControlerComponent::Uninit(void)
{
	Component::Uninit();
	delete fTransition;
	for (AnimationNode* node : AnimNodeList)
	{
		delete node;
	}
}

void AnimationControlerComponent::LoadDefaulAnimation(string fileName, string name)
{
	AnimationNode* node = new AnimationNode(this);
	node->CreateNode(fileName, name, TRUE);
	defaultNode = node;
	activeAnim = node;
	AnimNodeList.push_back(node);
}

void AnimationControlerComponent::LoadDefaulAnimationAssets(string assetsName, string name)
{
	AnimationNode* node = new AnimationNode(this);
	node->CreateNode(assetsName, name, FALSE, AnimationNode::Blend::None);
	defaultNode = node;
	activeAnim = node;

	AnimNodeList.push_back(node);

}

void AnimationControlerComponent::LoadDefaulAnimation(AnimationNode* node)
{
	AnimationNode* newnode = new AnimationNode(this);
	newnode->CreateNode(node);
	defaultNode = newnode;
	activeAnim = newnode;
	AnimNodeList.push_back(newnode);

}

void AnimationControlerComponent::LoadAnimation(string fileName, string name, BOOL loop)
{
	AnimationNode* node = new AnimationNode(this);
	node->CreateNode(fileName, name, loop);
	AnimNodeList.push_back(node);
	
}

void AnimationControlerComponent::LoadAnimation(string animName, string name, BOOL loop, AnimationNode::Blend blend)
{
	AnimationNode* node = new AnimationNode(this);
	node->CreateNode(animName, name, loop, blend);
	AnimNodeList.push_back(node);

}

void AnimationControlerComponent::LoadAnimation(string fileName1, string fileName2, string name, BOOL loop)
{
	AnimationNode* node = new AnimationNode(this);
	node->CreateNode(fileName1, fileName2, name, loop);
	AnimNodeList.push_back(node);

}

void AnimationControlerComponent::LoadAnimation(string fileNameFront, string fileNameRight, string fileNameBack, string fileNameLeft, string name, BOOL loop)
{
	AnimationNode* node = new AnimationNode(this);
	node->CreateNode(fileNameFront, fileNameRight, fileNameBack, fileNameLeft, name, loop);
	AnimNodeList.push_back(node);

}

void AnimationControlerComponent::LoadAnimation(bool loop)
{
	if (loadFileName[0].empty())
		return;

	LoadAnimation(loadFileName[0], loadFileName[0], loop);
}

void AnimationControlerComponent::LoadAnimation2(bool loop)
{
	if (loadFileName[0].empty()|| loadFileName[1].empty())
		return;

	LoadAnimation(loadFileName[0], loadFileName[1], loadFileName[0] + "blend", loop);
}

void AnimationControlerComponent::LoadAnimation4(bool loop)
{
	if (loadFileName[0].empty() || loadFileName[1].empty()||loadFileName[2].empty() || loadFileName[3].empty())
		return;

	LoadAnimation(loadFileName[0], loadFileName[1],loadFileName[2], loadFileName[3], loadFileName[0] + "blend", loop);

}

void AnimationControlerComponent::LoadAnimation(AnimationNode* node)
{
	AnimationNode* newnode = new AnimationNode(this);
	newnode->CreateNode(node);
	AnimNodeList.push_back(newnode);

}

void AnimationControlerComponent::CreateTransition(
	string beforeAnimName,//このアニメーションから
	string afterAnimName,//このアニメーションに
	string conditionName,//このコンディションが
	BOOL needCondition,//この値になった時に
	float transitionTime)//この時間をかけて移行する
{

	AnimationNode* beforeNode = nullptr;
	AnimationNode* afterNode = nullptr;

	for (AnimationNode* node:AnimNodeList)
	{
		if (node->GetName()==beforeAnimName)
		{
			beforeNode = node;
			break;
		}
		
	}
	if (beforeNode == nullptr)
		return;

	for (AnimationNode* node:AnimNodeList)
	{
		if (node->GetName()==afterAnimName)
		{
			afterNode = node;
		}
		
	}
	if (afterNode == nullptr)
		return;


	AnimationTransition* transition = new AnimationTransition(this);
	transition->CreateTransition(beforeNode, afterNode, transitionTime, conditionName, needCondition);
	beforeNode->AddTransition(transition);
	transition->isExit = false;

	transitionList.push_back(transition);

}

void AnimationControlerComponent::CreateTransition(string beforeAnimName, string afterAnimName, string conditionName, BOOL needCondition, int transitionFrame)
{
	AnimationNode* beforeNode = nullptr;
	AnimationNode* afterNode = nullptr;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == beforeAnimName)
		{
			beforeNode = node;
			break;
		}

	}
	if (beforeNode == nullptr)
		return;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == afterAnimName)
		{
			afterNode = node;
		}

	}
	if (afterNode == nullptr)
		return;


	float transitionTime = ((float)transitionFrame) / 60.0f;

	AnimationTransition* transition = new AnimationTransition(this);
	transition->CreateTransition(beforeNode, afterNode, transitionTime, conditionName, needCondition);
	beforeNode->AddTransition(transition);
	transition->isExit = false;

	transitionList.push_back(transition);

}

void AnimationControlerComponent::CreateTransition(string beforeAnimName, string afterAnimName, string conditionName, BOOL needCondition)
{
	AnimationNode* beforeNode = nullptr;
	AnimationNode* afterNode = nullptr;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == beforeAnimName)
		{
			beforeNode = node;
			break;
		}

	}
	if (beforeNode == nullptr)
		return;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == afterAnimName)
		{
			afterNode = node;
		}

	}
	if (afterNode == nullptr)
		return;



	AnimationTransition* transition = new AnimationTransition(this);
	transition->CreateTransition(beforeNode, afterNode, 0.25f, conditionName, needCondition);
	beforeNode->AddTransition(transition);
	transition->isExit = false;

	transitionList.push_back(transition);

}

void AnimationControlerComponent::CreateNotLoopAnimExitTransition(
	string beforeAnimName,//このアニメーションから
	string afterAnimName,//このアニメーションに
	float transitionTime)//この時間をかけて移行する
{

	AnimationNode* beforeNode = nullptr;
	AnimationNode* afterNode = nullptr;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == beforeAnimName)
		{
			beforeNode = node;
			break;
		}

	}
	if (beforeNode == nullptr)
		return;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == afterAnimName)
		{
			afterNode = node;
		}

	}
	if (afterNode == nullptr)
		return;


	AnimationTransition* transition = new AnimationTransition(this);
	transition->CreateExitTransition(beforeNode, afterNode, transitionTime);
	beforeNode->SetNotLoopExitTransition(transition);
	transition->isExit = true;

	transitionList.push_back(transition);

}

void AnimationControlerComponent::CreateNotLoopAnimExitTransition(string beforeAnimName, string afterAnimName, int transitionFrame)
{
	AnimationNode* beforeNode = nullptr;
	AnimationNode* afterNode = nullptr;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == beforeAnimName)
		{
			beforeNode = node;
			break;
		}

	}
	if (beforeNode == nullptr)
		return;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == afterAnimName)
		{
			afterNode = node;
		}

	}
	if (afterNode == nullptr)
		return;

	float transitionTime = ((float)transitionFrame) / 60.0f;

	AnimationTransition* transition = new AnimationTransition(this);
	transition->CreateExitTransition(beforeNode, afterNode, transitionTime);
	beforeNode->SetNotLoopExitTransition(transition);
	transition->isExit = true;

	transitionList.push_back(transition);

}

void AnimationControlerComponent::CreateNotLoopAnimExitTransition(string beforeAnimName, string afterAnimName)
{
	AnimationNode* beforeNode = nullptr;
	AnimationNode* afterNode = nullptr;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == beforeAnimName)
		{
			beforeNode = node;
			break;
		}

	}
	if (beforeNode == nullptr)
		return;

	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == afterAnimName)
		{
			afterNode = node;
		}

	}
	if (afterNode == nullptr)
		return;


	AnimationTransition* transition = new AnimationTransition(this);
	transition->CreateExitTransition(beforeNode, afterNode, 0.25f);
	beforeNode->SetNotLoopExitTransition(transition);
	transition->isExit = true;
	transitionList.push_back(transition);

}

AssetsManager* AnimationControlerComponent::GetAssetsmanager(void)
{
	return this->pAssetsManager;
}

GameEngine* AnimationControlerComponent::GetGameEngine(void)
{
	return this->pGameEngine;
}

void AnimationControlerComponent::SetBlendWeight(float weight)
{
	blendWeight = weight;
}

float AnimationControlerComponent::GetBlendWeight(void)
{
	return blendWeight;
}

void AnimationControlerComponent::SetAngle(float angle)
{
	while (angle < 0.0f)
	{
		angle += XM_2PI;
	}
	while (angle >= XM_2PI)
	{
		angle -= XM_2PI;
	}
	this->angle = angle;
}

float AnimationControlerComponent::GetAngle(void)
{
	return this->angle;
}

BOOL AnimationControlerComponent::GetIsTransition(void)
{
	if (dynamic_cast<AnimationTransition*>(this->activeAnim))
	{
		return TRUE;
	}
	return FALSE;
}

void AnimationControlerComponent::StopAnim(void)
{
	stop = TRUE;
}

void AnimationControlerComponent::StartAnim(void)
{
	stop = FALSE;
}

BOOL AnimationControlerComponent::GetStop(void)
{
	return this->stop;
}

void AnimationControlerComponent::StartForceTransition(string name, float time)
{
	AnimationNode* anode = nullptr;
	for (AnimationNode* node : AnimNodeList)
	{
		if (node->GetName() == name)
		{
			node = node;
			break;
		}

	}
	if (anode == nullptr)
		return;
	
	fTransition->StartTransition(anode, 0.0f, time);
}

list<AnimationNode*>& AnimationControlerComponent::GetAnimNodeList(void)
{
	return AnimNodeList;
}

list<pair<AnimParameter, string>>& AnimationControlerComponent::GetConditionList(void)
{
	return conditionList;
}

list<AnimationTransition*>& AnimationControlerComponent::GetTransitionList(void)
{
	return transitionList;
}



string AnimationControlerComponent::GetLoadFileName(int slot)
{
	if (slot >= 4 || slot < 0)
	{
		return "";
	}

	return loadFileName[slot];
}

void AnimationControlerComponent::SetLoadFileName(string fName, int slot)
{
	if (slot >= 4 || slot < 0)
	{
		return;
	}


	loadFileName[slot] = fName;
}

AnimationNode* AnimationControlerComponent::GetDefaultNode(void)
{
	return defaultNode;
}




void AnimationControlerComponent::UpdateAnimation(MtxNode* node, GameObject* gameObject)
{

	if (node->GetAttribute()==MtxNode::Attribute::Bone&&gameObject->GetComponent<BoneComponent>()->GetIsPhysics())
	{

	}
	else
	{
		XMMATRIX frameMtx = node->GetFrameMtx(timeCnt);
		gameObject->GetTransFormComponent()->SetLocalMtxNotScaling(frameMtx);

	}

	for (int i = 0; i < node->GetChildCnt(); i++)
	{
		MtxNode* child = node->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateAnimation(child, childObj);

	}

}
void AnimationControlerComponent::CreateCondition(string name, AnimParameter initValue)
{


	conditionList.push_back(make_pair(initValue, name));

}
void AnimationControlerComponent::SetCondition(string name, BOOL setValue)
{
	for (pair<AnimParameter,string>& condition:conditionList)
	{
		if (condition.second == name)
		{
			condition.first.value = setValue;
		}
	}
}

BOOL AnimationControlerComponent::GetCondition(string name)
{
	for (pair<AnimParameter, string> condition : conditionList)
	{
		if (condition.second == name)
		{
			return condition.first.value;
		}
	}
	return 0;
}

void AnimationControlerComponent::SetIsTrigger(string name, BOOL b)
{
	for (pair<AnimParameter, string>& condition : conditionList)
	{
		if (condition.second == name)
		{
			condition.first.isTrigger = b;
		}
	}

}


void AnimationControlerComponent::SetActiveAnimation(Animation* animation)
{
	this->activeAnim = animation;
}

AnimationTransition::AnimationTransition(AnimationControlerComponent* controler)
{
	this->controler = controler;
	this->pAssetsManager = controler->GetAssetsmanager();
	this->pGameEngine = controler->GetGameEngine();
}

AnimationTransition::~AnimationTransition()
{
}

void AnimationTransition::CreateTransition(AnimationNode* beforeAnimNode, AnimationNode* afterAnimNode, float transitionTime, string coditionName,BOOL needCondition)
{
	this->beforeAnimNode = beforeAnimNode;
	this->beforeAnimData = beforeAnimNode->GetAnimData();
	this->afterAnimNode = afterAnimNode;
	this->afterAnimData = afterAnimNode->GetAnimData();
	this->transitionTime = transitionTime;
	this->needConditionName = coditionName;
	this->needCondition = needCondition;
	this->name = beforeAnimNode->GetName() + "To" + afterAnimNode->GetName();
}

void AnimationTransition::CreateExitTransition(AnimationNode* beforeAnimNode, AnimationNode* afterAnimNode, float transitionTime)
{
	this->beforeAnimNode = beforeAnimNode;
	this->beforeAnimData = beforeAnimNode->GetAnimData();
	this->afterAnimNode = afterAnimNode;
	this->afterAnimData = afterAnimNode->GetAnimData();
	this->transitionTime = transitionTime;
	this->name = beforeAnimNode->GetName() + "Exit" + afterAnimNode->GetName();

}



void AnimationTransition::UpdateAnimation(GameObject* gameObject)
{
	MtxNode* root1 = beforeAnimData->GetMtxTreeRoot();
	MtxNode* root2 = afterAnimData->GetMtxTreeRoot();


	weight2 = timeCnt / transitionTime;
	weight1 = 1.0f - weight2;

	for (int i = 0; i < root1->GetChildCnt(); i++)
	{
		MtxNode* child1 = root1->GetChild(i);
		MtxNode* child2 = root2->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateMtx(child1,child2, childObj);

	}


	if (!controler->GetStop())
	{
		float deltaTime = pGameEngine->GetDeltaTime();
		timeCnt += deltaTime;
		beforeAnimCnt += deltaTime;
		afterAnimCnt += deltaTime;

	}




	if (timeCnt > transitionTime)
	{
		afterAnimNode->StartAnimation(timeCnt);
	}



}

float AnimationTransition::GetTransitionTime(void)
{
	return transitionTime;
}

BOOL AnimationTransition::CheckCondition(void)
{
	if (controler->GetCondition(this->needConditionName) == needCondition)
	{
		return TRUE;

	}
	return FALSE;
}

void AnimationTransition::StartTransition(float beforeAnimCnt, float afterAnimCnt)
{
	timeCnt = afterAnimCnt;
	this->afterAnimCnt = afterAnimCnt;
	this->beforeAnimCnt = beforeAnimCnt;
	this->controler->SetActiveAnimation(this);

}

string AnimationTransition::GetNeedConditionName(void)
{
	return this->needConditionName;
}

BOOL AnimationTransition::GetNeedCondition(void)
{
	return this->needCondition;
}

void AnimationTransition::SetNeedConditionName(string name)
{
	this->needConditionName = name;
}

void AnimationTransition::SetNeedCondition(BOOL b)
{
	this->needCondition = b;
}

AnimationNode* AnimationTransition::GetBeforeAnimNode(void)
{
	return beforeAnimNode;
}

AnimationNode* AnimationTransition::GetAfterAnimNode(void)
{
	return afterAnimNode;
}

void AnimationTransition::UpdateMtx(MtxNode* node1, MtxNode* node2 , GameObject* gameObject)
{


	if (node1->GetAttribute() == MtxNode::Attribute::Bone && gameObject->GetComponent<BoneComponent>()->GetIsPhysics())
	{

	}
	else
	{
		if (!gameObject->GetNotAnim())
		{

			XMMATRIX frameMtx1;
			XMMATRIX frameMtx2;

			switch (beforeAnimNode->GetBlend())
			{
			case AnimationNode::Blend::None:
			{
				frameMtx1 = node1->GetFrameMtx(beforeAnimCnt);

			}

			break;
			case AnimationNode::Blend::Double:
			{
				frameMtx1 = node1->GetFrameMtx(timeCnt, controler->GetBlendWeight());

			}

			break;
			case AnimationNode::Blend::Angle:
			{
				frameMtx1 = node1->GetFrameMtx(timeCnt, controler->GetAngle());

			}

			break;
			default:
				break;
			}


			switch (afterAnimNode->GetBlend())
			{
			case AnimationNode::Blend::None:
			{
				frameMtx2 = node2->GetFrameMtx(afterAnimCnt);

			}

			break;
			case AnimationNode::Blend::Double:
			{
				frameMtx2 = node2->GetFrameMtx(timeCnt, controler->GetBlendWeight());

			}

			break;
			case AnimationNode::Blend::Angle:
			{
				frameMtx2 = node2->GetFrameMtx(timeCnt, controler->GetAngle());

			}

			break;
			default:
				break;
			}

			gameObject->GetTransFormComponent()->SetLocalMtxNotScaling(frameMtx1, weight1, frameMtx2, weight2);
		}
	}


	for (int i = 0; i < node1->GetChildCnt(); i++)
	{
		MtxNode* child1 = node1->GetChild(i);
		MtxNode* child2 = node2->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateMtx(child1, child2, childObj);

	}


}


AnimationNode::AnimationNode(AnimationControlerComponent* controler)
{
	this->controler = controler;
	this->pAssetsManager = controler->GetAssetsmanager();
	this->pGameEngine = controler->GetGameEngine();
	this->exitTransition = nullptr;
}

AnimationNode::~AnimationNode()
{
	for (AnimationTransition* tra : transitionList)
	{
		delete tra;
	}
}


void AnimationNode::UpdateAnimation(GameObject* gameObject)
{
	MtxNode* root = animData->GetMtxTreeRoot();



	if (!controler->GetStop())
		timeCnt += pGameEngine->GetDeltaTime();

	if (loop)
	{
		while(timeCnt > endTime)
		{
			timeCnt -= endTime;
		}



	}
	else
	{
		if (timeCnt > exitTime)
		{

			float overTime = timeCnt - exitTime;
			if (exitTransition != nullptr)
			{
				exitTransition->StartTransition(timeCnt, overTime);
			}
			if (timeCnt > endTime)
			{
				timeCnt = endTime;
				controler->StopAnim();
			}

		}


	}
	for (AnimationTransition* transition : transitionList)
	{

		if (transition->CheckCondition())
		{
			transition->StartTransition(timeCnt, 0.0f);
			break;
		}
	}

	for (int i = 0; i < root->GetChildCnt(); i++)
	{
		MtxNode* child = root->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateMtx(child, childObj);
	}


}

void AnimationNode::CreateNode(string fileName, string name, BOOL loop)
{
	this->animData = pAssetsManager->LoadAnimationData(fileName);
	this->name = name;
	this->loop = loop;
	this->endTime = ((float)animData->GetFrameNum() - 1) / 60.0f;
	exitTime = endTime;
	animEnd = FALSE;
	blend = Blend::None;
}

void AnimationNode::CreateNode(string fileName1, string fileName2, string name, BOOL loop)
{
	this->animData = pAssetsManager->LoadAnimationData(fileName1, fileName2);
	this->name = name;
	this->loop = loop;
	this->endTime = ((float)animData->GetFrameNum() - 1) / 60.0f;
	exitTime = endTime;
	animEnd = FALSE;

	blend = Blend::Double;
}

void AnimationNode::CreateNode(string fileName1, string fileName2, string fileName3, string fileName4, string name, BOOL loop)
{
	this->animData = pAssetsManager->LoadAnimationData(fileName1, fileName2, fileName3, fileName4);
	this->name = name;
	this->loop = loop;
	this->endTime = ((float)animData->GetFrameNum() - 1) / 60.0f;
	exitTime = endTime;
	animEnd = FALSE;

	blend = Blend::Angle;

}

void AnimationNode::CreateNode(AnimationData* animData, string name, BOOL loop, Blend blend)
{
	this->animData = animData;
	this->name = name;
	this->loop = loop;
	this->endTime = ((float)animData->GetFrameNum() - 1) / 60.0f;
	exitTime = endTime;
	animEnd = FALSE;

	this->blend = blend;

}

void AnimationNode::CreateNode(AnimationNode* node)
{
	this->animData = node->animData;
	this->name = node->name;
	this->loop = node->loop;
	this->endTime = node->endTime;
	this->exitTime = node->exitTime;
	this->animEnd = node->animEnd;

	this->blend = node->blend;

}

void AnimationNode::CreateNode(string animName, string name, BOOL loop, Blend blend)
{
	this->animData = pAssetsManager->GetAnimationData(animName);
	this->name = name;
	this->loop = loop;
	this->endTime = ((float)animData->GetFrameNum() - 1) / 60.0f;
	exitTime = endTime;
	animEnd = FALSE;

	this->blend = blend;

}


void AnimationNode::SetLoop(BOOL loop)
{
	this->loop = loop;
}

AnimationData* AnimationNode::GetAnimData(void)
{
	return this->animData;
}

void AnimationNode::AddTransition(AnimationTransition* transition)
{
	this->transitionList.push_back(transition);
}

void AnimationNode::SetNotLoopExitTransition(AnimationTransition* transition)
{
	loop = FALSE;
	this->exitTransition = transition;
	this->exitTime = this->endTime - transition->GetTransitionTime();
}

void AnimationNode::StartAnimation(float startTime)
{
	this->timeCnt = startTime;
	controler->SetActiveAnimation(this);
}

AnimationNode::Blend AnimationNode::GetBlend(void)
{
	return blend;
}

list<AnimationTransition*>& AnimationNode::GetTransitionList(void)
{
	return transitionList;
}

AnimationTransition* AnimationNode::GetExitTransition(void)
{
	return exitTransition;
}

BOOL AnimationNode::GetLoop(void)
{
	return loop;
}

void AnimationNode::UpdateMtx(MtxNode* node, GameObject* gameObject)
{
	if (node->GetAttribute() == MtxNode::Attribute::Bone && gameObject->GetComponent<BoneComponent>()->GetIsPhysics())
	{

	}
	else
	{
		if (!gameObject->GetNotAnim())
		{
			switch (blend)
			{
			case AnimationNode::Blend::None:
			{
				XMMATRIX frameMtx = node->GetFrameMtx(timeCnt);
				gameObject->GetTransFormComponent()->SetLocalMtxNotScaling(frameMtx);

			}

			break;
			case AnimationNode::Blend::Double:
			{
				XMMATRIX frameMtx = node->GetFrameMtx(timeCnt, controler->GetBlendWeight());
				gameObject->GetTransFormComponent()->SetLocalMtxNotScaling(frameMtx);

			}

			break;
			case AnimationNode::Blend::Angle:
			{
				XMMATRIX frameMtx = node->GetFrameMtx(timeCnt, controler->GetAngle());
				gameObject->GetTransFormComponent()->SetLocalMtxNotScaling(frameMtx);

			}

			break;
			default:
				break;
			}
		}
	}


	for (int i = 0; i < node->GetChildCnt(); i++)
	{
		MtxNode* child = node->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateMtx(child, childObj);

	}

}

Animation::Animation()
{
	timeCnt = 0.0f;
}

Animation::~Animation()
{
}

float Animation::GetTimeCnt(void)
{
	return this->timeCnt;
}

void Animation::SetTimeCnt(float cnt)
{
	this->timeCnt = cnt;
}

string Animation::GetName(void)
{
	return this->name;
}

AnimationForceTransition::AnimationForceTransition(AnimationControlerComponent* controler)
{
	this->controler = controler;
	this->pAssetsManager = controler->GetAssetsmanager();
	this->pGameEngine = controler->GetGameEngine();

}

AnimationForceTransition::~AnimationForceTransition()
{
}

void AnimationForceTransition::UpdateAnimation(GameObject* gameObject)
{
	MtxNode* root = afterAnimData->GetMtxTreeRoot();


	weight2 = timeCnt / transitionTime;
	weight1 = 1.0f - weight2;

	for (int i = 0; i < root->GetChildCnt(); i++)
	{
		MtxNode* child = root->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateMtx(childObj, child);

	}


	if (!controler->GetStop())
	{
		float deltaTime = pGameEngine->GetDeltaTime();
		timeCnt += deltaTime;
		afterAnimCnt += deltaTime;

	}




	if (timeCnt > transitionTime)
	{
		afterAnimNode->StartAnimation(timeCnt);
	}


}

float AnimationForceTransition::GetTransitionTime(void)
{
	return transitionTime;
}


void AnimationForceTransition::StartTransition(AnimationNode* afterAnim, float afterAnimCnt, float transitionTime)
{
	timeCnt = 0.0f;
	this->transitionTime = transitionTime;
	this->afterAnimCnt = afterAnimCnt;
	controler->SetActiveAnimation(this);
}

void AnimationForceTransition::UpdateMtx(GameObject* gameObject, MtxNode* node)
{
	if (node->GetAttribute() == MtxNode::Attribute::Bone && gameObject->GetComponent<BoneComponent>()->GetIsPhysics())
	{

	}
	else
	{
		if (!gameObject->GetNotAnim())
		{

			XMMATRIX frameMtx1;
			XMMATRIX frameMtx2;

			frameMtx1 = gameObject->GetTransFormComponent()->GetLastLocalMtx();

			switch (afterAnimNode->GetBlend())
			{
			case AnimationNode::Blend::None:
			{
				frameMtx2 = node->GetFrameMtx(afterAnimCnt);

			}

			break;
			case AnimationNode::Blend::Double:
			{
				frameMtx2 = node->GetFrameMtx(timeCnt, controler->GetBlendWeight());

			}

			break;
			case AnimationNode::Blend::Angle:
			{
				frameMtx2 = node->GetFrameMtx(timeCnt, controler->GetAngle());

			}

			break;
			default:
				break;
			}

			gameObject->GetTransFormComponent()->SetLocalMtxNotScaling(frameMtx1, weight1, frameMtx2, weight2);
		}
	}


	for (int i = 0; i < node->GetChildCnt(); i++)
	{
		MtxNode* child = node->GetChild(i);
		GameObject* childObj = gameObject->GetChild(i);

		UpdateMtx(childObj, child);

	}

}
