#pragma once
#include "component.h"

class AssetsManager;
class MeshData;
class MtxNode;
class AnimationControlerComponent;
class AnimationData;
class AnimationNode;


struct AnimParameter
{
	BOOL value;
	BOOL isTrigger;
};

class Animation
{
public:
	Animation();
	~Animation();

	virtual void UpdateAnimation(GameObject* gameObject)=0;

	float GetTimeCnt(void);
	void SetTimeCnt(float cnt);
	string GetName(void);

protected:
	AnimationControlerComponent* controler;
	AssetsManager* pAssetsManager;
	GameEngine* pGameEngine;
	float timeCnt;
	string name;

};

class AnimationTransition:public Animation
{
public:
	AnimationTransition(AnimationControlerComponent* controler);
	~AnimationTransition();

	void CreateTransition(AnimationNode* beforeAnimNode, AnimationNode* afterAnimNode, float transitionTime, string coditionName,BOOL needCondition);
	void CreateExitTransition(AnimationNode* beforeAnimNode, AnimationNode* afterAnimNode, float transitionTime);

	virtual void UpdateAnimation(GameObject* gameObject) override;
	float GetTransitionTime(void);

	BOOL CheckCondition(void);

	void StartTransition(float beforeAnimCnt,float afterAnimCnt);

	string GetNeedConditionName(void);
	BOOL GetNeedCondition(void);

	void SetNeedConditionName(string name);
	void SetNeedCondition(BOOL b);

	AnimationNode* GetBeforeAnimNode(void);

	AnimationNode* GetAfterAnimNode(void);

	bool isExit;


private:
	AnimationNode* beforeAnimNode;
	AnimationNode* afterAnimNode;
	float transitionTime;
	AnimationData* beforeAnimData;
	AnimationData* afterAnimData;
	float beforeAnimCnt;
	float afterAnimCnt;
 
	float weight1;
	float weight2;

	BOOL needCondition;//�ڍs����true�ɂȂ�������false�ɂȂ��������H
	string needConditionName;//�ڍs����true�ɂȂ�������false�ɂȂ��������H


	void UpdateMtx(MtxNode* node1, MtxNode* node2, GameObject* gameObject);

};

class AnimationForceTransition:public Animation
{
public:
	AnimationForceTransition(AnimationControlerComponent* controler);
	~AnimationForceTransition();

	virtual void UpdateAnimation(GameObject* gameObject) override;
	float GetTransitionTime(void);


	void StartTransition(AnimationNode* afterAnim, float afterAnimCnt,float transitionTime);

	void UpdateMtx(GameObject* gameObject, MtxNode* node);

private:
	AnimationNode* afterAnimNode;
	float transitionTime;
	AnimationData* afterAnimData;
	float afterAnimCnt;
 
	float weight1;
	float weight2;




};

class AnimationNode :public Animation
{
public:

	enum class Blend:int
	{
		None,
		Double,
		Angle,
	};

	AnimationNode(AnimationControlerComponent* controler);
	~AnimationNode();


	virtual void UpdateAnimation(GameObject* gameObject)override;

	void CreateNode(string fileName, string name,BOOL loop);
	void CreateNode(string fileName1,string fileName2, string name,BOOL loop);
	void CreateNode(string fileName1,string fileName2,string fileName3,string fileName4, string name,BOOL loop);
	void CreateNode(AnimationData* animData, string name, BOOL loop, Blend blend);
	void CreateNode(AnimationNode* node);
	void CreateNode(string animName, string name, BOOL loop, Blend blend);
	void SetLoop(BOOL loop);
	AnimationData* GetAnimData(void);


	void AddTransition(AnimationTransition* transition);

	void SetNotLoopExitTransition(AnimationTransition* transition);

	void StartAnimation(float startTime);

	Blend GetBlend(void);


	list<AnimationTransition*>& GetTransitionList(void);

	AnimationTransition* GetExitTransition(void);

	BOOL GetLoop(void);

private:
	AnimationData* animData;
	float endTime;
	float exitTime;
	BOOL animEnd;
	BOOL loop;

	Blend blend;

	list<AnimationTransition*> transitionList;
	AnimationTransition* exitTransition;

	void UpdateMtx(MtxNode* node, GameObject* gameObject);
};

class AnimationControlerComponent : public Component
{
public:

