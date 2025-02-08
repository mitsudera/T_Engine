//=============================================================================
//
// 当たり判定処理 [collision.h]
// Author : 
//
//=============================================================================
#pragma once
#include "Coreminimal.h"

class TerrainColliderComponent;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

float dotProduct(XMVECTOR* v1, XMVECTOR* v2);
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2);
BOOL RayCast(XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 pos0, XMFLOAT3 pos1, XMFLOAT3* hit, XMFLOAT3* normal);

BOOL CollisionPointSphere(XMFLOAT3 point, XMFLOAT3 center, float r);
BOOL CollisionPointCapsule(XMFLOAT3 point, XMFLOAT3 pos1, XMFLOAT3 pos2,float r);
BOOL CollisionPointBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size);
BOOL CollisionPointRotBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
BOOL CollisionPointTerrain(XMFLOAT3 point, TerrainColliderComponent* terrain);

BOOL CollisionLineSphere(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, float r);
BOOL CollisionLineCapsule(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 cp1, XMFLOAT3 cp2,float r);
BOOL CollisionLineBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size);
BOOL CollisionLineRotBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
BOOL CollisionLineTerrain(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT2 hw);


BOOL CollisionSphereSphere(XMFLOAT3 center1, float r1, XMFLOAT3 center2, float r2);
BOOL CollisionSphereCapsule(XMFLOAT3 center1, float r1, XMFLOAT3 cp1, XMFLOAT3 cp2,float r2);
BOOL CollisionSphereBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size);
BOOL CollisionSphereRotBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
BOOL CollisionSphereTerrain(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT2 hw);


BOOL CollisionCapsuleCapsule(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2,float r1, XMFLOAT3 cp2_1, XMFLOAT3 cp2_2 ,float r2);
BOOL CollisionCapsuleBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size);
BOOL CollisionCapsuleRotBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
BOOL CollisionCapsuleTerrain(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT2 hw);


BOOL CollisionBoxBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2);
BOOL CollisionBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2,XMVECTOR rotQton);
BOOL CollisionBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center, XMFLOAT2 hw);

BOOL CollisionRotBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton2);
FLOAT LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3);
FLOAT LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2);
BOOL CollisionRotBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton, XMFLOAT3 center, XMFLOAT2 hw);

XMFLOAT4 GetDepthPointSphere(XMFLOAT3 point, XMFLOAT3 center, float r);
XMFLOAT4 GetDepthPointCapsule(XMFLOAT3 point, XMFLOAT3 pos1, XMFLOAT3 pos2,float r);
XMFLOAT4 GetDepthPointBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size);
XMFLOAT4 GetDepthPointRotBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
XMFLOAT4 GetDepthPointTerrain(XMFLOAT3 point, TerrainColliderComponent* terrain);

XMFLOAT4 GetDepthLineSphere(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, float r);
XMFLOAT4 GetDepthLineCapsule(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 cp1, XMFLOAT3 cp2,float r);
XMFLOAT4 GetDepthLineBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size);
XMFLOAT4 GetDepthLineRotBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
XMFLOAT4 GetDepthLineTerrain(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT2 hw);


XMFLOAT4 GetDepthSphereSphere(XMFLOAT3 center1, float r1, XMFLOAT3 center2, float r2);
XMFLOAT4 GetDepthSphereCapsule(XMFLOAT3 center1, float r1, XMFLOAT3 cp1, XMFLOAT3 cp2,float r2);
XMFLOAT4 GetDepthSphereBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size);
XMFLOAT4 GetDepthSphereRotBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
XMFLOAT4 GetDepthSphereTerrain(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT2 hw);


XMFLOAT4 GetDepthCapsuleCapsule(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2,float r1, XMFLOAT3 cp2_1, XMFLOAT3 cp2_2 ,float r2);
XMFLOAT4 GetDepthCapsuleBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size);
XMFLOAT4 GetDepthCapsuleRotBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton);
XMFLOAT4 GetDepthCapsuleTerrain(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT2 hw);


XMFLOAT4 GetDepthBoxBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2);
XMFLOAT4 GetDepthBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2,XMVECTOR rotQton);
XMFLOAT4 GetDepthBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center, XMFLOAT2 hw);

XMFLOAT4 GetDepthRotBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton2);
XMFLOAT4 GetDepthRotBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton, XMFLOAT3 center, XMFLOAT2 hw);



void ClosestPtSegmentSegment(XMVECTOR p1, XMVECTOR d1, XMVECTOR p2, XMVECTOR d2, float& t1, float& t2, XMVECTOR& c1, XMVECTOR& c2);





