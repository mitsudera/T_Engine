#include "LambartMaterial.h"
#include "LambartShader.h"
#include "DX11Texture.h"
#include "AssetsManager.h"

LambartMaterial::LambartMaterial(AssetsManager* assetsManager)
{

	this->pAssetsManager = assetsManager;

	this->pShader = assetsManager->GetLambartShader();
	this->pLambartShader = assetsManager->GetLambartShader();

	this->diffuse = { 1.0f,1.0f,1.0f,1.0f };
	this->noDiffuseTex = 1;
	this->noNormalTex = 1;
	this->noSpecularTex = 1;
	this->textureDiffuse = 0;
	this->textureNormal = 0;
	this->textureSpeculer = 0;


}

LambartMaterial::LambartMaterial(LambartMaterial* lambart)
{
	this->pAssetsManager = lambart->pAssetsManager;

	this->pShader = lambart->pShader;
	this->pLambartShader = lambart->pLambartShader;

	this->diffuse = lambart->diffuse;
	this->noDiffuseTex = lambart->noDiffuseTex;
	this->noNormalTex = lambart->noNormalTex;
	this->noSpecularTex = lambart->noSpecularTex;
	this->textureDiffuse = lambart->textureDiffuse;
	this->textureNormal = lambart->textureNormal;
	this->textureSpeculer = lambart->textureSpeculer;



}

LambartMaterial::~LambartMaterial()
{
}

void LambartMaterial::SetBufferMaterial(void)
{
	LambartShader::MaterialCBuffer mCBuffer;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noSpecularTex;
	this->pLambartShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) textureDiffuse->SetShaderResourcePS(0);
	if (!noNormalTex)textureNormal->SetShaderResourcePS(1);
	if (!noSpecularTex) textureSpeculer->SetShaderResourcePS(2);


}


void LambartMaterial::LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial)
{

	// Lambertにダウンキャスト
	FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)fbxmaterial;

	//ディフューズ
	FbxDouble3 fbxdiffuse = lambert->Diffuse;

	diffuse.x = (float)fbxdiffuse[0];
	diffuse.y = (float)fbxdiffuse[1];
	diffuse.z = (float)fbxdiffuse[2];


	FbxDouble3 fbxambient = lambert->Ambient;

	ambient.x = (float)fbxambient[0];
	ambient.y = (float)fbxambient[1];
	ambient.z = (float)fbxambient[2];


	// 透過度
	FbxDouble transparency = lambert->TransparencyFactor;
	diffuse.w = (float)transparency;



	this->noDiffuseTex = true;
	this->noNormalTex = true;
	this->noSpecularTex = true;


	// プロパティ取得。
	const FbxProperty property = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sDiffuse    // const char* pName
	);                                  // bool pCaseSensitive = true



	// プロパティが持っているレイヤードテクスチャの枚数をチェック
	int layerNum = property.GetSrcObjectCount<FbxFileTexture>();

	if (layerNum > 0)
	{
		FbxFileTexture* pFileTexture = property.GetSrcObject<FbxFileTexture>(0);

		FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTexture->GetTextureUse());


		if (m_type == FbxFileTexture::ETextureUse::eStandard)
		{
			const char* fileName = pFileTexture->GetFileName();
			string path;
			string basePath = "data/TEXTURE";

			// 最後に / または \\ が出てくる場所
			const char* lastSlash = strrchr(fileName, '/');
			const char* lastBackslash = strrchr(fileName, '\\');

			const char* last = max(lastSlash, lastBackslash);

			if (last == nullptr)
			{
				path = basePath + "/" + fileName;
			}
			else
			{
				path = basePath + "/" + (last + 1); // 最後の / または \ の次の文字から
			}

			this->LoadDiffuseTex(path.c_str());

			// マテリアル設定
			noDiffuseTex = false;
		}

	}

	//this->noDiffuseTex = true;
	//this->noNormalTex = true;
	//this->noArmTex = true;



	// プロパティ取得。
	const FbxProperty propertybump = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sBump    // const char* pName
	);                                  // bool pCaseSensitive = true




	// プロパティが持っているレイヤードテクスチャの枚数をチェック
	layerNum = propertybump.GetSrcObjectCount<FbxFileTexture>();

	if (layerNum > 0)
	{
		FbxFileTexture* pFileTextureBump = propertybump.GetSrcObject<FbxFileTexture>(0);

		FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTextureBump->GetTextureUse());


		if (m_type == FbxFileTexture::ETextureUse::eStandard)
		{

			const char* fileName = pFileTextureBump->GetFileName();
			string path;
			string basePath = "data/TEXTURE";

			// 最後に / または \\ が出てくる場所
			const char* lastSlash = strrchr(fileName, '/');
			const char* lastBackslash = strrchr(fileName, '\\');

			const char* last = max(lastSlash, lastBackslash);

			if (last == nullptr)
			{
				path = basePath + "/" + fileName;
			}
			else
			{
				path = basePath + "/" + (last + 1); // 最後の / または \ の次の文字から
			}

			this->LoadNormalTex(path.c_str());


			noNormalTex = FALSE;

		}


	}
	else
	{
		// プロパティ取得。
		const FbxProperty propertynormal = fbxmaterial->FindProperty(
			FbxSurfaceMaterial::sNormalMap    // const char* pName
		);                                  // bool pCaseSensitive = true

	// プロパティが持っているレイヤードテクスチャの枚数をチェック
		layerNum = propertynormal.GetSrcObjectCount<FbxFileTexture>();

		if (layerNum > 0)
		{
			FbxFileTexture* pFileTextureNormal = propertynormal.GetSrcObject<FbxFileTexture>(0);

			FbxFileTexture::ETextureUse m_type = FbxFileTexture::ETextureUse(pFileTextureNormal->GetTextureUse());


			if (m_type == FbxFileTexture::ETextureUse::eStandard)
			{

				const char* fileName = pFileTextureNormal->GetFileName();
				string path;
				string basePath = "data/TEXTURE";

				// 最後に / または \\ が出てくる場所
				const char* lastSlash = strrchr(fileName, '/');
				const char* lastBackslash = strrchr(fileName, '\\');

				const char* last = max(lastSlash, lastBackslash);

				if (last == nullptr)
				{
					path = basePath + "/" + fileName;
				}
				else
				{
					path = basePath + "/" + (last + 1); // 最後の / または \ の次の文字から
				}

				this->LoadNormalTex(path.c_str());


				noNormalTex = FALSE;

			}


		}

	}




}