	enum class ANIM_STATE:int
	{
		STOP,
		TRANSFER,
		ANIMATION,
	};


	AnimationControlerComponent(GameObject* gameObject);
	~AnimationControlerComponent();


	virtual void Awake(void) override;
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	void LoadDefaulAnimation(string fileName, string name);
	void LoadDefaulAnimationAssets(string assetsName, string name);
	void LoadDefaulAnimation(AnimationNode* node);

	void LoadAnimation(string fileName, string name, BOOL loop);
	void LoadAnimation(string animName, string name, BOOL loop,AnimationNode::Blend blend);
	void LoadAnimation(string fileName1,string fileName2, string name, BOOL loop);
	void LoadAnimation(string fileNameFront,string fileNameRight,string fileNameBack,string fileNameLeft, string name, BOOL loop);
	void LoadAnimation(bool loop);
	void LoadAnimation2(bool loop);
	void LoadAnimation4(bool loop);
	void LoadAnimation(AnimationNode* node);

	void CreateTransition(
		string beforeAnimName,//���̃A�j���[�V��������
		string afterAnimName,//���̃A�j���[�V������
		string conditionName,//���̃R���f�B�V������
		BOOL needCondition,//���̒l�ɂȂ�������
		float transitionTime);//���̎��Ԃ������Ĉڍs����

	void CreateTransition(
		string beforeAnimName,//���̃A�j���[�V��������
		string afterAnimName,//���̃A�j���[�V������
		string conditionName,//���̃R���f�B�V������
		BOOL needCondition,//���̒l�ɂȂ�������
		int transitionFrame);//���̎��Ԃ������Ĉڍs����

	//transitionTime��0.25�b�Œ�
	void CreateTransition(
		string beforeAnimName,//���̃A�j���[�V��������
		string afterAnimName,//���̃A�j���[�V������
		string conditionName,//���̃R���f�B�V������
		BOOL needCondition);//���̒l�ɂȂ�������
	

	//���[�v���Ȃ��A�j���[�V�����p
	void CreateNotLoopAnimExitTransition(
		string beforeAnimName,//���̃A�j���[�V��������
		string afterAnimName,//���̃A�j���[�V������
		float transitionTime);//���̎��Ԃ������Ĉڍs����

	void CreateNotLoopAnimExitTransition(
		string beforeAnimName,//���̃A�j���[�V��������
		string afterAnimName,//���̃A�j���[�V������
		int transitionFrame);//���̎��Ԃ������Ĉڍs����

	//transitionTime��0.25�b�Œ�
	void CreateNotLoopAnimExitTransition(
		string beforeAnimName,//���̃A�j���[�V��������
		string afterAnimName);//���̃A�j���[�V������



	void CreateCondition(string name, AnimParameter initValue);
	void SetCondition(string name, BOOL setValue);
	BOOL GetCondition(string name);

	void SetIsTrigger(string name, BOOL b);

	void SetActiveAnimation(Animation* animation);


	AssetsManager* GetAssetsmanager(void);
	GameEngine* GetGameEngine(void);

	void SetBlendWeight(float weight);
	float GetBlendWeight(void);


	void SetAngle(float angle);
	float GetAngle(void);

	BOOL GetIsTransition(void);

	void StopAnim(void);
	void StartAnim(void);
	BOOL GetStop(void);

	void StartForceTransition(string name,float time);

	list<AnimationNode*>& GetAnimNodeList(void);
	list<pair<AnimParameter, string>>& GetConditionList(void);
	list<AnimationTransition*>& GetTransitionList(void);
	string GetLoadFileName(int slot);
	void SetLoadFileName(string fName,int slot);

	AnimationNode* GetDefaultNode(void);

	string createConditionName;

protected:
	string loadFileName[4];

	AnimationForceTransition* fTransition;

	void UpdateAnimation(MtxNode* node, GameObject* gameObject);

	AssetsManager* pAssetsManager;

	AnimationNode* defaultNode;

	list<AnimationNode*> AnimNodeList;

	list<pair<AnimParameter,string>> conditionList;

	list<AnimationTransition*> transitionList;


	Animation* activeAnim;

	float timeCnt;
	int framenum;
	BOOL animation;

	float animSpeed;

	BOOL frameBlendMode;


	float blendWeight;

	float angle;


	BOOL stop;


};

