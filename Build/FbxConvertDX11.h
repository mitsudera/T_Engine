#pragma once
#include "CoreMinimal.h"

XMFLOAT3 FbxDouble3ConvertToXMFLOAT3(FbxDouble3 fbxd3);

XMMATRIX FbxMatrixConvertToXMMATRIX(FbxMatrix fbxmat);

XMFLOAT3 XMFLOAT3Normalize(XMFLOAT3 p);

XMFLOAT3 XMFLOAT3GetNormal(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3);