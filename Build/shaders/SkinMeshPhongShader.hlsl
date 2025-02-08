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


#define MAX_DIREC_LIGHT (4)
#define MAX_POINT_LIGHT (8)

// ライト用バッファ
struct DIREC_LIGHT
{
    float4 m_Direction[MAX_DIREC_LIGHT]; // ライトの方向
    float4 m_Diffuse[MAX_DIREC_LIGHT]; // 拡散光の色
    float4 m_Ambient[MAX_DIREC_LIGHT]; // 環境光の色
    int4 m_Enable[MAX_DIREC_LIGHT];
    int4 allEnable;

};


cbuffer DirecLightBuffer : register(b4)
{
    DIREC_LIGHT direcLight;
    
    
}
// ライト用定数バッファ構造体
struct POINT_LIGHT
{
    float4 m_Position[MAX_POINT_LIGHT]; // ライトの位置
    float4 m_Diffuse[MAX_POINT_LIGHT]; // 拡散光の色
    float4 m_Ambient[MAX_POINT_LIGHT]; // 環境光の色
    float4 m_Attenuation[MAX_POINT_LIGHT]; // 減衰率    
    float4 m_intensity[MAX_POINT_LIGHT]; // ライトの強度
    int4 m_Enable[MAX_POINT_LIGHT];
    int4 allEnable;
};
cbuffer PointLightBuffer : register(b5)
{
    POINT_LIGHT pointLight;
    
    
}


cbuffer CameraBuffer : register(b6)
{
    float4 Camera;
}

struct SHADOW
{
    matrix wvp;
    matrix wvpf;
    int enable;
    int mode;
    float facter;
    float bias;
};

cbuffer ShadowBuffer : register(b7)
{
    SHADOW Shadow;
}

