#include "GameEngine.h"
#include "CollisionManager.h"
#include "collision.h"
#include "ColliderComponent.h"
#include "PointColliderComponent.h"
#include "SphereColliderComponent.h"
#include "LineColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "BoxColliderComponent.h"
#include "TerrainColliderComponent.h"
#include "RotBoxColliderComponent.h"
#include "World.h"
CollisionManager::CollisionManager(World* world)
{
	this->pWorld = world;
	this->pGameEngine = world->GetGameEngine();

}

CollisionManager::~CollisionManager()
{
}


void CollisionManager::Update(void)
{

	for (int i = 0; i < colliderList.size(); i++)
	{
		colliderList[i]->Clear();
		colliderList[i]->UpdateCenter();
	}


	for (int i = 0; i < pairList.size(); i++)
	{
		
		BOOL ans = CheckCillision(pairList[i]);




		if (ans==TRUE)
		{
			pairList[i].collider1->SetHitObject(pairList[i].collider2->GetGameObject());
			pairList[i].collider2->SetHitObject(pairList[i].collider1->GetGameObject());

			pairList[i].collider1->SetHitTag(pairList[i].collider2->GetTag(), ans);
			pairList[i].collider2->SetHitTag(pairList[i].collider1->GetTag(), ans);


			if (pairList[i].collider1->GetIsRigid() && pairList[i].collider2->GetIsRigid())
			{
				XMFLOAT4 dep = CheckPenetrationDepth(pairList[i]);

				pairList[i].collider1->SetRigidObject(pairList[i].collider2->GetGameObject(), dep);
			}

		}



	}

	
}

void CollisionManager::AddCollider(ColliderComponent* collider)
{

	for (int i = 0; i < colliderList.size(); i++)
	{
		ColliderPair pair;

		pair.collider1 = colliderList[i];
		pair.collider2 = collider;

		pairList.push_back(pair);
	}

	colliderList.push_back(collider);

}

void CollisionManager::DeleteCllider(ColliderComponent* collider)
{
	for (int i = 0; i < pairList.size(); i++)
	{
		if (pairList[i].collider1 == collider || pairList[i].collider2 == collider)
		{
			pairList.erase(pairList.begin()+i);
		}

	}

	for (int i = 0; i < colliderList.size(); i++)
	{
		if (colliderList[i] == collider)
		{
			colliderList.erase(colliderList.begin()+i);
		}

	}


}

