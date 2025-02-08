#include "DebugUtility.h"
#include "GameEngine.h"
#include "DebugLineShader.h"
#include "renderer.h"
#include "MeshData.h"
#include "AssetsManager.h"
#include "CBufferManager.h"
DebugUtility::DebugUtility(GameEngine* gameEngine)
{
    pGameEngine = gameEngine;
    pRenderer = pGameEngine->GetRenderer();

    debugLineShader = new DebugLineShader(pRenderer);
    cBufferManager = pGameEngine->GetCBufferManager();
    AssetsManager* assetsManager = pGameEngine->GetAssetsManager();

    sphereMeshData = assetsManager->LoadMeshFileFbx("Sphere.fbx")->GetChild()[0];
    capsuleMeshData1 = assetsManager->LoadMeshFileFbx("Capsule.fbx")->GetChild()[0];
    capsuleMeshData2 = assetsManager->LoadMeshFileFbx("Capsule.fbx")->GetChild()[1];
    capsuleMeshData3 = assetsManager->LoadMeshFileFbx("Capsule.fbx")->GetChild()[2];


    HRESULT hr;




    DebugLineVertex* vertArray = new DebugLineVertex[8];
    unsigned int* indexArray = new unsigned int[24];
    vertArray[0].pos = { -0.5f,0.5f,0.5f };
    vertArray[0].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[1].pos = { 0.5f,0.5f,0.5f };
    vertArray[1].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[2].pos = { 0.5f,0.5f,-0.5f };
    vertArray[2].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[3].pos = { -0.5f,0.5f,-0.5f };
    vertArray[3].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[4].pos = { -0.5f,-0.5f,0.5f };
    vertArray[4].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[5].pos = { 0.5f,-0.5f,0.5f };
    vertArray[5].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[6].pos = { 0.5f,-0.5f,-0.5f };
    vertArray[6].color = { 0.0f,1.0f,0.0f,1.0f };
    vertArray[7].pos = { -0.5f,-0.5f,-0.5f };
    vertArray[7].color = { 0.0f,1.0f,0.0f,1.0f };


    indexArray[0] = 0;
    indexArray[1] = 1;
    indexArray[2] = 1;
    indexArray[3] = 2;
    indexArray[4] = 2;
    indexArray[5] = 3;
    indexArray[6] = 3;
    indexArray[7] = 0;
    indexArray[8] = 4;
    indexArray[9] = 5;
    indexArray[10] = 5;
    indexArray[11] = 6;
    indexArray[12] = 6;
    indexArray[13] = 7;
    indexArray[14] = 7;
    indexArray[15] = 4;
    indexArray[16] = 0;
    indexArray[17] = 4;
    indexArray[18] = 1;
    indexArray[19] = 5;
    indexArray[20] = 2;
    indexArray[21] = 6;
    indexArray[22] = 3;
    indexArray[23] = 7;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(DebugLineVertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->vertexBox);

    if (FAILED(hr))
        return;

    // 頂点バッファへのポインタを取得
    D3D11_MAPPED_SUBRESOURCE msr;

    this->pRenderer->GetDeviceContext()->Map(this->vertexBox, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    DebugLineVertex* pVtx = (DebugLineVertex*)msr.pData;

    memcpy(pVtx, vertArray, sizeof(DebugLineVertex) * 8);

    this->pRenderer->GetDeviceContext()->Unmap(this->vertexBox, 0);


    // indexバッファ生成
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(unsigned int) * 24;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = this->pRenderer->GetDevice()->CreateBuffer(&bd, NULL, &this->indexBox);



    this->pRenderer->GetDeviceContext()->Map(this->indexBox, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    unsigned int* pIdx = (unsigned int*)msr.pData;

    memcpy(pIdx, indexArray, sizeof(unsigned int) * 24);

    this->pRenderer->GetDeviceContext()->Unmap(this->indexBox, 0);

    if (vertArray) delete[] vertArray;
    if (indexArray) delete[] indexArray;



}

DebugUtility::~DebugUtility()
{
	debugLineShader->Uninit();
	delete debugLineShader;

    if (vertexSphere) vertexSphere->Release();
    if (indexSphere)indexSphere->Release();
    if (vertexBox) vertexBox->Release();
    if (indexBox)indexBox->Release();

}

void DebugUtility::DrawDebugSphere(void)
{

    pRenderer->SetFillMode(FILL_MODE::FILL_MODE_WIREFRAME);

    sphereMeshData->BufferSetVertex();
    sphereMeshData->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(sphereMeshData->GetIndexNum(), 0, 0);

    pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

}

void DebugUtility::DrawDebugBox(void)
{
    // 頂点バッファ設定
    UINT stride = sizeof(DebugLineVertex);
    UINT worldOffset = 0;

    this->pRenderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBox, &stride, &worldOffset);
    this->pRenderer->GetDeviceContext()->IASetIndexBuffer(this->indexBox, DXGI_FORMAT_R32_UINT, 0);

    this->pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    this->pRenderer->GetDeviceContext()->DrawIndexed(24, 0, 0);

}

void DebugUtility::DrawDebugCapsule(XMFLOAT3 p, float length, float radius,XMMATRIX rot)
{
    pRenderer->SetFillMode(FILL_MODE::FILL_MODE_WIREFRAME);

    XMMATRIX pos, scl;
    XMMATRIX world;
    XMVECTOR pv = XMLoadFloat3(&p);
    scl = XMMatrixScaling(radius, length, radius);
    pos = XMMatrixTranslationFromVector(pv);
    world = scl * rot * pos;
    cBufferManager->SetWorldMtx(&world);


    capsuleMeshData1->BufferSetVertex();
    capsuleMeshData1->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(capsuleMeshData1->GetIndexNum(), 0, 0);

    scl = XMMatrixScaling(radius, radius, radius);
    XMVECTOR up = XMVector3Rotate(yonevec(), XMQuaternionRotationMatrix(rot)) * length * 0.5f;
    pos = XMMatrixTranslationFromVector(pv+up);
    world = scl * pos;
    cBufferManager->SetWorldMtx(&world);


    capsuleMeshData2->BufferSetVertex();
    capsuleMeshData2->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(capsuleMeshData2->GetIndexNum(), 0, 0);



    scl = XMMatrixScaling(radius, radius, radius);
    XMVECTOR down = XMVector3Rotate(-yonevec(), XMQuaternionRotationMatrix(rot)) * length * 0.5f;
    pos = XMMatrixTranslationFromVector(pv + down);
    world = scl * pos;
    cBufferManager->SetWorldMtx(&world);

    capsuleMeshData3->BufferSetVertex();
    capsuleMeshData3->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(capsuleMeshData3->GetIndexNum(), 0, 0);

    pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

}

void DebugUtility::DrawDebugCapsule(XMFLOAT3 sp, XMFLOAT3 ep, float radius,XMMATRIX world)
{
    pRenderer->SetFillMode(FILL_MODE::FILL_MODE_WIREFRAME);
    XMVECTOR spv = XMVector3Transform(XMLoadFloat3(&sp), world);
    XMVECTOR epv = XMVector3Transform(XMLoadFloat3(&ep), world);
    XMVECTOR lspv = XMLoadFloat3(&sp);
    XMVECTOR lepv = XMLoadFloat3(&ep);
    XMVECTOR pv = (lspv + lepv) * 0.5f;
    float len;
    XMStoreFloat(&len, XMVector3Length(epv - spv));

    XMMATRIX pos, scl;
    XMMATRIX mtx;
    scl = XMMatrixScaling(radius, len * 0.5f, radius);
    pos = XMMatrixTranslationFromVector(pv);
    mtx = scl *pos* world;
    cBufferManager->SetWorldMtx(&mtx);


    capsuleMeshData1->BufferSetVertex();
    capsuleMeshData1->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(capsuleMeshData1->GetIndexNum(), 0, 0);


    scl = XMMatrixScaling(radius, radius, radius);
    pos = XMMatrixTranslationFromVector(lepv);
    mtx = scl * pos * world;
    cBufferManager->SetWorldMtx(&mtx);

    capsuleMeshData2->BufferSetVertex();
    capsuleMeshData2->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(capsuleMeshData2->GetIndexNum(), 0, 0);


    scl = XMMatrixScaling(radius, radius, radius);
    pos = XMMatrixTranslationFromVector(lspv);
    mtx = scl * pos * world;
    cBufferManager->SetWorldMtx(&mtx);


    capsuleMeshData3->BufferSetVertex();
    capsuleMeshData3->BufferSetIndex();
    this->pRenderer->GetDeviceContext()->DrawIndexed(capsuleMeshData3->GetIndexNum(), 0, 0);

    pRenderer->SetCullingMode(CULL_MODE::CULL_MODE_BACK);

}

void DebugUtility::SetDebugLineShader(void)
{
    debugLineShader->SetShaderRenderer();
}
