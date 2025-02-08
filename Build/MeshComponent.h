#pragma once
#include "PrimitiveComponent.h"

class MeshData;

class Mesh
{

};

class MeshComponent :public PrimitiveComponent
{



public:

	MeshComponent(GameObject* gameObject);
	~MeshComponent();

	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;

	virtual void ShadowMapping(void) override;


	void SetMeshData(MeshData* data);



protected:

	string meshFilePath;
	
	MeshData* meshData;
	

	BOOL isRoot;

};

