//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : 
//
//=============================================================================
#include "collision.h"
#include "TerrainColliderComponent.h"
//=============================================================================
// ����(dot)
//=============================================================================
float dotProduct(XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// �_�C���N�g�w�ł́A�A�A
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
#endif

	return(ans);
}


//=============================================================================
// �O��(cross)
//=============================================================================
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// �_�C���N�g�w�ł́A�A�A
	* ret = XMVector3Cross(*v1, *v2);
#endif

}


//=============================================================================
// ���C�L���X�g
// p0, p1, p2�@�|���S���̂R���_
// pos0 �n�_
// pos1 �I�_
// hit�@��_�̕ԋp�p
// normal �@���x�N�g���̕ԋp�p
// �������Ă���ꍇ�ATRUE��Ԃ�
//=============================================================================
BOOL RayCast(XMFLOAT3 xp0, XMFLOAT3 xp1, XMFLOAT3 xp2, XMFLOAT3 xpos0, XMFLOAT3 xpos1, XMFLOAT3* hit, XMFLOAT3* normal)
{
	XMVECTOR	p0 = XMLoadFloat3(&xp0);
	XMVECTOR	p1 = XMLoadFloat3(&xp1);
	XMVECTOR	p2 = XMLoadFloat3(&xp2);
	XMVECTOR	pos0 = XMLoadFloat3(&xpos0);
	XMVECTOR	pos1 = XMLoadFloat3(&xpos1);

	XMVECTOR	nor;	// �|���S���̖@��
	XMVECTOR	vec1;
	XMVECTOR	vec2;
	float		d1, d2;

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�(���̏����͌Œ蕨�Ȃ�\��Init()�ōs���Ă����Ɨǂ�)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		nor = XMVector3Normalize(nor);		// �v�Z���₷���悤�ɖ@�����m�[�}���C�Y���Ă���(���̃x�N�g���̒������P�ɂ��Ă���)
		XMStoreFloat3(normal, nor);			// ���߂��@�������Ă���
	}

	// �|���S�����ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�i�s�p�Ȃ�{�A�݊p�Ȃ�[�A���p�Ȃ�O�j
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// ���߂��|���S���̖@���ƂQ�̃x�N�g���i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if (((d1 * d2) > 0.0f) || (d1 == 0 && d2 == 0))
		{
			// �������Ă���\���͖���
			return(FALSE);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		d1 = (float)fabs(d1);	// ��Βl�����߂Ă���
		d2 = (float)fabs(d2);	// ��Βl�����߂Ă���
		float a = d1 / (d1 + d2);							// ������

		XMVECTOR	vec3 = (1 - a) * vec1 + a * vec2;		// p0�����_�ւ̃x�N�g��
		XMVECTOR	p3 = p0 + vec3;							// ��_
		XMStoreFloat3(hit, p3);								// ���߂���_�����Ă���

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			XMVECTOR	v1 = p1 - p0;
			XMVECTOR	v2 = p2 - p1;
			XMVECTOR	v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			XMVECTOR	v4 = p3 - p1;
			XMVECTOR	v5 = p3 - p2;
			XMVECTOR	v6 = p3 - p0;

			// �O�ςŊe�ӂ̖@�������߂āA�|���S���̖@���Ƃ̓��ς��Ƃ��ĕ������`�F�b�N����
			XMVECTOR	n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�
		}
	}

	return(TRUE);	// �������Ă���I(hit�ɂ͓������Ă����_�������Ă���Bnormal�ɂ͖@���������Ă���)
}

BOOL CollisionPointSphere(XMFLOAT3 point, XMFLOAT3 center, float r)
{
	BOOL ans = FALSE;

	XMVECTOR p = XMLoadFloat3(&point);
	XMVECTOR cp = XMLoadFloat3(&center);
	XMVECTOR lv = p - cp;

	float len;
	XMStoreFloat(&len, XMVector3Length(lv));

	if (len<r)
	{
		ans = TRUE;
	}
	return ans;
}
BOOL CollisionPointBox(XMFLOAT3 point, XMMATRIX boxMatrix)
{
	return FALSE;
}

BOOL CollisionPointCapsule(XMFLOAT3 point, XMFLOAT3 pos1, XMFLOAT3 pos2,float r)
{
	BOOL ans = FALSE;

	XMVECTOR p = XMLoadFloat3(&point);

	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);

	XMVECTOR p1c = p - p1;
	XMVECTOR p2c = p - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));


	//�����̗��[���܂��͌���
	if (p1l < r || p2l < r)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1, p2, p);
	float length;
	XMStoreFloat(&length, lv);
	if (length < r)
	{
		return TRUE;
	}

	return ans;
}

BOOL CollisionPointBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size)
{
	// �{�b�N�X�̊e�ӂ̍ŏ�����эő���W���v�Z
	XMFLOAT3 min = XMFLOAT3(center.x - size.x / 2.0f, center.y - size.y / 2.0f, center.z - size.z / 2.0f);
	XMFLOAT3 max = XMFLOAT3(center.x + size.x / 2.0f, center.y + size.y / 2.0f, center.z + size.z / 2.0f);

	// �_���{�b�N�X���ɂ��邩�ǂ������`�F�b�N
	if (point.x >= min.x && point.x <= max.x &&
		point.y >= min.y && point.y <= max.y &&
		point.z >= min.z && point.z <= max.z)
	{
		return TRUE; // �_���{�b�N�X���ɂ���ꍇ
	}
	return FALSE; // �_���{�b�N�X���ɂȂ��ꍇ
}
BOOL CollisionPointRotBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// �_���x�N�g���ɕϊ�
	XMVECTOR pointVec = XMLoadFloat3(&point);

	// ���S�_���x�N�g���ɕϊ�
	XMVECTOR centerVec = XMLoadFloat3(&center);

	// �{�b�N�X�̃��[�J�����W�n�ɓ_���ړ�
	XMVECTOR localPointVec = XMVectorSubtract(pointVec, centerVec);

	// �{�b�N�X�̉�]���t�����ɓK�p�i�{�b�N�X�̃��[�J�����W�n�ɖ߂����߁j
	XMVECTOR invRotQton = XMQuaternionInverse(rotQton);
	localPointVec = XMVector3Rotate(localPointVec, invRotQton);

	// �_�̍��W�����o��
	XMFLOAT3 localPoint;
	XMStoreFloat3(&localPoint, localPointVec);

	// �{�b�N�X�̔��a�i���S����̋����j���v�Z
	XMFLOAT3 halfSize = XMFLOAT3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);

	// �_���{�b�N�X���ɂ��邩�ǂ����𔻒�
	return (fabs(localPoint.x) <= halfSize.x) &&
		(fabs(localPoint.y) <= halfSize.y) &&
		(fabs(localPoint.z) <= halfSize.z);
}

BOOL CollisionPointTerrain(XMFLOAT3 point,TerrainColliderComponent* terrain )
{

	XMFLOAT2 length;
	length.x = point.x - terrain->GetCenter().x;
	length.y = point.z - terrain->GetCenter().z;

	XMFLOAT2 terrainSize = terrain->GetSize();

	//�͈͓��ɂ��邩�H
	if (length.x > (terrainSize.x * 0.5) ||
		length.x < -(terrainSize.x * 0.5) ||
		length.y >(terrainSize.y * 0.5) ||
		length.y < -(terrainSize.y * 0.5))
	{

		return FALSE;

	}
	else
	{
		return TRUE;
	}
		
	//float height = terrain->GetHeight(point);

	//if (height > point.y)
	//{
	//	return TRUE;
	//}

	return FALSE;

}






//�����Ƌ��̓����蔻��
BOOL CollisionLineSphere(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 center ,float r)
{
	BOOL ans = FALSE;
	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);
	XMVECTOR cp = XMLoadFloat3(&center);

	XMVECTOR p1c = cp - p1;
	XMVECTOR p2c = cp - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));


	//�����̗��[���܂��͌���
	if (p1l < r || p2l < r)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1,p2,cp);
	float length;
	XMStoreFloat(&length, lv);
	if (length<r)
	{
		return TRUE;
	}

	return ans;

}

