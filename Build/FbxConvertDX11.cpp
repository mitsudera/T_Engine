#include "FbxConvertDX11.h"


XMFLOAT3 FbxDouble3ConvertToXMFLOAT3(FbxDouble3 fbxd3)
{
	XMFLOAT3 xmfl3;

	xmfl3.x = (float)fbxd3[0];
	xmfl3.y = (float)fbxd3[1];
	xmfl3.z = (float)fbxd3[2];

	return xmfl3;
}

#include <DirectXMath.h>
#include <fbxsdk.h>  // FBX SDK のヘッダファイル

using namespace DirectX;

XMMATRIX FbxMatrixConvertToXMMATRIX(FbxMatrix fbxmat)
{
    XMFLOAT4X4 matrix;
    matrix._11 = (float)fbxmat[0][0];
    matrix._12 = (float)fbxmat[0][1];
    matrix._13 = (float)fbxmat[0][2];
    matrix._14 = (float)fbxmat[0][3];

    matrix._21 = (float)fbxmat[1][0];
    matrix._22 = (float)fbxmat[1][1];
    matrix._23 = (float)fbxmat[1][2];
    matrix._24 = (float)fbxmat[1][3];

    matrix._31 = (float)fbxmat[2][0];
    matrix._32 = (float)fbxmat[2][1];
    matrix._33 = (float)fbxmat[2][2];
    matrix._34 = (float)fbxmat[2][3];

    matrix._41 = (float)fbxmat[3][0];
    matrix._42 = (float)fbxmat[3][1];
    matrix._43 = (float)fbxmat[3][2];
    matrix._44 = (float)fbxmat[3][3];

    // XMFLOAT4X4 から XMMATRIX に変換
    XMMATRIX xmat = XMLoadFloat4x4(&matrix);


    return xmat;
}
XMFLOAT3 XMFLOAT3Normalize(XMFLOAT3 p)
{
	XMVECTOR v = XMLoadFloat3(&p);

	v = XMVector3Normalize(v);

	XMFLOAT3 ans;

	XMStoreFloat3(&ans, v);

	return ans;

}

XMFLOAT3 XMFLOAT3GetNormal(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3)
{

	XMVECTOR p1v = XMLoadFloat3(&p1);
	XMVECTOR p2v = XMLoadFloat3(&p2);
	XMVECTOR p3v = XMLoadFloat3(&p3);

	XMVECTOR v1 = p2v - p1v;
	XMVECTOR v2 = p3v - p1v;

	XMVECTOR cv = XMVector3Cross(v1, v2);

	cv = XMVector3Normalize(cv);

	XMFLOAT3 vec;
	XMStoreFloat3(&vec, cv);

	return vec;
}


