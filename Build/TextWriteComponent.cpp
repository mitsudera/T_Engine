#include "TextWriteComponent.h"
#include "GameEngine.h"
TextWriteComponent::TextWriteComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

TextWriteComponent::~TextWriteComponent()
{
}

void TextWriteComponent::Awake(void)
{
	PrimitiveComponent::Awake();
	this->hasShadow = FALSE;
	this->drawShadow = FALSE;
	this->outText = "TEST";
	this->textColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->pos = { 0.0f,0.0f };
	this->size = { 1920.0f,200.0f };
	this->anchor = { 0.0f, 0.0f };
	this->pivot = { 0.5f,0.5f };
	this->fontSize = 100.0f;
	this->font = "Showcard Gothic";
	this->SetText("test");
	this->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	this->SetPos(XMFLOAT2(0.0f, 0.0f));
	this->SetSize(XMFLOAT2(1920.0f, 1080.0f));
	this->SetFontSize(200.0f);
	this->SetAnchor(XMFLOAT2(0.5f, 0.5f));
	this->SetPivot(XMFLOAT2(0.5f, 0.5f));
	this->SetTextAnchor(TEXT_ANCHOR_CENTER_CENTER);
}

void TextWriteComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void TextWriteComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
}

void TextWriteComponent::Draw(void)
{
	PrimitiveComponent::Draw();

	XMFLOAT2 p;


	p.x = this->pos.x + (pGameEngine->GetWindowSize().x * anchor.x) - size.x * pivot.x;
	p.y = this->pos.y + (pGameEngine->GetWindowSize().y * anchor.y) - size.y * pivot.y;
	pRenderer->DrawStringText(
		this->outText,
		this->fontSize,
		this->textColor,
		p,
		this->size,
		this->font,
		this->textAnchor);
}

void TextWriteComponent::SetText(string text)
{
	this->outText = text;
}

void TextWriteComponent::SetColor(XMFLOAT4 color)
{
	this->textColor = color;
}

void TextWriteComponent::SetAlpha(float alpha)
{
	textColor.w = alpha;
}

void TextWriteComponent::SetPos(XMFLOAT2 pos)
{
	this->pos = pos;
}

void TextWriteComponent::SetSize(XMFLOAT2 size)
{
	this->size = size;
}

void TextWriteComponent::SetFontSize(float fontSize)
{
	this->fontSize = fontSize;
}

void TextWriteComponent::SetAnchor(XMFLOAT2 anchor)
{
	this->anchor = anchor;
}

void TextWriteComponent::SetPivot(XMFLOAT2 pivot)
{
	this->pivot = pivot;
}

void TextWriteComponent::SetTextAnchor(TEXT_ANCHOR textAnchor)
{
	this->textAnchor = textAnchor;
}