BOOL CollisionLineCapsule(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 cp1, XMFLOAT3 cp2, float r)
{
	XMVECTOR lp1vec = XMLoadFloat3(&lp1);
	XMVECTOR lp2vec = XMLoadFloat3(&lp2);

	XMVECTOR cp1vec = XMLoadFloat3(&cp1);
	XMVECTOR cp2vec = XMLoadFloat3(&cp2);

	// �����̕����x�N�g��
	XMVECTOR lineDir = XMVectorSubtract(lp2vec, lp1vec);
	XMVECTOR capsuleDir = XMVectorSubtract(cp2vec, cp1vec);

	// �����ƃJ�v�Z���̍ŋߐړ_�����߂�
	float t1, t2;
	XMVECTOR closestLinePoint, closestCapsulePoint;
	ClosestPtSegmentSegment(lp1vec, lineDir, cp1vec, capsuleDir, t1, t2, closestLinePoint, closestCapsulePoint);

	// �ŋߐړ_�Ԃ̋������v�Z
	XMVECTOR distVec = XMVectorSubtract(closestLinePoint, closestCapsulePoint);
	float distSq = XMVectorGetX(XMVector3LengthSq(distVec));

	// �J�v�Z���̔��a���l�����ďՓ˔���
	return distSq <= (r * r);
}

BOOL CollisionLineBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size)
{
	XMFLOAT3 boxMin(center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
	XMFLOAT3 boxMax(center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2);

	XMFLOAT3 direction(lp2.x - lp1.x, lp2.y - lp1.y, lp2.z - lp1.z);
	XMFLOAT3 invDirection(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);

	float t1 = (boxMin.x - lp1.x) * invDirection.x;
	float t2 = (boxMax.x - lp1.x) * invDirection.x;
	float t3 = (boxMin.y - lp1.y) * invDirection.y;
	float t4 = (boxMax.y - lp1.y) * invDirection.y;
	float t5 = (boxMin.z - lp1.z) * invDirection.z;
	float t6 = (boxMax.z - lp1.z) * invDirection.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	if (tmax < 0 || tmin > tmax)
		return FALSE;

	return TRUE;
}

BOOL CollisionLineRotBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	return 0;
}

BOOL CollisionLineTerrain(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT2 hw)
{
	// �n�`�͈̔͂��`
	XMFLOAT3 minPoint(center.x - hw.x / 2, center.y, center.z - hw.y / 2);
	XMFLOAT3 maxPoint(center.x + hw.x / 2, center.y, center.z + hw.y / 2);

	// �����̊e�_���͈͓����`�F�b�N
	auto isPointInRange = [&](const XMFLOAT3& p)
	{
		return (p.x >= minPoint.x && p.x <= maxPoint.x &&
			p.y >= minPoint.y && p.y <= maxPoint.y &&
			p.z >= minPoint.z && p.z <= maxPoint.z);
	};

	// �����̓_���͈͓��ɂ��邩���m�F
	if (isPointInRange(lp1) || isPointInRange(lp2))
	{
		return TRUE;
	}

	return FALSE;
}




BOOL CollisionSphereSphere(XMFLOAT3 center1, float r1, XMFLOAT3 center2, float r2)
{
	BOOL ans = FALSE;
	XMVECTOR p1 = XMLoadFloat3(&center1);
	XMVECTOR p2 = XMLoadFloat3(&center2);

	XMVECTOR dist = p1 - p2;

	float len;

	XMStoreFloat(&len, XMVector3Length(dist));

	if (len<r1+r2)
	{
		ans = TRUE;
	}

	return ans;
}

BOOL CollisionSphereCapsule(XMFLOAT3 center1, float r1, XMFLOAT3 cp1, XMFLOAT3 cp2, float r2)
{
	BOOL ans = FALSE;

	XMVECTOR p = XMLoadFloat3(&center1);

	XMVECTOR p1 = XMLoadFloat3(&cp1);
	XMVECTOR p2 = XMLoadFloat3(&cp2);

	XMVECTOR p1c = p - p1;
	XMVECTOR p2c = p - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));


	//�����̗��[���܂��͌���
	if (p1l < r1+r2 || p2l < r1+r2)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1, p2, p);
	float length;
	XMStoreFloat(&length, lv);
	if (length < r1+r2)
	{
		return TRUE;
	}

	return ans;
}

BOOL CollisionSphereBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size)
{
	// �{�b�N�X�͈̔͂��`
	XMFLOAT3 boxMin(center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
	XMFLOAT3 boxMax(center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2);

	// �{�b�N�X���ŋ��̒��S�ɍł��߂��_��������
	XMFLOAT3 closestPoint(
		max(boxMin.x, min(center1.x, boxMax.x)),
		max(boxMin.y, min(center1.y, boxMax.y)),
		max(boxMin.z, min(center1.z, boxMax.z))
	);

	// ���̒��S�ƃ{�b�N�X���ōł��߂��_�̋������v�Z
	XMFLOAT3 direction(center1.x - closestPoint.x, center1.y - closestPoint.y, center1.z - closestPoint.z);
	float distanceSq = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;

	// ���������̔��a�ȉ��ł���΁A�Փ˂��Ă���
	return distanceSq <= r1 * r1 ? TRUE : FALSE;
}

BOOL CollisionSphereRotBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// ���̒��S���x�N�g���ɕϊ�
	XMVECTOR sphereCenterVec = XMLoadFloat3(&center1);

	// �{�b�N�X�̒��S���x�N�g���ɕϊ�
	XMVECTOR boxCenterVec = XMLoadFloat3(&center);

	// �{�b�N�X�̃��[�J�����W�n�ɋ��̒��S���ړ�
	XMVECTOR localSphereCenterVec = XMVectorSubtract(sphereCenterVec, boxCenterVec);

	// �{�b�N�X�̉�]���t�����ɓK�p�i�{�b�N�X�̃��[�J�����W�n�ɖ߂����߁j
	XMVECTOR invRotQton = XMQuaternionInverse(rotQton);
	localSphereCenterVec = XMVector3Rotate(localSphereCenterVec, invRotQton);

	// ���̒��S�̍��W�����o��
	XMFLOAT3 localSphereCenter;
	XMStoreFloat3(&localSphereCenter, localSphereCenterVec);

	// �{�b�N�X�̔��a�i���S����̋����j���v�Z
	XMFLOAT3 halfSize = XMFLOAT3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);

	// �ŋߐړ_���v�Z
	XMFLOAT3 closestPoint;
	closestPoint.x = max(-halfSize.x, min(localSphereCenter.x, halfSize.x));
	closestPoint.y = max(-halfSize.y, min(localSphereCenter.y, halfSize.y));
	closestPoint.z = max(-halfSize.z, min(localSphereCenter.z, halfSize.z));

	// �ŋߐړ_�Ƌ��̒��S�̋������v�Z
	XMVECTOR closestPointVec = XMLoadFloat3(&closestPoint);
	XMVECTOR diffVec = XMVectorSubtract(localSphereCenterVec, closestPointVec);
	float distanceSquared = XMVectorGetX(XMVector3LengthSq(diffVec));

	// ���������̔��a�ȉ��ł���ΏՓ�
	return distanceSquared <= r1 * r1;
}

BOOL CollisionSphereTerrain(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT2 hw)
{
	// �n�`�͈̔͂��`
	XMFLOAT3 minPoint(center.x - hw.x / 2, center.y, center.z - hw.y / 2);
	XMFLOAT3 maxPoint(center.x + hw.x / 2, center.y, center.z + hw.y / 2);

	// �n�`�̊e�ʂɂ��ċ��̒��S�ɍł��߂��_��������
	XMFLOAT3 closestPoint(
		max(minPoint.x, min(center1.x, maxPoint.x)),
		center.y, // �����͕ς��Ȃ��̂�center.y���g�p
		max(minPoint.z, min(center1.z, maxPoint.z))
	);

	// ���̒��S�ƒn�`���ōł��߂��_�̋������v�Z
	XMFLOAT3 direction(center1.x - closestPoint.x, center1.y - closestPoint.y, center1.z - closestPoint.z);
	float distanceSq = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;

	// ���������̔��a�ȉ��ł���΁A�͈͓��ɂ���
	return distanceSq <= r1 * r1 ? TRUE : FALSE;
}

