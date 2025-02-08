

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    int noDiffuseTex;
    int noNormalTex;
    int noArmTex;
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}



struct GAUSSIAN
{
    float4 weight1;
    float4 weight2;

};

cbuffer GaussianBuffer : register(b9)
{
    GAUSSIAN gaus;

}




//=============================================================================
// 頂点シェーダ
//=============================================================================


Texture2D Texture : register(t0);
SamplerState smpWrap : register(s0);
SamplerState smpBorder : register(s1);

void VS_SM(
		in float4 inPosition : POSITION0,
		in float4 inNormal : NORMAL0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inTangent : TANGENT0,
		in float4 inBiNoramal : BINORMAL0,

        
        out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0
)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);


    outTexCoord = inTexCoord;


    outDiffuse = inDiffuse;
}


//シャドウマップ用ピクセルシェーダー
void PS_SM(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,


		out float4 outDiffuse : SV_Target
		)
{
    float4 color;
    if (Material.noDiffuseTex == 0)
    {
        color = Texture.Sample(smpWrap, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        
        color = inDiffuse;
    }
    
    color = color * Material.Diffuse;

    //color.r = 0.0f;
    color.r = inPosition.z;
    color.g = color.r * color.r;
    color.b = 0.0f;
    
    outDiffuse = color;
}





//const float sValue = 0.8f;

void xpass(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		

		out float4 outDiffuse : SV_Target)
{
    float x;
    float y;
    Texture.GetDimensions(x, y);
    float4 col;
    col = gaus.weight1.x * Texture.Sample(smpWrap, float2(inTexCoord));
    col += gaus.weight1.y * (Texture.Sample(smpWrap, inTexCoord + float2(+1.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-1.0f  / x, 0)));
    col += gaus.weight1.z * (Texture.Sample(smpWrap, inTexCoord + float2(+3.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-3.0f  / x, 0)));
    col += gaus.weight1.w * (Texture.Sample(smpWrap, inTexCoord + float2(+5.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-5.0f  / x, 0)));
    col += gaus.weight2.x * (Texture.Sample(smpWrap, inTexCoord + float2(+7.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-7.0f  / x, 0)));
    col += gaus.weight2.y * (Texture.Sample(smpWrap, inTexCoord + float2(+9.0f  / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-9.0f  / x, 0)));
    col += gaus.weight2.z * (Texture.Sample(smpWrap, inTexCoord + float2(+11.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-11.0f / x, 0)));
    col += gaus.weight2.w * (Texture.Sample(smpWrap, inTexCoord + float2(+13.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-13.0f / x, 0)));


    
    
    outDiffuse = col;


}
void ypass(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		

		out float4 outDiffuse : SV_Target)
{
    float x;
    float y;
    Texture.GetDimensions(x, y);
    float4 col;
    col = gaus.weight1.x * Texture.Sample(smpWrap, float2(inTexCoord));
    col += gaus.weight1.y * (Texture.Sample(smpWrap, inTexCoord + float2(0, +1.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -1.0f  / y)));
    col += gaus.weight1.z * (Texture.Sample(smpWrap, inTexCoord + float2(0, +3.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -3.0f  / y)));
    col += gaus.weight1.w * (Texture.Sample(smpWrap, inTexCoord + float2(0, +5.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -5.0f  / y)));
    col += gaus.weight2.x * (Texture.Sample(smpWrap, inTexCoord + float2(0, +7.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -7.0f  / y)));
    col += gaus.weight2.y * (Texture.Sample(smpWrap, inTexCoord + float2(0, +9.0f  / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -9.0f  / y)));
    col += gaus.weight2.z * (Texture.Sample(smpWrap, inTexCoord + float2(0, +11.0f / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -11.0f / y)));
    col += gaus.weight2.w * (Texture.Sample(smpWrap, inTexCoord + float2(0, +13.0f / y)) + Texture.Sample(smpWrap, inTexCoord + float2(0, -13.0f / y)));


    
    
    outDiffuse = col;


}