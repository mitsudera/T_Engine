#include "PhongMaterial.h"
#include "PhongShader.h"
#include "DX11Texture.h"
#include "AssetsManager.h"

PhongMaterial::PhongMaterial(AssetsManager* assetsManager)
{
	this->pAssetsManager = assetsManager;
	this->pPhongShader = assetsManager->GetPhongShader();
	this->pShader = assetsManager->GetPhongShader();

}

PhongMaterial::PhongMaterial(PhongMaterial* phong)
{
	this->pAssetsManager = phong->pAssetsManager;

	this->pShader = phong->pShader;
	this->pPhongShader = phong->pPhongShader;

	this->diffuse = phong->diffuse;
	this->ambient = phong->ambient;
	this->emission = phong->emission;
	this->specular = phong->specular;
	this->shininess = phong->shininess;
	this->noDiffuseTex = phong->noDiffuseTex;
	this->noNormalTex = phong->noNormalTex;
	this->noSpecularTex = phong->noSpecularTex;
	this->textureDiffuse = phong->textureDiffuse;
	this->textureNormal = phong->textureNormal;
	this->textureSpeculer = phong->textureSpeculer;

}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::SetBufferMaterial(void)
{
	PhongShader::MaterialCBuffer mCBuffer;
	mCBuffer.ambient = this->ambient;
	mCBuffer.diffuse = this->diffuse;
	mCBuffer.emission = this->emission;
	mCBuffer.shininess = this->shininess;
	mCBuffer.specular = this->specular;
	mCBuffer.noDiffuseTex = this->noDiffuseTex;
	mCBuffer.noNormalTex = this->noNormalTex;
	mCBuffer.noArmTex = this->noSpecularTex;
	this->pPhongShader->SetMaterialCbuffer(mCBuffer);

	if (!noDiffuseTex) textureDiffuse->SetShaderResourcePS(0);
	if (!noNormalTex)textureNormal->SetShaderResourcePS(1);
	if (!noSpecularTex) textureSpeculer->SetShaderResourcePS(2);


}



void PhongMaterial::LoadFbxMaterial(FbxSurfaceMaterial* fbxmaterial)
{

	// Phongにダウンキャスト
	FbxSurfacePhong* phong = (FbxSurfacePhong*)fbxmaterial;
	//アンビエント
	FbxDouble3 amb = phong->Ambient;

	ambient.x = (float)amb[0];
	ambient.y = (float)amb[1];
	ambient.z = (float)amb[2];

	//ディフューズ
	FbxDouble3 dif = phong->Diffuse;

	diffuse.x = (float)dif[0];
	diffuse.y = (float)dif[1];
	diffuse.z = (float)dif[2];

	// 透過度
	FbxDouble transparency = phong->TransparencyFactor;
	diffuse.w = (float)transparency;

	//エミッション
	FbxDouble3 emi = phong->Emissive;

	emission.x = (float)emi[0];
	emission.y = (float)emi[1];
	emission.z = (float)emi[2];

	//スペキュラー
	FbxDouble3 spe = phong->Specular;

	specular.x = (float)spe[0];
	specular.y = (float)spe[1];
	specular.z = (float)spe[2];

	FbxDouble shine = phong->Shininess;
	shininess = (float)shine;

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
