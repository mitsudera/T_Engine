#pragma once
#include "primitivecomponent.h"
#include "ShaderSet.h"
#include "renderer.h"
class RenderTexture;

class TextMeshComponent :public PrimitiveComponent
{
public:
	TextMeshComponent(GameObject* gameObject);
	~TextMeshComponent();

	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void Draw(void) override;

	void CreateVertexBuffer(void);

	void CreateTextMesh(string text, float fontSize, XMFLOAT4 color, XMFLOAT3 pos, XMFLOAT2 size, string font, TEXT_ANCHOR textAnchor);



private:

	int texIndex;
	ID3D11Texture2D* pTexture;
	ID3D11ShaderResourceView* pSRV;



	VERTEX_3D vertexArray[4];
	ID3D11Buffer* vertexBuffer;
	string outputText;


};