BOOL CollisionCapsuleCapsule(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 cp2_1, XMFLOAT3 cp2_2, float r2)
{
	XMVECTOR p1_1 = XMLoadFloat3(&cp1_1);
	XMVECTOR p1_2 = XMLoadFloat3(&cp1_2);
	XMVECTOR p2_1 = XMLoadFloat3(&cp2_1);
	XMVECTOR p2_2 = XMLoadFloat3(&cp2_2);

	XMVECTOR d1 = XMVectorSubtract(p1_2, p1_1);
	XMVECTOR d2 = XMVectorSubtract(p2_2, p2_1);

	float t1, t2;
	XMVECTOR c1, c2;

	// �ŋߐړ_�����߂�
	ClosestPtSegmentSegment(p1_1, d1, p2_1, d2, t1, t2, c1, c2);

	// �J�v�Z���̔��a���l�����ďՓ˔���
	XMVECTOR distVec = XMVectorSubtract(c1, c2);
	float distSq = XMVectorGetX(XMVector3LengthSq(distVec));
	float radiusSum = r1 + r2;

	return distSq <= (radiusSum * radiusSum);
}

BOOL CollisionCapsuleBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size)
{
	XMFLOAT3 boxMin(center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
	XMFLOAT3 boxMax(center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2);

	auto squaredDistancePointAABB = [](const XMFLOAT3& p, const XMFLOAT3& bMin, const XMFLOAT3& bMax)
	{
		float sqDist = 0.0f;
		if (p.x < bMin.x) sqDist += (bMin.x - p.x) * (bMin.x - p.x);
		if (p.x > bMax.x) sqDist += (p.x - bMax.x) * (p.x - bMax.x);
		if (p.y < bMin.y) sqDist += (bMin.y - p.y) * (bMin.y - p.y);
		if (p.y > bMax.y) sqDist += (p.y - bMax.y) * (p.y - bMax.y);
		if (p.z < bMin.z) sqDist += (bMin.z - p.z) * (bMin.z - p.z);
		if (p.z > bMax.z) sqDist += (p.z - bMax.z) * (p.z - bMax.z);
		return sqDist;
	};

	if (squaredDistancePointAABB(cp1_1, boxMin, boxMax) <= r1 * r1 ||
		squaredDistancePointAABB(cp1_2, boxMin, boxMax) <= r1 * r1)
		return TRUE;

	XMFLOAT3 d(cp1_2.x - cp1_1.x, cp1_2.y - cp1_1.y, cp1_2.z - cp1_1.z);
	float lengthSq = d.x * d.x + d.y * d.y + d.z * d.z;
	XMFLOAT3 closestPt = cp1_1;

	auto segmentClosestPtToPoint = [&](const XMFLOAT3& p, XMFLOAT3& closest)
	{
		XMFLOAT3 pToCp1(p.x - cp1_1.x, p.y - cp1_1.y, p.z - cp1_1.z);
		float t = (pToCp1.x * d.x + pToCp1.y * d.y + pToCp1.z * d.z) / lengthSq;
		t = max(0.0f, min(1.0f, t));
		closest.x = cp1_1.x + t * d.x;
		closest.y = cp1_1.y + t * d.y;
		closest.z = cp1_1.z + t * d.z;
	};

	segmentClosestPtToPoint(center, closestPt);

	if (squaredDistancePointAABB(closestPt, boxMin, boxMax) <= r1 * r1)
		return TRUE;

	return FALSE;
}

BOOL CollisionCapsuleRotBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// �J�v�Z���̒[�_���x�N�g���ɕϊ�
	XMVECTOR capsuleP1 = XMLoadFloat3(&cp1_1);
	XMVECTOR capsuleP2 = XMLoadFloat3(&cp1_2);

	// �{�b�N�X�̒��S���x�N�g���ɕϊ�
	XMVECTOR boxCenterVec = XMLoadFloat3(&center);

	// �{�b�N�X�̃��[�J�����W�n�ɃJ�v�Z���̒[�_���ړ�
	XMVECTOR localP1 = XMVectorSubtract(capsuleP1, boxCenterVec);
	XMVECTOR localP2 = XMVectorSubtract(capsuleP2, boxCenterVec);

	// �{�b�N�X�̉�]���t�����ɓK�p�i�{�b�N�X�̃��[�J�����W�n�ɖ߂����߁j
	XMVECTOR invRotQton = XMQuaternionInverse(rotQton);
	localP1 = XMVector3Rotate(localP1, invRotQton);
	localP2 = XMVector3Rotate(localP2, invRotQton);

	// �J�v�Z���̒[�_�̍��W�����o��
	XMFLOAT3 localCapsuleP1, localCapsuleP2;
	XMStoreFloat3(&localCapsuleP1, localP1);
	XMStoreFloat3(&localCapsuleP2, localP2);

	// �{�b�N�X�̔��a�i���S����̋����j���v�Z
	XMFLOAT3 halfSize = XMFLOAT3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);

	// �ŋߐړ_���v�Z����֐�
	auto ClosestPointOnAABB = [](XMFLOAT3 point, XMFLOAT3 min, XMFLOAT3 max) -> XMFLOAT3 {
		XMFLOAT3 closestPoint;
		closestPoint.x = max(min.x, min(point.x, max.x));
		closestPoint.y = max(min.y, min(point.y, max.y));
		closestPoint.z = max(min.z, min(point.z, max.z));
		return closestPoint;
	};

	// �{�b�N�X�̋��E
	XMFLOAT3 boxMin = XMFLOAT3(-halfSize.x, -halfSize.y, -halfSize.z);
	XMFLOAT3 boxMax = XMFLOAT3(halfSize.x, halfSize.y, halfSize.z);

	// �J�v�Z���̐�����̍ŋߐړ_�����߂�
	XMVECTOR v = XMVectorSubtract(localP2, localP1);
	XMVECTOR w = XMVectorSubtract(localP1, XMLoadFloat3(&boxMin));
	float t = max(0.0f, min(XMVectorGetX(XMVector3Dot(v, w)) / XMVectorGetX(XMVector3Dot(v, v)), 1.0f));
	XMVECTOR closestPoint = XMVectorAdd(localP1, XMVectorScale(v, t));

	// �ŋߐړ_���{�b�N�X�̋��E�ɐ���
	XMFLOAT3 closestPointLocal;
	XMStoreFloat3(&closestPointLocal, closestPoint);
	XMFLOAT3 closestOnBox = ClosestPointOnAABB(closestPointLocal, boxMin, boxMax);

	// �ŋߐړ_�ƃJ�v�Z���̒[�_�̋������v�Z
	XMVECTOR closestOnBoxVec = XMLoadFloat3(&closestOnBox);
	XMVECTOR diffVec = XMVectorSubtract(closestPoint, closestOnBoxVec);
	float distanceSquared = XMVectorGetX(XMVector3LengthSq(diffVec));

	// �������J�v�Z���̔��a�ȉ��ł���ΏՓ�
	return distanceSquared <= r1 * r1;
}


