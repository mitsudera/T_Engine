#include"math.h"

XMMATRIX mtxrotX(float x)
{
	float sx = sinf(x);
	float cx = cosf(x);

	XMMATRIX mtxrotx = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cx,-sx,0.0f,
		0.0f,sx,cx,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxrotx;
}
XMMATRIX mtxrotY(float y)
{
	float sy = sinf(y);
	float cy = cosf(y);

	XMMATRIX mtxroty = {
		cy,0.0f,sy,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		-sy,0.0f,cy,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxroty;

}
XMMATRIX mtxrotZ(float z)
{
	float sz = sinf(z);
	float cz = cosf(z);

	XMMATRIX mtxrotz = {
		cz,-sz,0.0f,0.0f,
		sz,cz,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxrotz;

}

XMMATRIX mtxrotZYX(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(cy * cz),(sx * sy * cz - cx * sz),(cx * sy * cz + sx * sz),0.0f,
		(cy * sz),(sx * sy * sz + cx * cz),(cx * sy * sz - sx * cz),0.0f,
		(-sy),(sx * cy),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}

XMMATRIX mtxrotZXY(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(-sx * sy * sz + cy * cz),(-cx * sz),(sx * cy * sz + sy * cz),0.0f,
		(sx * sy * cz + cy * sz),(cx * cz),(-sx * cy * cz + sy * sz),0.0f,
		(-cx * sy),(sx),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}
XMMATRIX mtxrotYXZ(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(sx * sy * sz + cy * cz),(sx * sy * cz - cy * sz),(cx * sy),0.0f,
		(cx * sz),(cx * cz),(-sx),0.0f,
		(sx * cy * sz - sy * cz),(sx * cy * cz + sy * sz),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}

XMMATRIX mtxrotXYZ(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(cy * cz),(-cy * sz),(sy),0.0f,
		(sx * sy * cz + cx * sz),(-sx * sy * sz + cx * cz),(-sx * cy),0.0f,
		(-cx * sy * cz + sx * sz),(cx * sy * sz + sx * cz),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}






XMVECTOR xonevec(void)
{
	XMFLOAT3 x = { 1.0f,0.0f,0.0f };

	XMVECTOR vec = XMLoadFloat3(&x);
	return vec;
}

XMVECTOR yonevec(void)
{
	XMFLOAT3 y = { 0.0f,1.0f,0.0f };

	XMVECTOR vec = XMLoadFloat3(&y);
	return vec;
}

XMVECTOR zonevec(void)
{
	XMFLOAT3 z = { 0.0f,0.0f,1.0f };

	XMVECTOR vec = XMLoadFloat3(&z);
	return vec;
}

float XMFLOAT3Length(XMFLOAT3 p1, XMFLOAT3 p2)
{
	XMVECTOR p1v = XMLoadFloat3(&p1);
	XMVECTOR p2v = XMLoadFloat3(&p2);
	XMVECTOR lenV = p2v - p1v;
	XMVECTOR len = XMVector3Length(lenV);
	float ans;
	XMStoreFloat(&ans, len);

	return ans;
}



float GetAngleInRadians(XMFLOAT2 vector1, XMFLOAT2 vector2)
{
	// ベクトルを正規化
	XMVECTOR vec1 = XMLoadFloat2(&vector1);
	XMVECTOR vec2 = XMLoadFloat2(&vector2);
	vec1 = XMVector2Normalize(vec1);
	vec2 = XMVector2Normalize(vec2);

	// ドット積を計算
	float dotProduct = XMVectorGetX(XMVector2Dot(vec1, vec2));

	// ドット積からラジアン角度を取得
	float angleInRadians = asinf(dotProduct);

	return angleInRadians;
}

XMFLOAT2 GetVectorFromAngleInRadians(float angleInRadians)
{
	// 角度からXYベクトルを計算
	float x = sinf(angleInRadians);
	float y = cosf(angleInRadians);

	return XMFLOAT2(x, y);
}

float GetAngleInRadiansFromVector(XMFLOAT2 vector)
{
	// ベクトルの正規化
	float length = sqrtf(vector.x * vector.x + vector.y * vector.y);
	float normalizedX = vector.x / length;
	float normalizedY = vector.y / length;

	// XYベクトルからラジアン角度を計算
	float angleInRadians = atan2f(normalizedY, normalizedX);

	return angleInRadians;
}

BOOL IsInsideFrustum(XMVECTOR center, XMVECTOR size, XMMATRIX wvp)
{
	// バウンディングボックスの8つの頂点を計算
	XMVECTOR vertices[8];
	vertices[0] = XMVectorSet(center.m128_f32[0] - size.m128_f32[0] / 2, center.m128_f32[1] - size.m128_f32[1] / 2, center.m128_f32[2] - size.m128_f32[2] / 2, 1.0f);
	vertices[1] = XMVectorSet(center.m128_f32[0] + size.m128_f32[0] / 2, center.m128_f32[1] - size.m128_f32[1] / 2, center.m128_f32[2] - size.m128_f32[2] / 2, 1.0f);
	vertices[2] = XMVectorSet(center.m128_f32[0] - size.m128_f32[0] / 2, center.m128_f32[1] + size.m128_f32[1] / 2, center.m128_f32[2] - size.m128_f32[2] / 2, 1.0f);
	vertices[3] = XMVectorSet(center.m128_f32[0] + size.m128_f32[0] / 2, center.m128_f32[1] + size.m128_f32[1] / 2, center.m128_f32[2] - size.m128_f32[2] / 2, 1.0f);
	vertices[4] = XMVectorSet(center.m128_f32[0] - size.m128_f32[0] / 2, center.m128_f32[1] - size.m128_f32[1] / 2, center.m128_f32[2] + size.m128_f32[2] / 2, 1.0f);
	vertices[5] = XMVectorSet(center.m128_f32[0] + size.m128_f32[0] / 2, center.m128_f32[1] - size.m128_f32[1] / 2, center.m128_f32[2] + size.m128_f32[2] / 2, 1.0f);
	vertices[6] = XMVectorSet(center.m128_f32[0] - size.m128_f32[0] / 2, center.m128_f32[1] + size.m128_f32[1] / 2, center.m128_f32[2] + size.m128_f32[2] / 2, 1.0f);
	vertices[7] = XMVectorSet(center.m128_f32[0] + size.m128_f32[0] / 2, center.m128_f32[1] + size.m128_f32[1] / 2, center.m128_f32[2] + size.m128_f32[2] / 2, 1.0f);

	// 各頂点を視錘台空間に変換し、スクリーン内にあるかチェック
	for (int i = 0; i < 8; ++i)
	{
		XMVECTOR transformedVertex = XMVector3TransformCoord(vertices[i], wvp);
		XMFLOAT3 cp;
		cp.x = (XMVectorGetX(transformedVertex) + 1.0f) * 0.5f;
		cp.y= (-XMVectorGetY(transformedVertex) + 1.0f) * 0.5f;
		cp.z = XMVectorGetZ(transformedVertex);

		if ((cp.x > -1.0f && cp.x < 1.0f) &&
			(cp.y > -1.0f && cp.y < 1.0f) &&
			(cp.z > 0.0f && cp.z < 1.0f))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IsCenterInsideFrustum(XMVECTOR center, XMMATRIX frustum)
{
	XMFLOAT3 cp;
	XMStoreFloat3(&cp, XMVector3TransformCoord(center, frustum));
	cp.x = (cp.x + 1.0f) * 0.5f;
	cp.y = (-cp.y + 1.0f) * 0.5f;


	if ((cp.x > -1.0f && cp.x < 1.0f) &&
		(cp.y > -1.0f && cp.y < 1.0f) &&
		(cp.z > 0.0f && cp.z < 1.0f))
	{
		return TRUE;

	}
	return FALSE;
}

void QuaternionToEulerAngles(const XMVECTOR& quat, XMFLOAT3& rot)
{
	XMFLOAT4 q;
	XMStoreFloat4(&q, quat);

	auto sx = -(2 * q.y * q.z - 2 * q.x * q.w);
	auto unlocked = std::abs(sx) < 0.99999f;
	rot = XMFLOAT3(asin(sx),
		unlocked ? atan2(2 * q.x * q.z + 2 * q.y * q.w, 2 * q.w * q.w + 2 * q.z * q.z - 1) : atan2(-(2 * q.x * q.z - 2 * q.y * q.w), 2 * q.w * q.w + 2 * q.x * q.x - 1),
		unlocked ? atan2(2 * q.x * q.y + 2 * q.z * q.w, 2 * q.w * q.w + 2 * q.y * q.y - 1) : 0);
		
}