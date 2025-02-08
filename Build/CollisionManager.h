#pragma once
#include "Coreminimal.h"

class ColliderComponent;
class GameEngine;
class World;
struct ColliderPair
{
	ColliderComponent* collider1;
	ColliderComponent* collider2;

};

class CollisionManager
{
public:
	CollisionManager(World* world);
	~CollisionManager();

	void Update(void);

	void AddCollider(ColliderComponent* collider);

	void DeleteCllider(ColliderComponent* collider);


	BOOL CheckCillision(ColliderPair pair);

	XMFLOAT4 CheckPenetrationDepth(ColliderPair pair);

private:

	GameEngine* pGameEngine;
	World* pWorld;
	vector<ColliderComponent*> colliderList;
	vector<ColliderPair> pairList;

};