BOOL CollisionCapsuleTerrain(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT2 hw)
{
	// �n�`�͈̔͂��` (xz���ʂ̂�)
	XMFLOAT2 minPoint(center.x - hw.x / 2, center.z - hw.y / 2);
	XMFLOAT2 maxPoint(center.x + hw.x / 2, center.z + hw.y / 2);

	auto squaredDistancePointAABB2D = [](const XMFLOAT2& p, const XMFLOAT2& bMin, const XMFLOAT2& bMax)
	{
		float sqDist = 0.0f;
		if (p.x < bMin.x) sqDist += (bMin.x - p.x) * (bMin.x - p.x);
		if (p.x > bMax.x) sqDist += (p.x - bMax.x) * (p.x - bMax.x);
		if (p.y < bMin.y) sqDist += (bMin.y - p.y) * (bMin.y - p.y);
		if (p.y > bMax.y) sqDist += (p.y - bMax.y) * (p.y - bMax.y);
		return sqDist;
	};

	// �J�v�Z���̗��[���n�`�͈͓̔����`�F�b�N
	XMFLOAT2 cp1_1_2D(cp1_1.x, cp1_1.z);
	XMFLOAT2 cp1_2_2D(cp1_2.x, cp1_2.z);
	if (squaredDistancePointAABB2D(cp1_1_2D, minPoint, maxPoint) <= r1 * r1 ||
		squaredDistancePointAABB2D(cp1_2_2D, minPoint, maxPoint) <= r1 * r1)
	{
		return TRUE;
	}

	// �J�v�Z���̐����̊e�_���n�`�͈͓̔����`�F�b�N
	XMFLOAT2 d(cp1_2_2D.x - cp1_1_2D.x, cp1_2_2D.y - cp1_1_2D.y);
	float lengthSq = d.x * d.x + d.y * d.y;
	XMFLOAT2 closestPt = cp1_1_2D;

	auto segmentClosestPtToPoint2D = [&](const XMFLOAT2& p, XMFLOAT2& closest)
	{
		XMFLOAT2 pToCp1(p.x - cp1_1_2D.x, p.y - cp1_1_2D.y);
		float t = (pToCp1.x * d.x + pToCp1.y * d.y) / lengthSq;
		t = max(0.0f, min(1.0f, t));
		closest.x = cp1_1_2D.x + t * d.x;
		closest.y = cp1_1_2D.y + t * d.y;
	};

	segmentClosestPtToPoint2D(XMFLOAT2(center.x, center.z), closestPt);

	if (squaredDistancePointAABB2D(closestPt, minPoint, maxPoint) <= r1 * r1)
		return TRUE;

	return FALSE;
}

BOOL CollisionBoxBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2)
{
	// �{�b�N�X1�̍ŏ�����эő���W���v�Z
	XMFLOAT3 min1 = XMFLOAT3(center1.x - size1.x / 2.0f, center1.y - size1.y / 2.0f, center1.z - size1.z / 2.0f);
	XMFLOAT3 max1 = XMFLOAT3(center1.x + size1.x / 2.0f, center1.y + size1.y / 2.0f, center1.z + size1.z / 2.0f);

	// �{�b�N�X2�̍ŏ�����эő���W���v�Z
	XMFLOAT3 min2 = XMFLOAT3(center2.x - size2.x / 2.0f, center2.y - size2.y / 2.0f, center2.z - size2.z / 2.0f);
	XMFLOAT3 max2 = XMFLOAT3(center2.x + size2.x / 2.0f, center2.y + size2.y / 2.0f, center2.z + size2.z / 2.0f);

	// �e���ɑ΂��ďd�Ȃ���`�F�b�N
	if (min1.x > max2.x || max1.x < min2.x) return FALSE;
	if (min1.y > max2.y || max1.y < min2.y) return FALSE;
	if (min1.z > max2.z || max1.z < min2.z) return FALSE;

	return TRUE; // �{�b�N�X���d�Ȃ��Ă���ꍇ
}

BOOL CollisionBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton2)
{
	return CollisionRotBoxRotBox(center1, size1, XMQuaternionIdentity(), center2, size2, rotQton2);
}


BOOL CollisionBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center, XMFLOAT2 hw)
{
	// �n�`�͈̔͂��` (xz���ʂ̂�)
	XMFLOAT2 minPoint(center.x - hw.x / 2, center.z - hw.y / 2);
	XMFLOAT2 maxPoint(center.x + hw.x / 2, center.z + hw.y / 2);

	// �{�b�N�X�͈̔͂��` (xz���ʂ̂�)
	XMFLOAT2 boxMin(center1.x - size1.x / 2, center1.z - size1.z / 2);
	XMFLOAT2 boxMax(center1.x + size1.x / 2, center1.z + size1.z / 2);

	// �{�b�N�X�ƒn�`�͈̔͂��������邩���`�F�b�N
	if (boxMin.x <= maxPoint.x && boxMax.x >= minPoint.x &&
		boxMin.y <= maxPoint.y && boxMax.y >= minPoint.y)
	{
		return TRUE;
	}

	return FALSE;
}



