#include "SkinMeshTreeData.h"
#include "GameEngine.h"
#include "AssetsManager.h"
#include "DX11Texture.h"
#include "Material.h"
#include "LambartMaterial.h"
#include "PhongMaterial.h"
#include "ShaderSet.h"
#include "SkinMeshShadowMappingMaterial.h"
#include "renderer.h"
#include "SkinMeshPhongMaterial.h"

SkinMeshData::SkinMeshData(AssetsManager* p)
{
	pAssetsManager = p;
}

SkinMeshData::~SkinMeshData()
{
	this->indexBuffer->Release();
	delete[] vertexArray;
	delete[] cpIndexArray;
}


void SkinMeshData::LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree)
{

	this->nodeAttribute = Attribute::Mesh;

	using namespace fbxsdk;
	this->pAssetsManager = skinMeshTree->GetAssetsMnager();
	this->pRenderer = pAssetsManager->GetGameEngine()->GetRenderer();
	this->skinMeshTree = skinMeshTree;
	FbxMesh* mesh = node->GetMesh();
	name = mesh->GetNode()->GetName();
	fileName = skinMeshTree->GetFileName();
	this->parent = parent;

	int PolygonNum = mesh->GetPolygonCount();               //���|���S����
	if (PolygonNum == 0)
	{
		return;
	}

	int PolygonVertexNum = mesh->GetPolygonVertexCount();   //�|���S�����_�C���f�b�N�X��
	//int* indexArray = mesh->GetPolygonVertices();             //�|���S�����_�C���f�b�N�X�z��

	int controlNum = mesh->GetControlPointsCount();         // ���_���W��
	FbxVector4* src = mesh->GetControlPoints();             // ���_���W�z��
	CreateVertexArray(PolygonVertexNum);
	unsigned int indexnum = PolygonNum * 3;



	//���_�z��𖄂߂�
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);
	FbxVector2* uv = new FbxVector2[PolygonVertexNum];


	bool bIsUnmapped = false;
	this->boneNum = skinMeshTree->GetBoneCnt();


	CreateCpArray(controlNum);
	for (int i = 0; i < controlNum; i++)
	{
		this->cpArray[i].pos.x = (float)src[i][0];
		this->cpArray[i].pos.y = (float)src[i][1];
		this->cpArray[i].pos.z = (float)src[i][2];

		this->cpArray[i].weight = new float[this->boneNum];

		for (int j = 0; j < boneNum; j++)
		{
			cpArray[i].weight[j] = 0.0f;
		}
	}
	int dc = mesh->GetDeformerCount();
	FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0);
	this->clusterNum = skin->GetClusterCount();
	for (int i = 0; i < clusterNum; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);

		FbxAMatrix initmat;
		cluster->GetTransformLinkMatrix(initmat);

		string na = cluster->GetLink()->GetName();

		int boneIndex = this->skinMeshTree->GetBoneNumber(na);

		int pointNum = cluster->GetControlPointIndicesCount();
		int* pointAry = cluster->GetControlPointIndices();
		double* weightAry = cluster->GetControlPointWeights();

		for (int j = 0; j < pointNum; ++j)
		{


			// ���_�C���f�b�N�X�ƃE�F�C�g���擾
			int   index = pointAry[j];
			float weight = (float)weightAry[j];
			this->cpArray[index].weight[boneIndex] = weight;


		}


	}



	int vcnt = 0;

	for (int p = 0; p < PolygonNum; p++)
	{



		int IndexNumInPolygon = mesh->GetPolygonSize(p);  // p�Ԗڂ̃|���S���̒��_��



		if (IndexNumInPolygon == 3)
		{
			FbxVector4 positions[3];
			FbxVector4 normals[3];
			FbxVector2 uvs[3];

			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				FbxVector4 nor;

				mesh->GetPolygonVertexNormal(
					p,				// �|���S���ԍ�
					n,				// ���Ԗڂ̒��_
					nor);			// �@����
				vertexArray[vcnt].Normal.x = (float)nor[0];
				vertexArray[vcnt].Normal.y = (float)nor[1];
				vertexArray[vcnt].Normal.z = (float)nor[2];

				normals[n] = nor;

				// UV�l�擾�B
				FbxString name = uvSetNameList.GetStringAt(0);
				mesh->GetPolygonVertexUV(p, n, name, uv[vcnt], bIsUnmapped);
				vertexArray[vcnt].TexCoord.x = (float)uv[vcnt][0];
				vertexArray[vcnt].TexCoord.y = 1.0f - (float)uv[vcnt][1];

				uvs[n] = uv[vcnt];


				//// �|���S��p���\������n�Ԗڂ̒��_�̃C���f�b�N�X�ԍ�
				int IndexNumber = mesh->GetPolygonVertex(p, n);
				vertexArray[vcnt].Position = cpArray[IndexNumber].pos;



				float boneWeight[4] = { 0.0f,0.0f,0.0f,0.0f };
				int boneIndex[4] = { -1,-1,-1,-1 };
				for (int i = 0; i < 4; i++)
				{
					float maxWeight = 0.001f;
					int index = -1;
					for (int j = 0; j < boneNum; j++)
					{
						if (j == boneIndex[0] || j == boneIndex[1] || j == boneIndex[2] || j == boneIndex[3])
							continue;

						if (maxWeight < cpArray[IndexNumber].weight[j])
						{
							maxWeight = cpArray[IndexNumber].weight[j];
							index = j;
						}

					}
					if (index != -1)
					{
						boneWeight[i] = maxWeight;
						boneIndex[i] = index;


					}
				}



				vertexArray[vcnt].BoneWeight.x = boneWeight[0];
				vertexArray[vcnt].BoneIndex.x = (unsigned int)boneIndex[0];

				vertexArray[vcnt].BoneWeight.y = boneWeight[1];
				vertexArray[vcnt].BoneIndex.y = (unsigned int)boneIndex[1];

				vertexArray[vcnt].BoneWeight.z = boneWeight[2];
				vertexArray[vcnt].BoneIndex.z = (unsigned int)boneIndex[2];

				vertexArray[vcnt].BoneWeight.w = boneWeight[3];
				vertexArray[vcnt].BoneIndex.w = (unsigned int)boneIndex[3];

				positions[n] = src[IndexNumber];

				this->cpIndexArray[vcnt] = IndexNumber;


				vertexArray[vcnt].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

				vcnt++;

			}


			// �^���W�F���g�x�N�g���̌v�Z
			FbxVector4 edge1 = positions[1] - positions[0];
			FbxVector4 edge2 = positions[2] - positions[0];
			FbxVector2 deltaUV1 = uvs[1] - uvs[0];
			FbxVector2 deltaUV2 = uvs[2] - uvs[0];

			float f = 1.0f / (float)(deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

			FbxVector4 tangent;
			tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
			tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
			tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
			tangent[3] = 0.0f;

			// �e���_�Ƀ^���W�F���g�x�N�g����ݒ�
			for (int n = 0; n < IndexNumInPolygon; n++)
			{
				vertexArray[vcnt - IndexNumInPolygon + n].Tangent.x = (float)tangent[0];
				vertexArray[vcnt - IndexNumInPolygon + n].Tangent.y = (float)tangent[1];
				vertexArray[vcnt - IndexNumInPolygon + n].Tangent.z = (float)tangent[2];
				vertexArray[vcnt - IndexNumInPolygon + n].Tangent = XMFLOAT3Normalize(vertexArray[vcnt - IndexNumInPolygon + n].Tangent);



				XMVECTOR nv = XMLoadFloat3(&vertexArray[vcnt - IndexNumInPolygon + n].Normal);
				XMVECTOR tv = XMLoadFloat3(&vertexArray[vcnt - IndexNumInPolygon + n].Tangent);

				XMVECTOR binv = XMVector3Cross(nv, tv);
				binv = XMVector3Normalize(binv);

				XMFLOAT3 binor;

				XMStoreFloat3(&binor, binv);

				vertexArray[vcnt - IndexNumInPolygon + n].BiNormal = binor;

			}

		}

	}

	//�C���f�b�N�X�z��𖄂߂�
	unsigned int* indexArray = nullptr;
	indexArray = new unsigned int[indexnum];
	int ic = 0;

	for (int i = 0; i < PolygonNum; i++)
	{

		indexArray[i * 3] = ic;
		ic++;
		indexArray[i * 3 + 1] = ic;
		ic++;
		indexArray[i * 3 + 2] = ic;
		ic++;
	}


	// �C���f�b�N�X�o�b�t�@����
	this->CreateIndexBuffer(indexnum);

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		this->pAssetsManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->Map(this->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned int* pIdx = (unsigned int*)msr.pData;


		for (unsigned int i = 0; i < indexnum; i++)
		{
			pIdx[i] = indexArray[i];
		}

		this->pAssetsManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->Unmap(this->indexBuffer, 0);
	}



	FbxMatrix worldOffset = node->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);//�I�t�Z�b�g�s��̎擾
	this->worldOffset = FbxMatrixConvertToXMMATRIX(worldOffset);

	FbxMatrix localOffset = node->EvaluateLocalTransform(FBXSDK_TIME_INFINITE);
	this->localOffset = FbxMatrixConvertToXMMATRIX(localOffset);

	FbxVector4 pos = node->EvaluateLocalTranslation(FBXSDK_TIME_INFINITE);
	FbxVector4 scl = node->EvaluateLocalScaling(FBXSDK_TIME_INFINITE);
	FbxVector4 rot = node->EvaluateLocalRotation(FBXSDK_TIME_INFINITE);

	this->posOffset.x = (float)pos[0];
	this->posOffset.y = (float)pos[1];
	this->posOffset.z = (float)pos[2];

	this->sclOffset.x = (float)scl[0];
	this->sclOffset.y = (float)scl[1];
	this->sclOffset.z = (float)scl[2];

	this->rotOffset.x = (float)rot[0];
	this->rotOffset.y = (float)rot[1];
	this->rotOffset.z = (float)rot[2];

	// �}�e���A���̐�

	int mtlcnt = node->GetMaterialCount();


	// �}�e���A�������擾
	for (int i = 0; i < mtlcnt; i++)
	{
		FbxSurfaceMaterial* fbxmaterial = node->GetMaterial(i);

		FbxClassId id = fbxmaterial->GetClassId();

		if (fbxmaterial != 0)
		{
			// �}�e���A�����
			// Lambert��Phong��
			//lambart



			if (id == (FbxSurfaceLambert::ClassId))
			{

				LambartMaterial* lambart = new LambartMaterial(this->pAssetsManager);

				lambart->LoadFbxMaterial(fbxmaterial);

				lambart->SetName(this->fileName + fbxmaterial->GetName());

				this->material = pAssetsManager->LoadMaterial(lambart);


			}
			//phong
			else if (id == (FbxSurfacePhong::ClassId))
			{

				SkinMeshPhongMaterial* phong = new SkinMeshPhongMaterial(this->pAssetsManager);

				phong->LoadFbxMaterial(fbxmaterial);

				phong->SetName(this->fileName + fbxmaterial->GetName());


				this->material = pAssetsManager->LoadMaterial(phong);


			}
			else
			{

			}
			SkinMeshShadowMappingMaterial* shadowMat = new SkinMeshShadowMappingMaterial(this->pAssetsManager);

			shadowMat->SetShadowMaterial(this->material);

			this->shadowMaterial = pAssetsManager->LoadMaterial(shadowMat);



		}
	}
	delete[] uv;
	delete[] indexArray;

	LoadChild(node);
}

