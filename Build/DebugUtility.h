#pragma once
#include "DebugLineShader.h"

class GameEngine;
class Renderer;
class MeshData;
class CBufferManager;
class DebugUtility
{
public:
	DebugUtility(GameEngine* gameEngine);
	~DebugUtility();

	void DrawDebugSphere(void);
	void DrawDebugBox(void);
	void DrawDebugCapsule(XMFLOAT3 p, float length, float radius, XMMATRIX rot);
	void DrawDebugCapsule(XMFLOAT3 sp, XMFLOAT3 ep, float radius, XMMATRIX world);

	void SetDebugLineShader(void);

private:

	GameEngine* pGameEngine;
	Renderer* pRenderer;
	DebugLineShader* debugLineShader;
	CBufferManager* cBufferManager;
	ID3D11Buffer* vertexSphere;
	ID3D11Buffer* indexSphere;

	MeshData* sphereMeshData;
	MeshData* cylinderMeshData;
	MeshData* capsuleMeshData1;
	MeshData* capsuleMeshData2;
	MeshData* capsuleMeshData3;

	ID3D11Buffer* vertexCylinder;

	ID3D11Buffer* vertexBox;
	ID3D11Buffer* indexBox;
};

