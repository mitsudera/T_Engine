

//=============================================================================
// 頂点シェーダ
//=============================================================================
void VSmain(              in float4 inPosition : POSITION0,
						  in float4 inNormal : NORMAL0,
						  in float4 inDiffuse : COLOR0,
						  in float2 inTexCoord : TEXCOORD0,
                          in float4 inTangent : TANGENT0,
                          in float4 inBiNoramal : BINORMAL0,

						  out float4 outPosition : SV_POSITION,
						  out float2 outTexCoord : TEXCOORD0,
						  out float4 outDiffuse : COLOR0)
{
    outPosition = inPosition;

    outTexCoord = inTexCoord;
    outDiffuse = inDiffuse;
	
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D DiffuseTexture : register(t0);

SamplerState WrapSampler : register(s0);
SamplerState BorderSampler : register(s1);


//=============================================================================
// ピクセルシェーダ
//=============================================================================



void PSmain(             in float4 inPosition : SV_POSITION,
						 in float2 inTexCoord : TEXCOORD0,
						 in float4 inDiffuse : COLOR0,

						 out float4 outDiffuse : SV_Target)
{
    float4 color;

	
    color = DiffuseTexture.Sample(WrapSampler, inTexCoord);
    color *= inDiffuse;
	
    outDiffuse = color;
}
