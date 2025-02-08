

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Diffuse;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
    int         dummy;
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
    float4 Position;
    float4 Direction;

    float4 Diffuse;
    float4 Ambient;
    float4 Attenuation;
    float4 Intensity;
    int Flags;
    int Enable;
    int Dummy[2]; //16byte境界用
};

#define MAX_LIGHT (8)


cbuffer LightBuffer : register(b4)
{
    LIGHT Light[MAX_LIGHT];
    
    int LightEnable;
    int Dummy[3]; //16byte境界用

    
}


cbuffer CameraBuffer : register(b5)
{
	float4 Camera;
}

struct SHADOW
{
    matrix wvp;
    matrix view;
    matrix proj;
    int enable;
    int mode;
    int2 dummy;
};

cbuffer ShadowBuffer : register(b6)
{
    SHADOW Shadow;
}






//=============================================================================
// 頂点シェーダ
//=============================================================================
void VSmain( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,
                          in float4 inTangent       : TANGENT0,
                          in float4 inBiNoramal    : BINORMAL0,

						  out float4 outPosition	: SV_POSITION,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0
)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);
    //outPosition.z = 1.0f;

	outTexCoord = inTexCoord;
	
    outDiffuse = inDiffuse;

	
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		DiffuseTexture       : register(t0);
Texture2D		NormalTex       : register(t1);
Texture2D       armTex           : register(t2);
Texture2D       ShadowMap       : register(t3);
Texture2D       ShadowMapTex    : register(t4);

SamplerState	WrapSampler : register( s0 );
SamplerState BorderSampler : register(s1);


//=============================================================================
// ピクセルシェーダ
//=============================================================================



void PSmain(in float4 inPosition : SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inTangent		: TANGENT0,
						 in  float4 inBiNormal		: BINORMAL0,

						 out float4 outDiffuse		: SV_Target )
{
    float4 color = DiffuseTexture.Sample(WrapSampler, inTexCoord);

    //color *= inDiffuse;
    
    //outDiffuse = color;
    outDiffuse = color;
}

