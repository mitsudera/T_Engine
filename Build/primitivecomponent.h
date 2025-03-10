#pragma once
#include "Component.h"

class Material;
class LambartMaterial;
class PhongMaterial;
class SkyMaterial;
class UIMaterial;
class Renderer;
class CBufferManager;
class AssetsManager;

class PrimitiveComponent : public Component
{
public:
	PrimitiveComponent();
	PrimitiveComponent(GameObject* gameObject);
	~PrimitiveComponent();

	BOOL GetHasShadow(void);
	void SetHasShadow(BOOL b);

	BOOL GetDrawShadow(void);
	void SetDrawShadow(BOOL b);

	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void Draw(void) override;
	virtual void ShadowMapping(void);


	Material* GetMaterial(void);
	void SetMaterial(Material* material);
	Material* LoadMaterial(Material* material);

	Material* GetShadowMaterial(void);
	void SetShadowMaterial(Material* material);
	Material* LoadShadowMaterial(Material* material);

	void SetCullingMode(int cullMode);
	int GetCullingMode(void);

	void SetAlphaTest(BOOL enable);
	BOOL GetAlphaTest(void);

	BOOL GetIsFrustumCulling(void);

	void SetIsFrustumCulling(BOOL b);

	void SetBoxMinMax(XMVECTOR min, XMVECTOR max);
	XMVECTOR GetBoxMin(void);
	XMVECTOR GetBoxMax(void);
	 
protected:
	Renderer* pRenderer;
	CBufferManager* pCBufferManager;
	AssetsManager* pAssetsManager;

	BOOL hasShadow;
	BOOL drawShadow;
	BOOL alphaTest;
	int cullMode;
	BOOL isFrustumCulling;

	Material* material;
	Material* shadowMaterial;


	XMVECTOR boxMin;
	XMVECTOR boxMax;

};

