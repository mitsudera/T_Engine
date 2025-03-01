#pragma once
#include "Material.h"
#include "TerrainShader.h"

class DX11Texture;


class TerrainMaterial :public Material
{
public:
	TerrainMaterial(AssetsManager* assetsManager);
	TerrainMaterial(TerrainMaterial* phong);
	~TerrainMaterial();

	// Material ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void SetBufferMaterial(void) override;


	void SetTessEdgeFacter(float facter);
	void SetTessInsideFacter(float facter);
	void SetHeightFacter(float facter);
	void SetUVScale(XMFLOAT2 scale);
	void SetHeghtMapUVScale(XMFLOAT2 scale);
	float GetHeightFacter(void);

	float GetEdgeFacter(void);

	float GetInsideFacter(void);

	XMFLOAT2 GetUVscale(void);

	XMFLOAT2 GetMapUVscale(void);


private:
	TerrainShader* pTerrainShader;

	float tessEdgeFacter;
	float tessInsideFacter;
	float heightFacter;
	XMFLOAT2 uvScale;
	XMFLOAT2 heightMapUvScale;

};

