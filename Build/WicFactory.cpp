#include "WicFactory.h"
#include "GameEngine.h"

WicFactory::WicFactory(GameEngine* gameEngine)
{
	pGameEngine = gameEngine;
	HRESULT hr;
	// WICファクトリーの作成
	pWICFactory = nullptr;
	hr = CoInitialize(nullptr);
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));


}

WicFactory::~WicFactory()
{
	pWICFactory->Release();
}

FileTexture* WicFactory::LoadFileTexture(string filePath)
{
	FileTexture* fileTexture = new FileTexture;

	fileTexture->filePath = filePath;

	wstring wstr(filePath.begin(), filePath.end());

	// 画像ファイルの読み込み
	IWICBitmapDecoder* pDecoder = nullptr;
	pWICFactory->CreateDecoderFromFilename(wstr.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	// フレームの取得
	IWICBitmapFrameDecode* pFrame = nullptr;
	pDecoder->GetFrame(0, &pFrame);

	// 画像の幅と高さを取得
	UINT width = 0;
	UINT height = 0;
	pFrame->GetSize(&width, &height);

	fileTexture->width = width;
	fileTexture->height = height;
	
	// フォーマット変換
	IWICFormatConverter* pConverter = nullptr;
	pWICFactory->CreateFormatConverter(&pConverter);
	pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

	// ピクセルデータのバッファを作成
	BYTE* pPixels;
	UINT stride = width * 4; // 1ピクセルあたり4バイト（RGBA）
	UINT bufferSize = stride * height;
	pPixels = new BYTE[bufferSize];
	// ピクセルデータを取得
	pFrame->CopyPixels(nullptr, stride, bufferSize, pPixels);

	int cnt = 0;
	unsigned int pixelNum = width * height;

	fileTexture->color = new XMFLOAT4[pixelNum];

	
	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			BYTE* pixel = pPixels + y * stride + x * 4;
			// ピクセルデータの操作（例：色の変更） 
			float r = pixel[0] / 255.0f; // R 
			float g = pixel[1] / 255.0f; // G 
			float b = pixel[2] / 255.0f; // B 
			float a = pixel[3] / 255.0f; // A		

			fileTexture->color[cnt].x = r;
			fileTexture->color[cnt].y = g;
			fileTexture->color[cnt].z = b;
			fileTexture->color[cnt].w = a;

			cnt++;
		}
	}

	pDecoder->Release();
	pFrame->Release();
	pConverter->Release();

	return fileTexture;
}

FileTexture::~FileTexture()
{
}

XMFLOAT4 FileTexture::sampling(XMFLOAT2 TexCoord)
{


	float x = (TexCoord.x * (float)(width-1));
	float y = (TexCoord.y * (float)(height-1));

	int x1 = (int)x;
	int y1 = (int)y;
	int x2 = x1 + 1;
	int y2 = y1 + 1;

	// uv0.0~1.0の外をどうするかとりあえずcalmpで 

	x1 = clamp(x1, 0, (int)(width - 1));
	x2 = clamp(x2, 0, (int)(width - 1));
	y1 = clamp(y1, 0, (int)(height - 1));
	y2 = clamp(y2, 0, (int)(height - 1));
	

	float dx = x - x1;
	float dy = y - y1;
	float sampleWeight1 = (1 - dx) * (1 - dy);
	float sampleWeight2 = dx * (1 - dy);
	float sampleWeight3 = (1 - dx) * dy;
	float sampleWeight4 = dx * dy;

	int samplePos1 = (y1 * width) + x1;
	int samplePos2 = (y1 * width) + x2;
	int samplePos3 = (y2 * width) + x1;
	int samplePos4 = (y2 * width) + x2;

	XMFLOAT4 color1 = color[samplePos1];
	color1.x *= sampleWeight1;
	color1.y *= sampleWeight1;
	color1.z *= sampleWeight1;
	color1.w *= sampleWeight1;

	XMFLOAT4 color2 = color[samplePos2];
	color2.x *= sampleWeight2;
	color2.y *= sampleWeight2;
	color2.z *= sampleWeight2;
	color2.w *= sampleWeight2;

	XMFLOAT4 color3 = color[samplePos3];
	color3.x *= sampleWeight3;
	color3.y *= sampleWeight3;
	color3.z *= sampleWeight3;
	color3.w *= sampleWeight3;

	XMFLOAT4 color4 = color[samplePos4];
	color4.x *= sampleWeight4;
	color4.y *= sampleWeight4;
	color4.z *= sampleWeight4;
	color4.w *= sampleWeight4;

	XMFLOAT4 retColor;
	retColor.x = color1.x + color2.x + color3.x + color4.x;
	retColor.y = color1.y + color2.y + color3.y + color4.y;
	retColor.z = color1.z + color2.z + color3.z + color4.z;
	retColor.w = color1.w + color2.w + color3.w + color4.w;

	return 	retColor;
}