BOOL CollisionRotBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton2)
{
	XMVECTOR c1 = XMLoadFloat3(&center1);
	XMVECTOR c2 = XMLoadFloat3(&center2);
	XMVECTOR halfSize1 = XMLoadFloat3(&XMFLOAT3(size1.x * 0.5f, size1.y * 0.5f, size1.z * 0.5f));
	XMVECTOR halfSize2 = XMLoadFloat3(&XMFLOAT3(size2.x * 0.5f, size2.y * 0.5f, size2.z * 0.5f));
	XMVECTOR interval = c1 - c2;

	

	// �e�{�b�N�X�̎��x�N�g�����v�Z
	XMVECTOR axesA[3] = {
		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQton1),
		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQton1),
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQton1)
	};

	XMVECTOR axesB[3] = {
		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQton2),
		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQton2),
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQton2)
	};

	XMVECTOR ae[3] = {
		axesA[0]*halfSize1.m128_f32[0],
		axesA[1]*halfSize1.m128_f32[1],
		axesA[2]*halfSize1.m128_f32[2],
	};

	XMVECTOR be[3] = {
		axesB[0]*halfSize2.m128_f32[0],
		axesB[1]*halfSize2.m128_f32[1],
		axesB[2]*halfSize2.m128_f32[2],
	};



	// ������ : ae[0]
	FLOAT rA = XMVectorGetX(XMVector3Length(ae[0]));
	FLOAT rB = LenSegOnSeparateAxis(&axesA[0], &be[0], &be[1], &be[2]);
	FLOAT L = fabs(XMVectorGetX(XMVector3Dot(interval, axesA[0])));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	 // ������ : ae[1]
	rA = XMVectorGetX(XMVector3Length(ae[1]));
	rB = LenSegOnSeparateAxis(&axesA[1], &be[0], &be[1], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, axesA[1])));
	if (L > rA + rB)
		return false;

	// ������ : ae[2]
	rA = XMVectorGetX(XMVector3Length(ae[2]));
	rB = LenSegOnSeparateAxis(&axesA[2], &be[0], &be[1], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, axesA[2])));
	if (L > rA + rB)
		return false;

	// ������ : be[0]
	rA = LenSegOnSeparateAxis(&axesB[0], &ae[0], &ae[1], &ae[2]);
	rB = XMVectorGetX(XMVector3Length(be[0]));
	L = fabs(XMVectorGetX(XMVector3Dot(interval, axesB[0])));
	if (L > rA + rB)
		return false;

	// ������ : be[1]
	rA = LenSegOnSeparateAxis(&axesB[1], &ae[0], &ae[1], &ae[2]);
	rB = XMVectorGetX(XMVector3Length(be[1]));
	L = fabs(XMVectorGetX(XMVector3Dot(interval, axesB[1])));
	if (L > rA + rB)
		return false;

	// ������ : be[2]
	rA = LenSegOnSeparateAxis(&axesB[2], &ae[0], &ae[1], &ae[2]);
	rB = XMVectorGetX(XMVector3Length(be[2]));
	L = fabs(XMVectorGetX(XMVector3Dot(interval, axesB[2])));
	if (L > rA + rB)
		return false;

	// ������ : C11
	XMVECTOR Cross;
	Cross=XMVector3Cross(axesA[0], axesB[0]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[1], &ae[2]);
	rB = LenSegOnSeparateAxis(&Cross, &be[1], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross=XMVector3Cross(axesA[0], axesB[1]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[1], &ae[2]);
	rB = LenSegOnSeparateAxis(&Cross, &be[0], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross=XMVector3Cross(axesA[0], axesB[2]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[1], &ae[2]);
	rB = LenSegOnSeparateAxis(&Cross, &be[0], &be[1]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross=XMVector3Cross(axesA[1], axesB[0]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[0], &ae[2]);
	rB = LenSegOnSeparateAxis(&Cross, &be[1], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross=XMVector3Cross(axesA[1], axesB[1]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[0], &ae[2]);
	rB = LenSegOnSeparateAxis(&Cross, &be[0], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross=XMVector3Cross(axesA[1], axesB[2]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[0], &ae[2]);
	rB = LenSegOnSeparateAxis(&Cross, &be[0], &be[1]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross=XMVector3Cross(axesA[2], axesB[0]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[0], &ae[1]);
	rB = LenSegOnSeparateAxis(&Cross, &be[1], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross=XMVector3Cross(axesA[2], axesB[1]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[0], &ae[1]);
	rB = LenSegOnSeparateAxis(&Cross, &be[0], &be[2]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross=XMVector3Cross(axesA[2], axesB[2]);
	rA = LenSegOnSeparateAxis(&Cross, &ae[0], &ae[1]);
	rB = LenSegOnSeparateAxis(&Cross, &be[0], &be[1]);
	L = fabs(XMVectorGetX(XMVector3Dot(interval, Cross)));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
FLOAT LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��

	FLOAT r1 = fabs(XMVectorGetX(XMVector3Dot(*Sep, *e1)));
	FLOAT r2 = fabs(XMVectorGetX(XMVector3Dot(*Sep, *e2)));
	FLOAT r3 = fabs(XMVectorGetX(XMVector3Dot(*Sep, *e3)));
	return r1 + r2 + r3;
}

FLOAT LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��

	FLOAT r1 = fabs(XMVectorGetX(XMVector3Dot(*Sep, *e1)));
	FLOAT r2 = fabs(XMVectorGetX(XMVector3Dot(*Sep, *e2)));
	FLOAT r3 = 0;
	return r1 + r2 + r3;
}

BOOL CollisionRotBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton, XMFLOAT3 center, XMFLOAT2 hw)
{
	return 0;
}




XMFLOAT4 GetDepthPointSphere(XMFLOAT3 point, XMFLOAT3 center, float r)
{
	XMVECTOR p = XMLoadFloat3(&point);
	XMVECTOR c = XMLoadFloat3(&center);
	XMVECTOR diff = XMVectorSubtract(p, c);
	float distance = XMVectorGetX(XMVector3Length(diff));

	XMFLOAT4 result;
	if (distance < r)
	{
		// ���������a�ȓ��Ȃ�ђʐ[�x�Ɩ@�����v�Z
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), XMVector3Normalize(diff));
		result.w = r - distance;
	}
	else
	{
		// ���a�O�Ȃ�Փ˂��Ă��Ȃ��̂Ŗ@�����[���x�N�g���ɂ��Đ[�x��0
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthPointCapsule(XMFLOAT3 point, XMFLOAT3 pos1, XMFLOAT3 pos2, float r)
{
	XMVECTOR p = XMLoadFloat3(&point);
	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);
	XMVECTOR d = XMVectorSubtract(p2, p1);
	XMVECTOR pp1 = XMVectorSubtract(p, p1);
	float t = XMVectorGetX(XMVector3Dot(pp1, d)) / XMVectorGetX(XMVector3Dot(d, d));
	t = max(0.0f, min(1.0f, t));
	XMVECTOR closestPoint = XMVectorAdd(p1, XMVectorScale(d, t));
	XMVECTOR diff = XMVectorSubtract(p, closestPoint);
	float distance = XMVectorGetX(XMVector3Length(diff));

	XMFLOAT4 result;
	if (distance < r)
	{
		// ���������a�ȓ��Ȃ�ђʐ[�x�Ɩ@�����v�Z
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), XMVector3Normalize(diff));
		result.w = r - distance;
	}
	else
	{
		// ���a�O�Ȃ�Փ˂��Ă��Ȃ��̂Ŗ@�����[���x�N�g���ɂ��Đ[�x��0
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthPointBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size)
{
	XMFLOAT3 halfSize = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };
	XMFLOAT3 min = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
	XMFLOAT3 max = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z };

	float dx = max(min.x - point.x, 0.0f);
	dx = max(dx, point.x - max.x);
	float dy = max(min.y - point.y, 0.0f);
	dy = max(dy, point.y - max.y);
	float dz = max(min.z - point.z, 0.0f);
	dz = max(dz, point.z - max.z);

	XMFLOAT4 result;
	if (dx > 0 || dy > 0 || dz > 0)
	{
		float distance = sqrt(dx * dx + dy * dy + dz * dz);
		result.x = dx > 0 ? (point.x < center.x ? -1.0f : 1.0f) : 0.0f;
		result.y = dy > 0 ? (point.y < center.y ? -1.0f : 1.0f) : 0.0f;
		result.z = dz > 0 ? (point.z < center.z ? -1.0f : 1.0f) : 0.0f;
		result.w = distance;
	}
	else
	{
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthPointRotBox(XMFLOAT3 point, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// �_����]�̋t�N�I�[�^�j�I���ŉ�]�����ă{�b�N�X�̃��[�J����Ԃɕϊ�
	XMVECTOR qInv = XMQuaternionInverse(rotQton);
	XMVECTOR p = XMVector3Rotate(XMLoadFloat3(&point), qInv);
	XMVECTOR c = XMVector3Rotate(XMLoadFloat3(&center), qInv);

	XMFLOAT3 localPoint;
	XMStoreFloat3(&localPoint, XMVectorSubtract(p, c));

	return GetDepthPointBox(localPoint, XMFLOAT3(0, 0, 0), size);
}

XMFLOAT4 GetDepthPointTerrain(XMFLOAT3 point, TerrainColliderComponent* terrain)
{
	return XMFLOAT4();
}

XMFLOAT4 GetDepthLineSphere(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, float r)
{
	XMVECTOR p1 = XMLoadFloat3(&lp1);
	XMVECTOR p2 = XMLoadFloat3(&lp2);
	XMVECTOR c = XMLoadFloat3(&center);

	XMVECTOR d = XMVectorSubtract(p2, p1);
	XMVECTOR f = XMVectorSubtract(p1, c);

	float a = XMVectorGetX(XMVector3Dot(d, d));
	float b = 2.0f * XMVectorGetX(XMVector3Dot(f, d));
	float c_val = XMVectorGetX(XMVector3Dot(f, f)) - r * r;

	float discriminant = b * b - 4.0f * a * c_val;
	XMFLOAT4 result;

	if (discriminant < 0.0f)
	{
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		return result;
	}

	discriminant = sqrt(discriminant);
	float t1 = (-b - discriminant) / (2.0f * a);
	float t2 = (-b + discriminant) / (2.0f * a);

	if ((t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f))
	{
		float distance = fmax(t1, t2) * XMVectorGetX(XMVector3Length(d));
		float penetrationDepth = r - distance;

		if (penetrationDepth > 0.0f)
		{
			XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), XMVector3Normalize(XMVectorSubtract(c, (t1 < t2 ? XMVectorAdd(p1, t1 * d) : XMVectorAdd(p1, t2 * d)))));
			result.w = penetrationDepth;
		}
		else
		{
			result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		return result;
	}

	result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	return result;
}

XMFLOAT4 GetDepthLineCapsule(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 cp1, XMFLOAT3 cp2, float r)
{
	XMVECTOR p1 = XMLoadFloat3(&lp1);
	XMVECTOR p2 = XMLoadFloat3(&lp2);
	XMVECTOR c1 = XMLoadFloat3(&cp1);
	XMVECTOR c2 = XMLoadFloat3(&cp2);

	XMVECTOR segment = c2 - c1;
	XMVECTOR lineDir = XMVector3Normalize(p2 - p1);
	XMVECTOR toStart = p1 - c1;

	float segmentLen = XMVectorGetX(XMVector3Length(segment));
	XMVECTOR segmentNorm = XMVector3Normalize(segment);

	float t = max(0.0f, min(segmentLen, XMVectorGetX(XMVector3Dot(toStart, segmentNorm))));
	XMVECTOR closestPoint = c1 + t * segmentNorm;

	return GetDepthLineSphere(lp1, lp2, XMFLOAT3(XMVectorGetX(closestPoint), XMVectorGetY(closestPoint), XMVectorGetZ(closestPoint)), r);
}

XMFLOAT4 GetDepthLineBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size)
{
	XMFLOAT3 halfSize = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };
	XMFLOAT3 min = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
	XMFLOAT3 max = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z };

	// ���������x�N�g��
	XMVECTOR p1 = XMLoadFloat3(&lp1);
	XMVECTOR p2 = XMLoadFloat3(&lp2);
	XMVECTOR dir = XMVectorSubtract(p2, p1);

	XMFLOAT3 dirFloat;
	XMStoreFloat3(&dirFloat, dir);

	float tmin = 0.0f;
	float tmax = 1.0f;

	// �e���ɑ΂��Čv�Z
	for (int i = 0; i < 3; ++i)
	{
		float t1 = ((&min.x)[i] - (&lp1.x)[i]) / (&dirFloat.x)[i];
		float t2 = ((&max.x)[i] - (&lp1.x)[i]) / (&dirFloat.x)[i];

		if (t1 > t2) swap(t1, t2);

		tmin = max(tmin, t1);
		tmax = min(tmax, t2);

		if (tmin > tmax)
		{
			return XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); // �Փ˂Ȃ�
		}
	}

	// �Փ˖@���̌v�Z
	XMFLOAT3 hitPoint = { lp1.x + tmin * dirFloat.x, lp1.y + tmin * dirFloat.y, lp1.z + tmin * dirFloat.z };
	XMFLOAT3 normal = { 0.0f, 0.0f, 0.0f };

	if (fabs(hitPoint.x - min.x) < 1e-5f) normal = { -1.0f, 0.0f, 0.0f };
	else if (fabs(hitPoint.x - max.x) < 1e-5f) normal = { 1.0f, 0.0f, 0.0f };
	else if (fabs(hitPoint.y - min.y) < 1e-5f) normal = { 0.0f, -1.0f, 0.0f };
	else if (fabs(hitPoint.y - max.y) < 1e-5f) normal = { 0.0f, 1.0f, 0.0f };
	else if (fabs(hitPoint.z - min.z) < 1e-5f) normal = { 0.0f, 0.0f, -1.0f };
	else if (fabs(hitPoint.z - max.z) < 1e-5f) normal = { 0.0f, 0.0f, 1.0f };

	// �Փ˖@���Ɗђʐ[�x��Ԃ�
	return XMFLOAT4(normal.x, normal.y, normal.z, tmax - tmin);
}

