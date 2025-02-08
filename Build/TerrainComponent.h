#pragma once
#include "primitivecomponent.h"
#include "ShaderSet.h"
#include "WicFactory.h"
class TerrainComponent :public PrimitiveComponent
{
public:
	TerrainComponent(GameObject* gameObject);
	~TerrainComponent();
	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;


	void CreateVetexBuffer(XMINT2 resolution, XMFLOAT2 size);

	void LoadHeightMap(string path);



	float GetHeight(XMFLOAT3 pos);

	XMFLOAT2 GetHW(void);
	void SetHeightMapPath(string path);
	string GetHeightMapPath(void);

	void SetHeightFacter(float facter);
	float GetHeightFacter(void);

private:
	
	XMINT2 terrainResolution;
	XMFLOAT2 terrainSize;
	int vertNum;
	int indexNum;
	VERTEX_3D* vertexArray;
	int* indexArray;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11ShaderResourceView* heightMapSRV;
	FileTexture* heightMap;
	string heightMapPath;
	


};

