

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
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noDiffuseTex;
	int			noNormalTex;
	int			noArmTex;
	int			phong;
    int         dummy[3];
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
    float4      Position[5];
    float4      Direction[5];

	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
    float4		Intensity[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte境界用
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte境界用
};

// フォグ用バッファ
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};

// 縁取り用バッファ
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
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

cbuffer ShadowBuffer : register(b8)
{
    SHADOW Shadow;
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

cbuffer TessellationBuffer : register(b10)
{
    float tessellationFactor;
    float3 dummy;
};


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VS_Main( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,
                          in float4 inTangent       : TANGENT0,
                          in float4 inBiNoramal    : BINORMAL0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
						  out float4 outPosSM		: POSITION1,
						  out float4 outTangent     : TANGENT0,
						  out float4 outBiNormal    : BINORMAL0
)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));
	outTangent = normalize(mul(float4(inTangent.xyz, 0.0f), World));
	outBiNormal = normalize(mul(float4(inBiNoramal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;
	
	
	 //頂点座標　モデル座標系→透視座標系(シャドウマップ)
    matrix SMWorldViewProj = mul(World, Shadow.wvp);
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    float4 pos4 = mul(inPosition, SMWorldViewProj);
    pos4.xyz = pos4.xyz / pos4.w;
    outPosSM.x = (pos4.x + 1.0) / 2.0;
    outPosSM.y = (-pos4.y + 1.0) / 2.0;
    outPosSM.z = pos4.z;

	
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture       : register(t0);
Texture2D		NormalTex       : register(t1);
Texture2D       ShadowMap       : register(t2);
Texture2D       ShadowMapTex    : register(t3);
Texture2D       armTex           : register(t4);
Texture2D       heightMap           : register(t5);
SamplerState	smpWrap : register( s0 );
SamplerState    smpBorder : register(s1);




//テッセレーション
//ハルシェーダー
struct HS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
    float4 PosSM : POSITION1;
    float4 Tangent : TANGENT0;
    float4 BiNormal : BINORMAL0;
    
};

struct HS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
    float4 PosSM : POSITION1;
    float4 Tangent : TANGENT0;
    float4 BiNormal : BINORMAL0;
};

HS_OUTPUT HS_ControlPoint(HS_INPUT input)
{
    HS_OUTPUT output;
    output.Position = input.Position;
    output.Normal = input.Normal;
    output.TexCoord = input.TexCoord;
    output.Diffuse = input.Diffuse;
    output.WorldPos = input.WorldPos;
    output.PosSM = input.PosSM;
    output.Tangent = input.Tangent;
    output.BiNormal = input.BiNormal;
    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HS_PatchConstants")]
HS_OUTPUT HS_Main(InputPatch<HS_INPUT, 4> patch, uint id : SV_OutputControlPointID)
{
    return HS_ControlPoint(patch[id]);
}

struct HS_CONSTANT_DATA
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

HS_CONSTANT_DATA HS_PatchConstants(InputPatch<HS_INPUT, 4> patch)
{
    HS_CONSTANT_DATA output;
    output.edges[0] = tessellationFactor;
    output.edges[1] = tessellationFactor;
    output.edges[2] = tessellationFactor;
    output.edges[3] = tessellationFactor;
    output.inside[0] = tessellationFactor;
    output.inside[1] = tessellationFactor; // 必ず全ての要素を初期化
    return output;
}


struct DS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
    float4 PosSM : POSITION1;
    float4 Tangent : TANGENT0;
    float4 BiNormal : BINORMAL0;
};

[domain("quad")]
DS_OUTPUT DS_Main(HS_CONSTANT_DATA input, float2 uv : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> patch)
{
    DS_OUTPUT output;
    float3 pos = patch[0].Position * (1.0f - uv.x) * (1.0f - uv.y) +
                 patch[1].Position * uv.x * (1.0f - uv.y) +
                 patch[2].Position * uv.x * uv.y +
                 patch[3].Position * (1.0f - uv.x) * uv.y;

    
    float3 wpos = patch[0].WorldPos * (1.0f - uv.x) * (1.0f - uv.y) +
                 patch[1].WorldPos * uv.x * (1.0f - uv.y) +
                 patch[2].WorldPos * uv.x * uv.y +
                 patch[3].WorldPos * (1.0f - uv.x) * uv.y;


    
    
    // Heightmapのデータを使用して高さを調整
    float height = heightMap.SampleLevel(smpWrap, uv, 0);
    pos.y += height;
    

    output.Position = float4(pos, 1.0f);
        
    float4 nor = (patch[0].Normal + patch[1].Normal + patch[2].Normal + patch[3].Normal) / 4.0f;
    
    nor = normalize(nor);
    output.Normal = nor;
    output.TexCoord = uv;
    output.Diffuse = patch[0].Diffuse; // 必要に応じて補間することも可能
    output.WorldPos = wpos; // 必要に応じて補間することも可能
    output.PosSM = patch[0].PosSM; // 必要に応じて補間することも可能
    output.Tangent = patch[0].Tangent; // 必要に応じて補間することも可能
    output.BiNormal = patch[0].BiNormal; // 必要に応じて補間することも可能

    return output;
}
//=============================================================================
// ピクセルシェーダ
//=============================================================================


float GetVarianceDirectionalShadowFactor(float4 shadowCoord)
{
    
    float2 depth = ShadowMapTex.Sample(smpBorder, shadowCoord.xy).xy;
    float depth_sq = depth.x * depth.x; // E(x)^2
    float variance = depth.y - depth_sq; // σ^2 = E(x^2) - E(x^2)
    variance = saturate(variance + 0.0001); // 0.0001を追加して安定性を向上

    float fragDepth = shadowCoord.z;
    float md = fragDepth - depth.x; // t - μ
    float p = variance / (variance + (md * md)); // σ^2 / (σ^2 + (t - μ)^2)

    return saturate(max(p, fragDepth <= depth.x)); // P(x >= t)を満たすときのみ
}

float VSM_Filter(float2 texcoord, float fragDepth)
{

    float4 depth = ShadowMapTex.Sample(smpBorder, texcoord);
  

    float depth_sq = depth.x * depth.x;

    float variance = depth.y - depth_sq;
    variance = min(1.0f, max(0.0f, variance + 0.0001f));

    float md = fragDepth - depth.x;
    float p = variance / (variance + (md * md));
  
    return saturate(max(p, depth.x <= fragDepth));
}

void PS_Main(in float4 inPosition : SV_POSITION,
						 in float4 inNormal : NORMAL0,
						 in float2 inTexCoord : TEXCOORD0,
						 in float4 inDiffuse : COLOR0,
						 in float4 inWorldPos : POSITION0,
						 in float4 inPosSM : POSITION1,
						 in float4 inTangent : TANGENT0,
						 in float4 inBiNormal : BINORMAL0,

						 out float4 outDiffuse : SV_Target)
{
    float4 color;

    float sma = 1.0;
    bool shadow;
    
    
    float4 normal = inNormal;
    
    if (Material.noNormalTex == 0)
    {
        float3 normalMap = NormalTex.Sample(smpWrap, inTexCoord).rgb;
        
        normalMap.x = 1.0 - normalMap.x;
        normalMap.y = 1.0 - normalMap.y;
       
        normalMap = normalize(normalMap * 2.0 - 1.0);
        
        
        
        float3x3 TBN = float3x3(inTangent.xyz, inBiNormal.xyz, inNormal.xyz);
        normal.xyz = mul(normalMap, TBN);
        
        
    }
    
    
    
    
    
    					//影
    if (Shadow.enable == 1)
    {

		
        if (inPosSM.z > 1.0)
        {
            sma = 1.0;
        }
        else if (Shadow.mode == 0)
        {
            float sm0 = ShadowMap.Sample(smpBorder, inPosSM.xy);

            
            if (inPosSM.z - 0.0002 > sm0)
            {
                sma = 0.5;


            }
            
            
            
        }
        else if (Shadow.mode == 1)
        {
            sma = GetVarianceDirectionalShadowFactor(inPosSM);
        
 
        
            if (sma < 0.99f)
            {
                sma = sma * sma;

            }

        }
        
 

    }
    else
    {
        sma = 1.0;
    }

	
    if (Material.noDiffuseTex == 0)
    {
        color = g_Texture.Sample(smpWrap, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        color = inDiffuse;
    }

    float alpha = color.a;
    
    if (Light.Enable == 0)
    {
        color = color * Material.Diffuse * sma;
    }
    else
    {
        float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

        if (Material.phong == false)
        {
            for (int i = 0; i < 5; i++)
            {
                float3 lightDir;
                float light;

                if (Light.Flags[i].y == 1)
                {
                    if (Light.Flags[i].x == 1)
                    {
                        lightDir = normalize(Light.Direction[i].xyz);
                        light = dot(lightDir, normal.xyz);

                        light = (0.5 - 0.5 * light) * sma;
                        tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
                    }
                    else if (Light.Flags[i].x == 2)
                    {
                        lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
                        light = dot(lightDir, normal.xyz);

                        tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

                        float distance = length(inWorldPos - Light.Position[i]);

                        float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
                        tempColor *= att;
                    }
                    else
                    {
                        tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
                    }

                    outColor += tempColor;
                }
            }

        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                float3 lightDir;
                float light;
                float3 iA;
                float3 iD;
                float3 iS;
                
                if (Light.Flags[i].y == 1)
                {
                    lightDir = normalize(Light.Direction[i].xyz);
                    light = dot(lightDir, normal.xyz);
                    light = (0.5 - 0.5 * light);

                    if (Light.Flags[i].x == 1)
                    {
           

                        float3 r = 2.0 * normal.xyz * light - lightDir;

                        float3 v = normalize(Camera.xyz - inWorldPos.xyz);
                        
                        
                        iA = color.xyz * Material.Ambient.xyz * Light.Ambient[i].xyz;
                        
                        
                        
                        iD = color * Material.Diffuse * light * Light.Diffuse[i];
                        iS = color.xyz * pow(saturate(dot(r, v)), Material.Shininess) * Material.Specular.xyz;

                        if (light > 0.5)
                        {
                            iD *= sma;
                        }
                        

                       
                        
                        
                        tempColor = float4(saturate((iA + iD + iS)), 1.0f);

                    }
                    else
                    {
                        tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
                    }

                    outColor += tempColor;
                }

            }

        }
		

        color = outColor;
        color.a = alpha;
    }

    //color.xyz = normal.xyz;
    
	//フォグ
    if (Fog.Enable == 1)
    {
        float z = inPosition.z * inPosition.w;
        float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
        f = saturate(f);
        outDiffuse = f * color + (1 - f) * Fog.FogColor;
        outDiffuse.a = color.a;
    }
    else
    {
        outDiffuse = color;
    }

}