void SkinMeshData::Destroy(void)
{
	SkinMeshTreeNode::Destroy();
	this->indexBuffer->Release();
	delete[] vertexArray;
	delete[] cpIndexArray;


}



ID3D11Buffer* SkinMeshData::GetIndexBuffer(void)
{
	return this->indexBuffer;
}

int SkinMeshData::GetCotrolPointNum(void)
{
	return this->cpNum;
}

CONTROLPOINT* SkinMeshData::GetControlPoint(void)
{
	return cpArray;
}

int SkinMeshData::GetVertNum(void)
{
	return this->vertNum;
}

SkinMeshVertex* SkinMeshData::GetVertexArray(void)
{
	return this->vertexArray;
}

int* SkinMeshData::GetCPIndexArray(void)
{
	return cpIndexArray;
}

SkinMeshTreeData* SkinMeshData::GetSkinMeshTree(void)
{
	return this->skinMeshTree;
}

void SkinMeshData::CreateVertexArray(int n)
{
	this->vertNum = n;
	this->cpIndexArray = new int[n];
	this->vertexArray = new SkinMeshVertex[n];
}

void SkinMeshData::CreateCpArray(int n)
{
	this->cpArray = new CONTROLPOINT[n];
	this->cpNum = n;

}

void SkinMeshData::CreateIndexBuffer(int n)
{
	this->indexNum = n;
	// �C���f�b�N�X�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * n;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->pAssetsManager->GetGameEngine()->GetRenderer()->GetDevice()->CreateBuffer(&bd, NULL, &this->indexBuffer);

}

