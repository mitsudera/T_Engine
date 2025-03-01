#include "TextMeshComponent.h"
#include "renderer.h"
#include "gameobject.h"
#include "GameEngine.h"
#include "Scene.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "UIMaterial.h"
#include "GameEngine.h"
#include "gameobject.h"
#include "AssetsManager.h"
#include "Scene.h"
#include "CBufferManager.h"
#include "RenderTexture.h"
#include "savebitmap.h"

TextMeshComponent::TextMeshComponent(GameObject* gameObject)
{
	pGameObject = gameObject;
}

TextMeshComponent::~TextMeshComponent()
{

}

void TextMeshComponent::Awake(void)
{
	PrimitiveComponent::Awake();
	CreateVertexBuffer();
	this->material = pGameObject->GetScene()->GetGameEngine()->GetAssetsManager()->GetMaterial("UIMaterial");
	SetAlphaTest(TRUE);
    TypeName = typeid(TextMeshComponent).name();;

}

void TextMeshComponent::Update(void)
{
	PrimitiveComponent::Update();
}

void TextMeshComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();
	if (vertexBuffer) vertexBuffer->Release();
    if (pTexture) pTexture->Release();
    if (pSRV) pSRV->Release();
}

void TextMeshComponent::Draw(void)
{
	PrimitiveComponent::Draw();

	//���_�o�b�t�@�̒��g�𖄂߂�

	D3D11_MAPPED_SUBRESOURCE msr;
	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	memcpy(pVtx, vertexArray, sizeof(VERTEX_3D) * 4);

	this->GetGameObject()->GetScene()->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->vertexBuffer, 0);


	pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    pRenderer->GetDeviceContext()->PSSetShaderResources(0, 1, &pSRV);


	pRenderer->GetDeviceContext()->Draw(4, 0);

}

void TextMeshComponent::CreateVertexBuffer(void)
{

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBuffer);

}


void TextMeshComponent::CreateTextMesh(string text, float fontSize, XMFLOAT4 color, XMFLOAT3 pos, XMFLOAT2 size, string font, TEXT_ANCHOR textAnchor)
{
    outputText = text;

    // �f�o�C�X�R���e�L�X�g�̍쐬
    HDC hdc = CreateCompatibleDC(NULL);


    HFONT hFont = CreateFont((int)fontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, font.c_str());//�t�H���g�̎w��@�����ɂł���


    SelectObject(hdc, hFont);

    // �r�b�g�}�b�v�̍쐬
    int bitmapWidth = static_cast<int>(size.x);
    int bitmapHeight = static_cast<int>(size.y);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, bitmapWidth, bitmapHeight);
    SelectObject(hdc, hBitmap);


    int rectDT = DT_SINGLELINE;	//�c��RECT���L��������

    switch (textAnchor % 3)
    {	//����RECT
    case 0:
        rectDT |= DT_LEFT;
        break;
    case 1:
        rectDT |= DT_CENTER;
        break;
    case 2:
        rectDT |= DT_RIGHT;
        break;
    default:
        break;
    }

    switch (textAnchor / 3)
    {	//�c��RECT
    case 0:
        rectDT |= DT_TOP;
        break;
    case 1:
        rectDT |= DT_VCENTER;
        break;
    case 2:
        rectDT |= DT_BOTTOM;
        break;
    default:
        break;
    }

    // �r�b�g�}�b�v�ɕ�����`��
    RECT rect = { 0, 0, bitmapWidth, bitmapHeight };

    DrawText(hdc, const_cast<char*>(text.c_str()), (int)text.length(), &rect, rectDT);


    // �r�b�g�}�b�v�f�[�^�̎擾
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    std::vector<BYTE> bitmapData(bitmap.bmWidthBytes * bitmap.bmHeight);
    GetBitmapBits(hBitmap, bitmap.bmWidthBytes * bitmap.bmHeight, bitmapData.data());
    SaveBitMapImage(hBitmap, text);


    // DirectX11�e�N�X�`���̍쐬
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = bitmap.bmWidth;
    desc.Height = bitmap.bmHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = bitmapData.data();
    initData.SysMemPitch = bitmap.bmWidthBytes;

    pRenderer->GetDevice()->CreateTexture2D(&desc, &initData, &pTexture);

    // �V�F�[�_�[���\�[�X�r���[�̍쐬
    pRenderer->GetDevice()->CreateShaderResourceView(pTexture, nullptr, &pSRV);

    // �e�N�X�`���𒸓_�ɓK�p
    XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
    float z = pos.z;

    float w = (size.x / screenHW.x) * 2.0f;
    float h = (size.y / screenHW.y) * 2.0f;
    float l = (pos.x / screenHW.x) - w * 0.5f;
    float t = (pos.y / screenHW.y) - h * 0.5f;

    vertexArray[0].Position = { l, t + h, z };
    vertexArray[0].Diffuse = { color.x, color.y, color.z, color.w };
    vertexArray[0].TexCoord = { 0.0f, 0.0f };

    vertexArray[1].Position = { l + w, t + h, z };
    vertexArray[1].Diffuse = { color.x, color.y, color.z, color.w };
    vertexArray[1].TexCoord = { 1.0f, 0.0f };

    vertexArray[2].Position = { l, t, z };
    vertexArray[2].Diffuse = { color.x, color.y, color.z, color.w };
    vertexArray[2].TexCoord = { 0.0f, 1.0f };

    vertexArray[3].Position = { l + w, t, z };
    vertexArray[3].Diffuse = { color.x, color.y, color.z, color.w };
    vertexArray[3].TexCoord = { 1.0f, 1.0f };
}
//void TextMeshComponent::CreateTextMeshLeftDown(string text, XMFLOAT3 pos, float width, float height)
//{
//
//	XMFLOAT2 screenHW = pGameEngine->GetWindowSize();
//	float z = pos.z;
//
//	float w = (width / screenHW.x) * 2.0f;
//	float h = (height / screenHW.y) * 2.0f;
//	float l = ((pos.x / screenHW.x) - 0.5f) * 2.0f;
//	float t = ((pos.y / screenHW.y) - 0.5f) * 2.0f;
//
//
//
//
//
//	vertexArray[0].Position = { l,t + h,z };
//	vertexArray[0].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
//	vertexArray[0].TexCoord = { 0.0f,0.0f };
//
//
//	vertexArray[1].Position = { l + w,t + h,z };
//	vertexArray[1].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
//	vertexArray[1].TexCoord = { 1.0f,0.0f };
//
//	vertexArray[2].Position = { l,t ,z };
//	vertexArray[2].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
//	vertexArray[2].TexCoord = { 0.0f,1.0f };
//
//	vertexArray[3].Position = { l + w ,t,z };
//	vertexArray[3].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
//	vertexArray[3].TexCoord = { 1.0f,1.0f };
//
//
//}
//
