#pragma once
#include "primitivecomponent.h"
class DX11Texture;

class QuadComponent :public PrimitiveComponent
{
public:
	
	QuadComponent(GameObject* gameObject);
	~QuadComponent();
	virtual void Awake(void) override;
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;

	void LoadTexture(string path);

	void SetBillBorad(BOOL b);

private:
	DX11Texture* texture;
	ID3D11Buffer* vertexBuffer;
	BOOL isBillBorad;
};