XMFLOAT4 GetDepthLineRotBox(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// �_����]�̋t�N�I�[�^�j�I���ŉ�]�����ă{�b�N�X�̃��[�J����Ԃɕϊ�
	XMVECTOR qInv = XMQuaternionInverse(rotQton);
	XMVECTOR p1 = XMVector3Rotate(XMLoadFloat3(&lp1), qInv);
	XMVECTOR p2 = XMVector3Rotate(XMLoadFloat3(&lp2), qInv);
	XMVECTOR c = XMVector3Rotate(XMLoadFloat3(&center), qInv);

	XMFLOAT3 localLp1;
	XMFLOAT3 localLp2;
	XMFLOAT3 localCenter;

	XMStoreFloat3(&localLp1, p1);
	XMStoreFloat3(&localLp2, p2);
	XMStoreFloat3(&localCenter, c);

	return GetDepthLineBox(localLp1, localLp2, localCenter, size);
}

XMFLOAT4 GetDepthLineTerrain(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, XMFLOAT2 hw)
{
	return XMFLOAT4();
}

XMFLOAT4 GetDepthSphereSphere(XMFLOAT3 center1, float r1, XMFLOAT3 center2, float r2)
{
	XMVECTOR c1 = XMLoadFloat3(&center1);
	XMVECTOR c2 = XMLoadFloat3(&center2);
	XMVECTOR diff = XMVectorSubtract(c1, c2);
	float distance = XMVectorGetX(XMVector3Length(diff));
	float penetrationDepth = (r1 + r2) - distance;

	XMFLOAT4 result;
	// ���������a�̍��v�ȓ��Ȃ�ђʐ[�x�Ɩ@�����v�Z
	if (penetrationDepth > 0.0f)
	{
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), XMVector3Normalize(diff));
		result.w = penetrationDepth;
	}
	else
	{
		// �d�Ȃ肪�Ȃ��ꍇ�͊ђʐ[�x��0
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthSphereCapsule(XMFLOAT3 center1, float r1, XMFLOAT3 cp1, XMFLOAT3 cp2, float r2)
{
	XMVECTOR c1 = XMLoadFloat3(&center1);
	XMVECTOR p1 = XMLoadFloat3(&cp1);
	XMVECTOR p2 = XMLoadFloat3(&cp2);

	// �J�v�Z���̐�����̍ŋߐړ_�����߂�
	XMVECTOR d = XMVectorSubtract(p2, p1);
	XMVECTOR f = XMVectorSubtract(c1, p1);
	float t = XMVectorGetX(XMVector3Dot(f, d)) / XMVectorGetX(XMVector3Dot(d, d));
	t = max(0.0f, min(1.0f, t));
	XMVECTOR closestPoint = XMVectorAdd(p1, t * d);

	// ���ƍŋߐړ_�̋������v�Z
	XMVECTOR diff = XMVectorSubtract(c1, closestPoint);
	float distance = XMVectorGetX(XMVector3Length(diff));
	float penetrationDepth = (r1 + r2) - distance;

	XMFLOAT4 result;
	// ���������a�̍��v�ȓ��Ȃ�ђʐ[�x�Ɩ@�����v�Z
	if (penetrationDepth > 0.0f)
	{
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), XMVector3Normalize(diff));
		result.w = penetrationDepth;
	}
	else
	{
		// �d�Ȃ肪�Ȃ��ꍇ�͊ђʐ[�x��0
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthSphereBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size)
{
	XMFLOAT3 halfSize = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };
	XMFLOAT3 min = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
	XMFLOAT3 max = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z };

	float dx = max(min.x - center1.x, max(0.0f, center1.x - max.x));
	float dy = max(min.y - center1.y, max(0.0f, center1.y - max.y));
	float dz = max(min.z - center1.z, max(0.0f, center1.z - max.z));

	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	float penetrationDepth = r1 - distance;

	XMFLOAT4 result;
	// �����{�b�N�X���ɓ����Ă���ꍇ
	if (penetrationDepth > 0.0f)
	{
		result.w = penetrationDepth;
		if (dx > dy && dx > dz)
		{
			result.x = center1.x < center.x ? -1.0f : 1.0f;
			result.y = 0.0f;
			result.z = 0.0f;
		}
		else if (dy > dz)
		{
			result.x = 0.0f;
			result.y = center1.y < center.y ? -1.0f : 1.0f;
			result.z = 0.0f;
		}
		else
		{
			result.x = 0.0f;
			result.y = 0.0f;
			result.z = center1.z < center.z ? -1.0f : 1.0f;
		}
	}
	else
	{
		// �d�Ȃ肪�Ȃ��ꍇ�͊ђʐ[�x��0
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthSphereRotBox(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// �_����]�̋t�N�I�[�^�j�I���ŉ�]�����ă{�b�N�X�̃��[�J����Ԃɕϊ�
	XMVECTOR qInv = XMQuaternionInverse(rotQton);
	XMVECTOR c1 = XMVector3Rotate(XMLoadFloat3(&center1), qInv);
	XMVECTOR c = XMVector3Rotate(XMLoadFloat3(&center), qInv);

	XMFLOAT3 localCenter1;
	XMFLOAT3 localCenter;

	XMStoreFloat3(&localCenter1, c1);
	XMStoreFloat3(&localCenter, c);

	return GetDepthSphereBox(localCenter1, r1, localCenter, size);
}

XMFLOAT4 GetDepthSphereTerrain(XMFLOAT3 center1, float r1, XMFLOAT3 center, XMFLOAT2 hw)
{
	return XMFLOAT4();
}

XMFLOAT4 GetDepthCapsuleCapsule(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 cp2_1, XMFLOAT3 cp2_2, float r2)
{
	XMVECTOR p1_1 = XMLoadFloat3(&cp1_1);
	XMVECTOR p1_2 = XMLoadFloat3(&cp1_2);
	XMVECTOR p2_1 = XMLoadFloat3(&cp2_1);
	XMVECTOR p2_2 = XMLoadFloat3(&cp2_2);

	XMVECTOR d1 = XMVectorSubtract(p1_2, p1_1);
	XMVECTOR d2 = XMVectorSubtract(p2_2, p2_1);

	float t1 = max(0.0f, min(1.0f, XMVectorGetX(XMVector3Dot(XMVectorSubtract(p2_1, p1_1), d1) / XMVectorGetX(XMVector3Dot(d1, d1)))));
	XMVECTOR closestPoint1 = XMVectorAdd(p1_1, t1 * d1);

	float t2 = max(0.0f, min(1.0f, XMVectorGetX(XMVector3Dot(XMVectorSubtract(p2_1, p1_1), d2) / XMVectorGetX(XMVector3Dot(d2, d2)))));
	XMVECTOR closestPoint2 = XMVectorAdd(p2_1, t2 * d2);

	XMVECTOR diff = XMVectorSubtract(closestPoint1, closestPoint2);
	float distance = XMVectorGetX(XMVector3Length(diff));
	float penetrationDepth = (r1 + r2) - distance;

	XMFLOAT4 result;
	// ���������a�̍��v�ȓ��Ȃ�ђʐ[�x�Ɩ@�����v�Z
	if (penetrationDepth > 0.0f)
	{
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), XMVector3Normalize(diff));
		result.w = penetrationDepth;
	}
	else
	{
		// �d�Ȃ肪�Ȃ��ꍇ�͊ђʐ[�x��0
		result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return result;
}

