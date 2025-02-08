#pragma once
#include "primitivecomponent.h"
#include "renderer.h"
class TextWriteComponent :public PrimitiveComponent
{
public:
	TextWriteComponent(GameObject* gameObject);
	~TextWriteComponent();

	virtual void Awake(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void Draw(void) override;

	void SetText(string text);
	void SetColor(XMFLOAT4 color);
	void SetAlpha(float alpha);
	void SetPos(XMFLOAT2 pos);
	void SetSize(XMFLOAT2 size);
	void SetFontSize(float fontSize);
	void SetAnchor(XMFLOAT2 anchor);
	void SetPivot(XMFLOAT2 pivot);
	void SetTextAnchor(TEXT_ANCHOR textAnchor);
private:

	string outText;
	XMFLOAT4 textColor;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	XMFLOAT2 anchor;
	XMFLOAT2 pivot;
	float fontSize;
	string font;
	TEXT_ANCHOR textAnchor;

};

