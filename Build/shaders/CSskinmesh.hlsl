
#define BoneMax 64

cbuffer ConstantBuffer : register(b0)
{
    matrix BoneMatrices[BoneMax]; // ボーンの変換行列
}
cbuffer ConstantBuffer : register(b1)
{
    int boneNum;
    int dummy[3];
}

struct ControlPoint
{
    float3 pos; // 12バイト
    int showBoneNum; // 4バイト
    float weights[BoneMax]; // 4*256バイト
    int showBoneIndex[BoneMax]; // 4*256バイト
};
StructuredBuffer<ControlPoint> ControlPoints : register(t0);
RWStructuredBuffer<matrix> OutputPositions : register(u0);
// CSinput
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};



[numthreads(32, 1, 1)]
void CSFunc(uint3 DTid : SV_DispatchThreadID)
{

    ControlPoint cp = ControlPoints[DTid.x];
    float3 pos = cp.pos;
    float4 cpoint = float4(pos, 1.0f);

    matrix lasttrans = matrix(0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0);
    
    for (int k = 0; k < cp.showBoneNum; k++)
    {
        int n = cp.showBoneIndex[k];
        matrix transmtx = BoneMatrices[n];
            
        lasttrans += transmtx * cp.weights[n];
    }
    

    OutputPositions[DTid.x] = lasttrans;
}