void SkinMeshData::BufferSetIndex(void)
{

	// �v���~�e�B�u�g�|���W�ݒ�
	this->pAssetsManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	this->pAssetsManager->GetGameEngine()->GetRenderer()->GetDeviceContext()->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

}

int SkinMeshData::GetIndexNum(void)
{
	return this->indexNum;
}

Material* SkinMeshData::GetMaterial(void)
{
	return this->material;
}

Material* SkinMeshData::GetShadowMaterial(void)
{
	return this->shadowMaterial;
}

XMFLOAT3 SkinMeshData::GetPosOffset(void)
{
	return this->posOffset;
}

XMFLOAT3 SkinMeshData::GetSclOffset(void)
{
	return this->sclOffset;
}

XMFLOAT3 SkinMeshData::GetRotOffset(void)
{
	return this->rotOffset;
}



SkinMeshTreeData::SkinMeshTreeData(AssetsManager* p)
{
	pAssetsManager = p;
}

SkinMeshTreeData::~SkinMeshTreeData()
{
	delete[] linkMtxArray;

	for (SkinMeshTreeNode* node:nodeArray)
	{
		node->Destroy();
		delete node;
	}
	
}

void SkinMeshTreeData::LoadFbxFile(string fileName)
{
	this->fileName = fileName;
	this->name = fileName + "root";
	FbxManager* manager;
	FbxIOSettings* ioSettings;
	manager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	// Importer�𐶐�
	FbxImporter* importer = FbxImporter::Create(manager, "");

	if (importer->Initialize(fileName.c_str(), -1, manager->GetIOSettings()) == false)
	{
		// �C���|�[�g�G���[
	}

	//Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);
	importer->Destroy(); // �V�[���𗬂����񂾂�Importer�͉��

	this->boneCnt = scene->GetSrcObjectCount<FbxSkeleton>();
	this->linkMtxArray = new pair<XMMATRIX, string>[boneCnt];
	for (int i = 0; i < boneCnt; i++)
	{
		this->linkMtxArray[i].second = scene->GetSrcObject<FbxSkeleton>(i)->GetNode()->GetName();
	}

	FbxNode* root = scene->GetRootNode();
	this->skinCnt= scene->GetSrcObjectCount<FbxSkin>();
	clusterCnt = new int[skinCnt];
	for (int i = 0; i < skinCnt; i++)
	{
		FbxSkin* skin = scene->GetSrcObject<FbxSkin>(i);
		this->clusterCnt[i] = skin->GetClusterCount();

		for (int j = 0; j < clusterCnt[i]; j++)
		{
			FbxCluster* cluster = skin->GetCluster(j);

			FbxAMatrix initmat;
			cluster->GetTransformLinkMatrix(initmat);

			pair<XMMATRIX, string> linkMtx;

			linkMtx.second = cluster->GetLink()->GetName();

			linkMtx.first = FbxMatrixConvertToXMMATRIX(initmat);

			SetLinkMtx(linkMtx);
		}

	}


	for (int i = 0; i < root->GetChildCount(); i++)
	{
		FbxNode* child = root->GetChild(i);

		FbxNodeAttribute* atr = child->GetNodeAttribute();

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			SkinMeshData* childData = new SkinMeshData(pAssetsManager);
			childData->LoadNode(child, nullptr, this);
			nodeArray.push_back(childData);

		}
		else if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			BoneData* childData = new BoneData(pAssetsManager);
			childData->LoadNode(child, nullptr, this);
			nodeArray.push_back(childData);

		}
		else
		{
			NullData* childData = new NullData (pAssetsManager);
			childData->LoadNode(child, nullptr, this);
			nodeArray.push_back(childData);

		}




	}






	scene->Destroy();
	manager->Destroy();//fbx�t�@�C���̓ǂݍ��݂��I�������}�l�[�W���[�͉������


}