XMFLOAT4 GetDepthCapsuleBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size)
{
	XMVECTOR p1 = XMLoadFloat3(&cp1_1);
	XMVECTOR p2 = XMLoadFloat3(&cp1_2);
	XMVECTOR boxCenter = XMLoadFloat3(&center);

	XMFLOAT3 halfSize = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };
	XMFLOAT3 min = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
	XMFLOAT3 max = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z };

	float distance = 0.0f;
	float penetrationDepth = 0.0f;
	XMFLOAT4 result;

	// Find the closest point on the capsule to the box
	for (float t = 0.0f; t <= 1.0f; t += 0.01f)
	{
		XMVECTOR capsulePoint = p1 + t * (p2 - p1);
		XMFLOAT3 point;
		XMStoreFloat3(&point, capsulePoint);

		float dx = max(min.x - point.x, max(0.0f, point.x - max.x));
		float dy = max(min.y - point.y, max(0.0f, point.y - max.y));
		float dz = max(min.z - point.z, max(0.0f, point.z - max.z));
		distance = sqrt(dx * dx + dy * dy + dz * dz);
		penetrationDepth = r1 - distance;

		// If penetration depth is positive, calculate normal and return it
		if (penetrationDepth > 0.0f)
		{
			result.w = penetrationDepth;
			if (dx > dy && dx > dz)
			{
				result.x = point.x < center.x ? -1.0f : 1.0f;
				result.y = 0.0f;
				result.z = 0.0f;
			}
			else if (dy > dz)
			{
				result.x = 0.0f;
				result.y = point.y < center.y ? -1.0f : 1.0f;
				result.z = 0.0f;
			}
			else
			{
				result.x = 0.0f;
				result.y = 0.0f;
				result.z = point.z < center.z ? -1.0f : 1.0f;
			}
			return result;
		}
	}

	// �d�Ȃ肪�Ȃ��ꍇ�͊ђʐ[�x��0
	result = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	return result;
}

XMFLOAT4 GetDepthCapsuleRotBox(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT3 size, XMVECTOR rotQton)
{
	// �_����]�̋t�N�I�[�^�j�I���ŉ�]�����ă{�b�N�X�̃��[�J����Ԃɕϊ�
	XMVECTOR qInv = XMQuaternionInverse(rotQton);
	XMVECTOR p1 = XMVector3Rotate(XMLoadFloat3(&cp1_1), qInv);
	XMVECTOR p2 = XMVector3Rotate(XMLoadFloat3(&cp1_2), qInv);
	XMVECTOR boxCenter = XMVector3Rotate(XMLoadFloat3(&center), qInv);

	XMFLOAT3 localCp1_1;
	XMFLOAT3 localCp1_2;
	XMFLOAT3 localCenter;

	XMStoreFloat3(&localCp1_1, p1);
	XMStoreFloat3(&localCp1_2, p2);
	XMStoreFloat3(&localCenter, boxCenter);

	return GetDepthCapsuleBox(localCp1_1, localCp1_2, r1, localCenter, size);
}

XMFLOAT4 GetDepthCapsuleTerrain(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 center, XMFLOAT2 hw)
{
	return XMFLOAT4();
}

XMFLOAT4 GetDepthBoxBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2)
{
	XMFLOAT3 halfSize1 = { size1.x * 0.5f, size1.y * 0.5f, size1.z * 0.5f };
	XMFLOAT3 halfSize2 = { size2.x * 0.5f, size2.y * 0.5f, size2.z * 0.5f };

	float dx = fabs(center1.x - center2.x) - (halfSize1.x + halfSize2.x);
	float dy = fabs(center1.y - center2.y) - (halfSize1.y + halfSize2.y);
	float dz = fabs(center1.z - center2.z) - (halfSize1.z + halfSize2.z);

	XMFLOAT4 result;
	if (dx < 0 && dy < 0 && dz < 0)
	{
		// �e���̊ђʐ[�x���v�Z���A���̒��ōł��������l��Ԃ�
		float minDepth = -min(dx, min(dy, dz));

		if (minDepth == -dx)
		{
			result = XMFLOAT4(center1.x < center2.x ? -1.0f : 1.0f, 0.0f, 0.0f, minDepth);
		}
		else if (minDepth == -dy)
		{
			result = XMFLOAT4(0.0f, center1.y < center2.y ? -1.0f : 1.0f, 0.0f, minDepth);
		}
		else
		{
			result = XMFLOAT4(0.0f, 0.0f, center1.z < center2.z ? -1.0f : 1.0f, minDepth);
		}

		return result;
	}

	// �d�Ȃ肪�Ȃ��ꍇ�͊ђʐ[�x��0
	return XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

XMFLOAT4 GetDepthBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton)
{
	return GetDepthRotBoxRotBox(center1, size1, XMQuaternionIdentity(), center2, size2, rotQton);
}

XMFLOAT4 GetDepthBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMFLOAT3 center, XMFLOAT2 hw)
{
	return XMFLOAT4();
}



