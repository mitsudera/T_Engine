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

	// Material ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void SetBufferMaterial(void) override;


	void SetTessEdgeFacter(float facter);
	void SetTessInsideFacter(float facter);
	void SetHeightFacter(float facter);
	void SetUVScale(XMFLOAT2 scale);
	void SetHeghtMapUVScale(XMFLOAT2 scale);
	float GetHeightFacter(void);


private:
	TerrainShader* pTerrainShader;
	int heightMapTexIndex;

	float tessEdgeFacter;
	float tessInsideFacter;
	float heightFacter;
	XMFLOAT2 uvScale;
	XMFLOAT2 heightMapUvScale;

};

