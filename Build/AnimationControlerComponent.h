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

protected:
	AnimationControlerComponent* controler;
	AssetsManager* pAssetsManager;
	GameEngine* pGameEngine;
	float timeCnt;


};

class AnimationTransition:public Animation
{
public:
	AnimationTransition(AnimationControlerComponent* controler);
	~AnimationTransition();

	void CreateTransition(AnimationNode* beforeAnimNode, AnimationNode* afterAnimNode, float transitionTime, int coditionIndex,BOOL needCondition);
	void CreateExitTransition(AnimationNode* beforeAnimNode, AnimationNode* afterAnimNode, float transitionTime);

	virtual void UpdateAnimation(GameObject* gameObject) override;
	float GetTransitionTime(void);

	BOOL CheckCondition(void);

	void StartTransition(float beforeAnimCnt,float afterAnimCnt);

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

	BOOL needCondition;//移行条件trueになった時かfalseになった時か？
	int needConditionIndex;//移行条件trueになった時かfalseになった時か？


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
	void SetLoop(BOOL loop);
	AnimationData* GetAnimData(void);
	string GetName(void);


	void AddTransition(AnimationTransition* transition);

	void SetNotLoopExitTransition(AnimationTransition* transition);

	void StartAnimation(float startTime);

	Blend GetBlend(void);

private:
	AnimationData* animData;
	vector<AnimationTransition*> transitionArray;
	AnimationTransition* exitTransition;
	float endTime;
	float exitTime;
	BOOL animEnd;
	BOOL loop;
	string name;

	Blend blend;

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
	void LoadAnimation(string fileName, string name, BOOL loop);
	void LoadAnimation(string fileName1,string fileName2, string name, BOOL loop);
	void LoadAnimation(string fileNameFront,string fileNameRight,string fileNameBack,string fileNameLeft, string name, BOOL loop);

	void CreateTransition(
		string beforeAnimName,//このアニメーションから
		string afterAnimName,//このアニメーションに
		string conditionName,//このコンディションが
		BOOL needCondition,//この値になった時に
		float transitionTime);//この時間をかけて移行する

	void CreateTransition(
		string beforeAnimName,//このアニメーションから
		string afterAnimName,//このアニメーションに
		string conditionName,//このコンディションが
		BOOL needCondition,//この値になった時に
		int transitionFrame);//この時間をかけて移行する

	//transitionTimeを0.25秒固定
	void CreateTransition(
		string beforeAnimName,//このアニメーションから
		string afterAnimName,//このアニメーションに
		string conditionName,//このコンディションが
		BOOL needCondition);//この値になった時に
	

	//ループしないアニメーション用
	void CreateNotLoopAnimExitTransition(
		string beforeAnimName,//このアニメーションから
		string afterAnimName,//このアニメーションに
		float transitionTime);//この時間をかけて移行する

	void CreateNotLoopAnimExitTransition(
		string beforeAnimName,//このアニメーションから
		string afterAnimName,//このアニメーションに
		int transitionFrame);//この時間をかけて移行する

	//transitionTimeを0.25秒固定
	void CreateNotLoopAnimExitTransition(
		string beforeAnimName,//このアニメーションから
		string afterAnimName);//このアニメーションに



	void CreateCondition(string name, AnimParameter initValue);
	void SetCondition(string name, BOOL setValue);
	BOOL GetCondition(string name);
	BOOL GetCondition(int index);


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
protected:
	
	AnimationForceTransition* fTransition;

	void UpdateAnimation(MtxNode* node, GameObject* gameObject);

	AssetsManager* pAssetsManager;

	vector<AnimationNode*> AnimNodeArray;

	vector<pair<AnimParameter,string>> conditionArray;

	Animation* activeAnim;

	float timeCnt;
	int framenum;
	BOOL animation;

	float animSpeed;

	BOOL frameBlendMode;

	int defaultAnimIndex;

	float blendWeight;

	float angle;


	BOOL stop;


};