XMFLOAT4 GetDepthRotBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton2)
{
	XMVECTOR c1 = XMLoadFloat3(&center1);
	XMVECTOR c2 = XMLoadFloat3(&center2);
	XMVECTOR halfSize1 = XMLoadFloat3(&XMFLOAT3(size1.x * 0.5f, size1.y * 0.5f, size1.z * 0.5f));
	XMVECTOR halfSize2 = XMLoadFloat3(&XMFLOAT3(size2.x * 0.5f, size2.y * 0.5f, size2.z * 0.5f));

	// �e�{�b�N�X�̎��x�N�g�����v�Z
	XMVECTOR axes1[3] = {
		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQton1),
		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQton1),
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQton1)
	};

	XMVECTOR axes2[3] = {
		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQton2),
		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQton2),
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQton2)
	};

	// ���������i�[����x�N�g��
	vector<XMVECTOR> axes;

	// �{�b�N�X1�ƃ{�b�N�X2�̎���ǉ�
	axes.insert(axes.end(), begin(axes1), end(axes1));
	axes.insert(axes.end(), begin(axes2), end(axes2));

	// �N���X�ς��v�Z���Ēǉ�
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			axes.push_back(XMVector3Cross(axes1[i], axes2[j]));
		}
	}

	float minPenetrationDepth = FLT_MAX;
	XMVECTOR collisionNormal = XMVectorZero();

	for (const auto& axis : axes)
	{
		if (XMVector3Equal(axis, XMVectorZero()))
		{
			continue;
		}

		XMVECTOR axisNorm = XMVector3Normalize(axis);

		float min1 = FLT_MAX, max1 = -FLT_MAX;
		float min2 = FLT_MAX, max2 = -FLT_MAX;

		for (int i = 0; i < 8; ++i)
		{
			XMVECTOR corner1 = c1 + XMVectorSet((i & 1 ? halfSize1.m128_f32[0] : -halfSize1.m128_f32[0]),
				(i & 2 ? halfSize1.m128_f32[1] : -halfSize1.m128_f32[1]),
				(i & 4 ? halfSize1.m128_f32[2] : -halfSize1.m128_f32[2]), 0);

			XMVECTOR corner2 = c2 + XMVectorSet((i & 1 ? halfSize2.m128_f32[0] : -halfSize2.m128_f32[0]),
				(i & 2 ? halfSize2.m128_f32[1] : -halfSize2.m128_f32[1]),
				(i & 4 ? halfSize2.m128_f32[2] : -halfSize2.m128_f32[2]), 0);

			float projection1 = XMVectorGetX(XMVector3Dot(corner1, axisNorm));
			float projection2 = XMVectorGetX(XMVector3Dot(corner2, axisNorm));

			min1 = min(min1, projection1);
			max1 = max(max1, projection1);
			min2 = min(min2, projection2);
			max2 = max(max2, projection2);
		}

		float overlap = min(max1, max2) - max(min1, min2);

		if (overlap < 0.0f)
		{
			return XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		if (overlap < minPenetrationDepth)
		{
			minPenetrationDepth = overlap;
			collisionNormal = axisNorm;
		}
	}

	XMFLOAT4 result;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result), collisionNormal);
	result.w = minPenetrationDepth;

	return result;
}
//// SAT���g����2�̃{�b�N�X�̊ђʐ[�x�v�Z�p�֐�
//XMFLOAT4 GetDepthRotBoxRotBox(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton1, XMFLOAT3 center2, XMFLOAT3 size2, XMVECTOR rotQton2)
//{
//	XMVECTOR c1 = XMLoadFloat3(&center1);
//	XMVECTOR c2 = XMLoadFloat3(&center2);
//	XMVECTOR halfSize1 = XMLoadFloat3(&XMFLOAT3(size1.x * 0.5f, size1.y * 0.5f, size1.z * 0.5f));
//	XMVECTOR halfSize2 = XMLoadFloat3(&XMFLOAT3(size2.x * 0.5f, size2.y * 0.5f, size2.z * 0.5f));
//
//	// �e�{�b�N�X�̎��x�N�g�����v�Z
//	XMVECTOR axes1[3] = {
//		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQton1),
//		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQton1),
//		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQton1)
//	};
//
//	XMVECTOR axes2[3] = {
//		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQton2),
//		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQton2),
//		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQton2)
//	};
//
//	// ���������i�[����x�N�g��
//	vector<XMVECTOR> axes;
//
//	// �{�b�N�X1�ƃ{�b�N�X2�̎���ǉ�
//	axes.insert(axes.end(), begin(axes1), end(axes1));
//	axes.insert(axes.end(), begin(axes2), end(axes2));
//
//	// �N���X�ς��v�Z���Ēǉ�
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			axes.push_back(XMVector3Cross(axes1[i], axes2[j]));
//		}
//	}
//
//	float minPenetrationDepth = FLT_MAX;
//
//	for (const auto& axis : axes)
//	{
//		if (XMVector3Equal(axis, XMVectorZero()))
//		{
//			continue;
//		}
//
//		XMVECTOR axisNorm = XMVector3Normalize(axis);
//
//		float min1 = FLT_MAX, max1 = -FLT_MAX;
//		float min2 = FLT_MAX, max2 = -FLT_MAX;
//
//		for (int i = 0; i < 8; ++i)
//		{
//			XMVECTOR corner1 = c1 + XMVectorSet((i & 1 ? halfSize1.m128_f32[0] : -halfSize1.m128_f32[0]),
//				(i & 2 ? halfSize1.m128_f32[1] : -halfSize1.m128_f32[1]),
//				(i & 4 ? halfSize1.m128_f32[2] : -halfSize1.m128_f32[2]), 0);
//
//			XMVECTOR corner2 = c2 + XMVectorSet((i & 1 ? halfSize2.m128_f32[0] : -halfSize2.m128_f32[0]),
//				(i & 2 ? halfSize2.m128_f32[1] : -halfSize2.m128_f32[1]),
//				(i & 4 ? halfSize2.m128_f32[2] : -halfSize2.m128_f32[2]), 0);
//
//			float projection1 = XMVectorGetX(XMVector3Dot(corner1, axisNorm));
//			float projection2 = XMVectorGetX(XMVector3Dot(corner2, axisNorm));
//
//			min1 = min(min1, projection1);
//			max1 = max(max1, projection1);
//			min2 = min(min2, projection2);
//			max2 = max(max2, projection2);
//		}
//
//		float overlap = min(max1, max2) - max(min1, min2);
//
//		if (overlap < 0.0f)
//		{
//			return 0.0f;
//		}
//
//		minPenetrationDepth = min(minPenetrationDepth, overlap);
//	}
//
//	return minPenetrationDepth;
//}

XMFLOAT4 GetDepthRotBoxTerrain(XMFLOAT3 center1, XMFLOAT3 size1, XMVECTOR rotQton, XMFLOAT3 center, XMFLOAT2 hw)
{
	return XMFLOAT4();
}

void ClosestPtSegmentSegment(XMVECTOR p1, XMVECTOR d1, XMVECTOR p2, XMVECTOR d2, float& t1, float& t2, XMVECTOR& c1, XMVECTOR& c2)
{
	XMVECTOR r = XMVectorSubtract(p1, p2);
	float a = XMVectorGetX(XMVector3Dot(d1, d1));
	float e = XMVectorGetX(XMVector3Dot(d2, d2));
	float f = XMVectorGetX(XMVector3Dot(d2, r));

	// �ǂ��炩�̐������_�̏ꍇ
	if (a <= FLT_EPSILON && e <= FLT_EPSILON)
	{
		t1 = t2 = 0.0f;
		c1 = p1;
		c2 = p2;
		return;
	}
	if (a <= FLT_EPSILON)
	{
		t1 = 0.0f;
		t2 = f / e;
		t2 = clamp(t2, 0.0f, 1.0f);
	}
	else
	{
		float c = XMVectorGetX(XMVector3Dot(d1, r));
		if (e <= FLT_EPSILON)
		{
			t2 = 0.0f;
			t1 = clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = XMVectorGetX(XMVector3Dot(d1, d2));
			float denom = a * e - b * b;

			if (denom != 0.0f)
			{
				t1 = clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				t1 = 0.0f;
			}

			t2 = (b * t1 + f) / e;

			if (t2 < 0.0f)
			{
				t2 = 0.0f;
				t1 = clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t2 > 1.0f)
			{
				t2 = 1.0f;
				t1 = clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = XMVectorAdd(p1, XMVectorScale(d1, t1));
	c2 = XMVectorAdd(p2, XMVectorScale(d2, t2));
}