#define BoneMax 256
cbuffer BoneBuffer : register(b8)
{
    matrix boneMtx[BoneMax];
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VSmain(in float4 inPosition : POSITION0,
						  in float4 inNormal : NORMAL0,
						  in float4 inDiffuse : COLOR0,
						  in float2 inTexCoord : TEXCOORD0,
                          in float4 inTangent : TANGENT0,
                          in float4 inBiNoramal : BINORMAL0,
                          in float4 inBoneWeight : BONEWEIGHT,
                          in uint4 inBoneIndex : BONEINDEX,

						  out float4 outPosition : SV_POSITION,
						  out float4 outNormal : NORMAL0,
						  out float2 outTexCoord : TEXCOORD0,
						  out float4 outDiffuse : COLOR0,
						  out float4 outWorldPos : POSITION0,
						  out float4 outTangent : TANGENT0,
						  out float4 outBiNormal : BINORMAL0
)
{
    
    matrix World = matrix(
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0);
    
    for (int i = 0; i < 4;i++)
    {
        if (inBoneWeight[i]==0.0f)
            continue;
        
        World += boneMtx[inBoneIndex[i]] * inBoneWeight[i];
    }
    
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
	
	



}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D DiffuseTexture : register(t0);
Texture2D NormalTex : register(t1);
Texture2D armTex : register(t2);
Texture2D ShadowMapNear : register(t3);
Texture2D ShadowMapFar : register(t4);

SamplerState WrapSampler : register(s0);
SamplerState BorderSampler : register(s1);


//=============================================================================
// ピクセルシェーダ
//=============================================================================


float GetVarianceDirectionalShadowFactor(float4 shadowCoord)
{
    
    float2 depth = ShadowMapNear.Sample(BorderSampler, shadowCoord.xy).xy;
    float depth_sq = depth.x * depth.x; // E(x)^2
    float variance = depth.y - depth_sq; // σ^2 = E(x^2) - E(x^2)
    variance = saturate(variance + Shadow.facter + Shadow.facter); // facterを追加して安定性を向上

    float fragDepth = shadowCoord.z + Shadow.bias;
    float md = fragDepth - depth.x; // t - μ
    float p = variance / (variance + (md * md)); // σ^2 / (σ^2 + (t - μ)^2)

    return saturate(max(p, fragDepth <= depth.x)); // P(x >= t)を満たすときのみ
}


void PSmain(in float4 inPosition : SV_POSITION,
						 in float4 inNormal : NORMAL0,
						 in float2 inTexCoord : TEXCOORD0,
						 in float4 inDiffuse : COLOR0,
						 in float4 inWorldPos : POSITION0,
						 in float4 inTangent : TANGENT0,
						 in float4 inBiNormal : BINORMAL0,

						 out float4 outDiffuse : SV_Target)
{
    float4 color;

    float sma = 1.0;
    bool shadow;
    
    
    float4 normal = inNormal;
    
    float4 pos4 = mul(inWorldPos, Shadow.wvp);
    float4 PosSM;
    PosSM.x = (pos4.x + 1.0) / 2.0;
    PosSM.y = (-pos4.y + 1.0) / 2.0;
    PosSM.z = pos4.z;
    PosSM.w = 1.0f;
    

    
    if (Material.noNormalTex == 0)
    {
        // Sample the normal map
        float3 normalMap = NormalTex.Sample(WrapSampler, inTexCoord).rgb;
        
        normalMap.x = 1.0 - normalMap.x;
        normalMap.y = 1.0 - normalMap.y;
        //normalMap.z = 1.0 - normalMap.z;
       
        normalMap = normalize(normalMap * 2.0 - 1.0);
        
        
        
        float3x3 TBN = float3x3(inTangent.xyz, inBiNormal.xyz, inNormal.xyz);
        normal.xyz = mul(normalMap, TBN);
        
        
    }
    
    					//影
    if (Shadow.enable == 1)
    {
        if (PosSM.z > 1.0)
        {
            sma = 1.0;
        }
        else if (Shadow.mode == 0)
        {
            float sm0 = ShadowMapNear.Sample(BorderSampler, PosSM.xy);

            
            if (PosSM.z - 0.0002 > sm0)
            {
                sma = 0.5;


            }
        }
        else if (Shadow.mode == 1)
        {
            sma = GetVarianceDirectionalShadowFactor(PosSM);
            if (sma != 1.0f)
            {
                sma = sma * 0.5;

            }
        }
        
 

    }
    else
    {
        sma = 1.0;
    }

	
    if (Material.noDiffuseTex == 0)
    {
        color = DiffuseTexture.Sample(WrapSampler, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        color = inDiffuse;
    }

    float alpha = color.a;
    //ディレクショナルライト
    if (direcLight.allEnable.x == 0)
    {
        color = color * Material.Diffuse * sma;
    }
    else
    {
        float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

        float3 lightDir;
        float light;
        float3 iA;
        float3 iD;
        float3 iS;

        
        for (int i = 0; i < MAX_DIREC_LIGHT; i++)
        {
  
            if (direcLight.m_Enable[i].x == 1)
            {
                lightDir = normalize(direcLight.m_Direction[i].xyz);
                light = dot(lightDir, normal.xyz);
                light = (0.5 - 0.5 * light);

                
         

                float3 r = 2.0 * normal.xyz * light - lightDir;

                float3 v = normalize(Camera.xyz - inWorldPos.xyz);
                        
                        
                iA = color.xyz * Material.Ambient.xyz * direcLight.m_Ambient[i].xyz;
                        
                        
                        
                iD = color * Material.Diffuse * light * direcLight.m_Diffuse[i] * sma;
                iS = color.xyz * pow(saturate(dot(r, v)), Material.Shininess) * Material.Specular.xyz;

                //if (light > 0.5)
                //{
                //    iD *= sma;
                //}
                        

                       
                        
                        
                tempColor = float4(saturate((iA + iD + iS)), 1.0f);

                
            }
            else
            {
                tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
            }
            
            outColor += tempColor;
            
            for (int i = 0; i < MAX_POINT_LIGHT; i++)
            {
                if (pointLight.m_Enable[i].x == 1)
                {
                }

            }

            
        }
        
        
        
        color = outColor;
        color.a = alpha;
        
    }
		

    
    
    outDiffuse = color;
}

        
