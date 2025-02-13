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

	// Phong�Ƀ_�E���L���X�g
	FbxSurfacePhong* phong = (FbxSurfacePhong*)fbxmaterial;
	//�A���r�G���g
	FbxDouble3 amb = phong->Ambient;

	ambient.x = (float)amb[0];
	ambient.y = (float)amb[1];
	ambient.z = (float)amb[2];

	//�f�B�t���[�Y
	FbxDouble3 dif = phong->Diffuse;

	diffuse.x = (float)dif[0];
	diffuse.y = (float)dif[1];
	diffuse.z = (float)dif[2];

	// ���ߓx
	FbxDouble transparency = phong->TransparencyFactor;
	diffuse.w = (float)transparency;

	//�G�~�b�V����
	FbxDouble3 emi = phong->Emissive;

	emission.x = (float)emi[0];
	emission.y = (float)emi[1];
	emission.z = (float)emi[2];

	//�X�y�L�����[
	FbxDouble3 spe = phong->Specular;

	specular.x = (float)spe[0];
	specular.y = (float)spe[1];
	specular.z = (float)spe[2];

	FbxDouble shine = phong->Shininess;
	shininess = (float)shine;

	this->noDiffuseTex = true;
	this->noNormalTex = true;
	this->noSpecularTex = true;


	// �v���p�e�B�擾�B
	const FbxProperty property = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sDiffuse    // const char* pName
	);                                  // bool pCaseSensitive = true



	// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
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

			// �Ō�� / �܂��� \\ ���o�Ă���ꏊ
			const char* lastSlash = strrchr(fileName, '/');
			const char* lastBackslash = strrchr(fileName, '\\');

			const char* last = max(lastSlash, lastBackslash);

			if (last == nullptr)
			{
				path = basePath + "/" + fileName;
			}
			else
			{
				path = basePath + "/" + (last + 1); // �Ō�� / �܂��� \ �̎��̕�������
			}

			this->LoadDiffuseTex(path.c_str());

			// �}�e���A���ݒ�
			noDiffuseTex = false;
		}

	}



	// �v���p�e�B�擾�B
	const FbxProperty propertybump = fbxmaterial->FindProperty(
		FbxSurfaceMaterial::sBump    // const char* pName
	);                                  // bool pCaseSensitive = true




	// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
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

			// �Ō�� / �܂��� \\ ���o�Ă���ꏊ
			const char* lastSlash = strrchr(fileName, '/');
			const char* lastBackslash = strrchr(fileName, '\\');

			const char* last = max(lastSlash, lastBackslash);

			if (last == nullptr)
			{
				path = basePath + "/" + fileName;
			}
			else
			{
				path = basePath + "/" + (last + 1); // �Ō�� / �܂��� \ �̎��̕�������
			}

			this->LoadNormalTex(path.c_str());


			noNormalTex = FALSE;

		}


	}
	else
	{
		// �v���p�e�B�擾�B
		const FbxProperty propertynormal = fbxmaterial->FindProperty(
			FbxSurfaceMaterial::sNormalMap    // const char* pName
		);                                  // bool pCaseSensitive = true

	// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
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

				// �Ō�� / �܂��� \\ ���o�Ă���ꏊ
				const char* lastSlash = strrchr(fileName, '/');
				const char* lastBackslash = strrchr(fileName, '\\');

				const char* last = max(lastSlash, lastBackslash);

				if (last == nullptr)
				{
					path = basePath + "/" + fileName;
				}
				else
				{
					path = basePath + "/" + (last + 1); // �Ō�� / �܂��� \ �̎��̕�������
				}

				this->LoadNormalTex(path.c_str());


				noNormalTex = FALSE;

			}


		}

	}



}