AssetsManager* SkinMeshTreeData::GetAssetsMnager(void)
{
	return this->pAssetsManager;
}

string SkinMeshTreeData::GetFileName(void)
{
	return fileName;
}


int SkinMeshTreeData::GetBoneCnt(void)
{
	return boneCnt;
}

vector<SkinMeshTreeNode*>& SkinMeshTreeData::GetNodeArray(void)
{
	return this->nodeArray;
}


XMMATRIX SkinMeshTreeData::GetInitMtx(string name)
{
	for (int i = 0; i < boneCnt; i++)
	{
		if (linkMtxArray[i].second == name)
		{
			return linkMtxArray[i].first;
		}
	}

	return XMMatrixIdentity();
}

void SkinMeshTreeData::SetLinkMtx(pair<XMMATRIX, string> link)
{
	for (int i = 0; i < boneCnt; i++)
	{
		if (linkMtxArray[i].second==link.second)
		{
			linkMtxArray[i].first = link.first;
		}
		
	}
}

int SkinMeshTreeData::GetBoneNumber(string name)
{
	for (int i = 0; i < boneCnt; i++)
	{
		if (linkMtxArray[i].second == name)
		{
			return i;
		}

	}
	return -1;
}

BoneData::BoneData(AssetsManager* p)
{ 
	this->pAssetsManager = p;
}