BOOL CollisionManager::CheckCillision(ColliderPair pair)
{
	XMFLOAT3 pos1 = pair.collider1->GetCenter();
	XMFLOAT3 pos2 = pair.collider2->GetCenter();

	float check1 = pair.collider1->GetCheckRadius();
	float check2 = pair.collider2->GetCheckRadius();

	if (check1 < 0.0f || check2 < 0.0f)
	{

	}
	//ここで大まかに当たっているか判定
	else if (CollisionSphereSphere(pos1, check1, pos2, check2) == FALSE)
	{
		return FALSE;
	}


	//一旦terrainはシェイプに関わらずポイントだけで


	if (pair.collider1->GetShape() == ColliderComponent::ColliderComponent::ColliderComponent::Shape::Point)
	{
		PointColliderComponent* point= static_cast<PointColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionPointSphere(pos1,pos2,sphere->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionPointCapsule(pos1,capsule->GetStart(),capsule->GetEnd(),capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionPointBox(pos1, box->GetCenter(),box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return CollisionPointRotBox(pos1, box->GetCenter(),box->GetSize(),box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return CollisionPointTerrain(pos1, terrain);

		}

	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Line)
	{
		LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{

			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return FALSE;
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);

			return CollisionLineSphere(line->GetStart(), line->GetEnd(),pos2,sphere->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionLineCapsule(line->GetStart(), line->GetEnd(), capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionLineBox(line->GetStart(), line->GetEnd(), box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return CollisionLineRotBox(line->GetStart(), line->GetEnd(), box->GetCenter(), box->GetSize(),box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return CollisionPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Sphere)
	{
		SphereColliderComponent* sphere1 = static_cast<SphereColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			PointColliderComponent* point = static_cast<PointColliderComponent*>(pair.collider2);


			return CollisionPointSphere(pos2,pos1,sphere1->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);

			return CollisionLineSphere(line->GetStart(), line->GetEnd(), pos1, sphere1->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere2 = static_cast<SphereColliderComponent*>(pair.collider2);

			return CollisionSphereSphere(pos1, sphere1->GetCheckRadius(), pos2, sphere2->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionSphereCapsule(pos1, sphere1->GetCheckRadius(), capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionSphereBox(pos1, sphere1->GetCheckRadius(), box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return CollisionSphereRotBox(pos1, sphere1->GetCheckRadius(), box->GetCenter(), box->GetSize(),box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return CollisionPointTerrain(pos1, terrain);

		}



	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Capsule)
	{
	CapsuleColliderComponent* capsule1 = static_cast<CapsuleColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return CollisionPointCapsule(pos2, capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return CollisionLineCapsule(line->GetStart(), line->GetEnd(), capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionSphereCapsule(pos2, sphere->GetCheckRadius(), capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule2 = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionCapsuleCapsule(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(),capsule2->GetStart(), capsule2->GetEnd(), capsule2->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionCapsuleBox(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(), box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return CollisionCapsuleRotBox(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(), box->GetCenter(), box->GetSize(),box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return CollisionPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Box)
	{
	BoxColliderComponent* box1 = static_cast<BoxColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return CollisionPointBox(pos2, box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return CollisionLineBox(line->GetStart(), line->GetEnd(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionSphereBox(pos2, sphere->GetCheckRadius(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionCapsuleBox(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box2 = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionBoxBox(box1->GetCenter(), box1->GetSize(), box2->GetCenter(), box2->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box2 = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return CollisionBoxRotBox(box1->GetCenter(), box1->GetSize(), box2->GetCenter(), box2->GetSize(),box2->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return CollisionPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::RotBox)
	{
	RotBoxColliderComponent* box1 = static_cast<RotBoxColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return CollisionPointRotBox(pos2, box1->GetCenter(), box1->GetSize(),box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return CollisionLineRotBox(line->GetStart(), line->GetEnd(), box1->GetCenter(), box1->GetSize(),box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return CollisionSphereRotBox(pos2, sphere->GetCheckRadius(), box1->GetCenter(), box1->GetSize(),box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return CollisionCapsuleRotBox(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius(), box1->GetCenter(), box1->GetSize(),box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box2 = static_cast<BoxColliderComponent*>(pair.collider2);
			return CollisionBoxRotBox(box2->GetCenter(), box2->GetSize(), box1->GetCenter(), box1->GetSize(), box1->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box2 = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return CollisionRotBoxRotBox(box1->GetCenter(), box1->GetSize(),box1->GetRot(), box2->GetCenter(), box2->GetSize(),box2->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return CollisionPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Terrain)
	{
	TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return CollisionPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return CollisionPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			return CollisionPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			return CollisionPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			return CollisionPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			return CollisionPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			return FALSE;
		}


	}

	return FALSE;
}

XMFLOAT4 CollisionManager::CheckPenetrationDepth(ColliderPair pair)
{
	XMFLOAT3 pos1 = pair.collider1->GetCenter();
	XMFLOAT3 pos2 = pair.collider2->GetCenter();

	float check1 = pair.collider1->GetCheckRadius();
	float check2 = pair.collider2->GetCheckRadius();





	if (pair.collider1->GetShape() == ColliderComponent::ColliderComponent::ColliderComponent::Shape::Point)
	{
		PointColliderComponent* point = static_cast<PointColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return XMFLOAT4();
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return XMFLOAT4();
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return GetDepthPointSphere(pos1, pos2, sphere->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return GetDepthPointCapsule(pos1, capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return GetDepthPointBox(pos1, box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return GetDepthPointRotBox(pos1, box->GetCenter(), box->GetSize(), box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return GetDepthPointTerrain(pos1, terrain);

		}

	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Line)
	{
		LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{

			return XMFLOAT4();
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return XMFLOAT4();
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);

			return GetDepthLineSphere(line->GetStart(), line->GetEnd(), pos2, sphere->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return GetDepthLineCapsule(line->GetStart(), line->GetEnd(), capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return GetDepthLineBox(line->GetStart(), line->GetEnd(), box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return GetDepthLineRotBox(line->GetStart(), line->GetEnd(), box->GetCenter(), box->GetSize(), box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return GetDepthPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Sphere)
	{
		SphereColliderComponent* sphere1 = static_cast<SphereColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			PointColliderComponent* point = static_cast<PointColliderComponent*>(pair.collider2);


			return GetDepthPointSphere(pos2, pos1, sphere1->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);

			return GetDepthLineSphere(line->GetStart(), line->GetEnd(), pos1, sphere1->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			SphereColliderComponent* sphere2 = static_cast<SphereColliderComponent*>(pair.collider2);

			return GetDepthSphereSphere(pos1, sphere1->GetCheckRadius(), pos2, sphere2->GetCheckRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return GetDepthSphereCapsule(pos1, sphere1->GetCheckRadius(), capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return GetDepthSphereBox(pos1, sphere1->GetCheckRadius(), box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return GetDepthSphereRotBox(pos1, sphere1->GetCheckRadius(), box->GetCenter(), box->GetSize(), box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return GetDepthPointTerrain(pos1, terrain);

		}



	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Capsule)
	{
		CapsuleColliderComponent* capsule1 = static_cast<CapsuleColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return GetDepthPointCapsule(pos2, capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return GetDepthLineCapsule(line->GetStart(), line->GetEnd(), capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return GetDepthSphereCapsule(pos2, sphere->GetCheckRadius(), capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule2 = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return GetDepthCapsuleCapsule(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(), capsule2->GetStart(), capsule2->GetEnd(), capsule2->GetRadius());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box = static_cast<BoxColliderComponent*>(pair.collider2);
			return GetDepthCapsuleBox(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(), box->GetCenter(), box->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return GetDepthCapsuleRotBox(capsule1->GetStart(), capsule1->GetEnd(), capsule1->GetRadius(), box->GetCenter(), box->GetSize(), box->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return GetDepthPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Box)
	{
		BoxColliderComponent* box1 = static_cast<BoxColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return GetDepthPointBox(pos2, box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return GetDepthLineBox(line->GetStart(), line->GetEnd(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return GetDepthSphereBox(pos2, sphere->GetCheckRadius(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return GetDepthCapsuleBox(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius(), box1->GetCenter(), box1->GetSize());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box2 = static_cast<BoxColliderComponent*>(pair.collider2);
			return GetDepthBoxBox(box1->GetCenter(), box1->GetSize(), box2->GetCenter(), box2->GetSize());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box2 = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return GetDepthBoxRotBox(box1->GetCenter(), box1->GetSize(), box2->GetCenter(), box2->GetSize(), box2->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return GetDepthPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::RotBox)
	{
		RotBoxColliderComponent* box1 = static_cast<RotBoxColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return GetDepthPointRotBox(pos2, box1->GetCenter(), box1->GetSize(), box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			LineColliderComponent* line = static_cast<LineColliderComponent*>(pair.collider2);


			return GetDepthLineRotBox(line->GetStart(), line->GetEnd(), box1->GetCenter(), box1->GetSize(), box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{

			SphereColliderComponent* sphere = static_cast<SphereColliderComponent*>(pair.collider2);
			return GetDepthSphereRotBox(pos2, sphere->GetCheckRadius(), box1->GetCenter(), box1->GetSize(), box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			CapsuleColliderComponent* capsule = static_cast<CapsuleColliderComponent*>(pair.collider2);

			return GetDepthCapsuleRotBox(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius(), box1->GetCenter(), box1->GetSize(), box1->GetRot());
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			BoxColliderComponent* box2 = static_cast<BoxColliderComponent*>(pair.collider2);
			return GetDepthBoxRotBox(box2->GetCenter(), box2->GetSize(), box1->GetCenter(), box1->GetSize(), box1->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			RotBoxColliderComponent* box2 = static_cast<RotBoxColliderComponent*>(pair.collider2);
			return GetDepthRotBoxRotBox(box1->GetCenter(), box1->GetSize(), box1->GetRot(), box2->GetCenter(), box2->GetSize(), box2->GetRot());

		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider2);
			return GetDepthPointTerrain(pos1, terrain);

		}


	}
	else if (pair.collider1->GetShape() == ColliderComponent::Shape::Terrain)
	{
		TerrainColliderComponent* terrain = static_cast<TerrainColliderComponent*>(pair.collider1);

		if (pair.collider2->GetShape() == ColliderComponent::Shape::Point)
		{
			return GetDepthPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Line)
		{
			return GetDepthPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Sphere)
		{
			return GetDepthPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Capsule)
		{
			return GetDepthPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Box)
		{
			return GetDepthPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::RotBox)
		{
			return GetDepthPointTerrain(pos2, terrain);
		}
		else if (pair.collider2->GetShape() == ColliderComponent::Shape::Terrain)
		{
			return XMFLOAT4();
		}


	}

	return XMFLOAT4();
}