BoneData::~BoneData()
{
}


void BoneData::LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree)
{
	this->nodeAttribute = Attribute::Bone;

	using namespace fbxsdk;
	this->parent = parent;
	this->nodeAttribute = Attribute::Bone;
	this->skinMeshTree = skinMeshTree;
	FbxSkeleton* bone = node->GetSkeleton();

	this->name = node->GetName();

	//FbxMatrix worldOffset = node->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);//�I�t�Z�b�g�s��̎擾
	//this->worldOffset = FbxMatrixConvertToXMMATRIX(worldOffset);


	this->worldOffset = skinMeshTree->GetInitMtx(this->name);

	FbxMatrix localOffset = node->EvaluateLocalTransform(FBXSDK_TIME_INFINITE);
	this->localOffset = FbxMatrixConvertToXMMATRIX(localOffset);

	FbxVector4 pos = node->EvaluateLocalTranslation(FBXSDK_TIME_INFINITE);
	FbxVector4 scl = node->EvaluateLocalScaling(FBXSDK_TIME_INFINITE);
	FbxVector4 rot = node->EvaluateLocalRotation(FBXSDK_TIME_INFINITE);

	this->posOffset.x = (float)pos[0];
	this->posOffset.y = (float)pos[1];
	this->posOffset.z = (float)pos[2];

	this->sclOffset.x = (float)scl[0];
	this->sclOffset.y = (float)scl[1];
	this->sclOffset.z = (float)scl[2];

	this->rotOffset.x = (float)(rot[0] / 180.0) * XM_PI;
	this->rotOffset.y = (float)(rot[1] / 180.0) * XM_PI;
	this->rotOffset.z = (float)(rot[2] / 180.0) * XM_PI;





	this->LoadChild(node);

}




XMMATRIX BoneData::GetWorldOffset(void)
{
	return this->worldOffset;
}

XMMATRIX BoneData::GetLocalOffset(void)
{
	return this->localOffset;
}

XMFLOAT3 BoneData::GetPosOffset(void)
{
	return this->posOffset;
}

XMFLOAT3 BoneData::GetSclOffset(void)
{
	return this->sclOffset;
}

XMFLOAT3 BoneData::GetRotOffset(void)
{
	return this->rotOffset;
}

SkinMeshTreeNode::SkinMeshTreeNode()
{
}

SkinMeshTreeNode::~SkinMeshTreeNode()
{
}

void SkinMeshTreeNode::LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree)
{
	this->skinMeshTree = skinMeshTree;

	this->parent = parent;
	this->nodeAttribute = Attribute::Null;
	this->name = node->GetName();

	this->LoadChild(node);
}

void SkinMeshTreeNode::LoadChild(FbxNode* node)
{
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		FbxNode* child = node->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			SkinMeshData* childData = new SkinMeshData(pAssetsManager);
			childData->LoadNode(child, this, skinMeshTree);
			childArray.push_back(childData);

		}
		else if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			BoneData* childData = new BoneData(pAssetsManager);
			childData->LoadNode(child, this, skinMeshTree);
			childArray.push_back(childData);

		}
		else
		{
			NullData* childData = new NullData(pAssetsManager);
			childData->LoadNode(child, this, skinMeshTree);
			childArray.push_back(childData);

		}



	}

}


string SkinMeshTreeNode::GetName(void)
{
	return name;
}

vector<SkinMeshTreeNode*>& SkinMeshTreeNode::GetChildArray(void)
{
	return this->childArray;
}

SkinMeshTreeNode::Attribute SkinMeshTreeNode::GetAttribute(void)
{
	return this->nodeAttribute;
}

void SkinMeshTreeNode::Destroy(void)
{
	for (SkinMeshTreeNode* child : childArray)
	{
		child->Destroy();
		delete child;
	}
}

SkinMeshTreeNode* SkinMeshTreeNode::GetParent(void)
{
	return parent;
}

NullData::NullData(AssetsManager* p)
{
	pAssetsManager = p;
}

NullData::~NullData()
{
}

void NullData::LoadNode(FbxNode* node, SkinMeshTreeNode* parent, SkinMeshTreeData* skinMeshTree)
{
	this->parent = parent;
	this->nodeAttribute = Attribute::Null;
	using namespace fbxsdk;

	this->skinMeshTree = skinMeshTree;

	this->name = node->GetName();

	this->LoadChild(node);

}
