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
    int enable;
    int mode;
    float facter;
    int dummy;
};

cbuffer ShadowBuffer : register(b7)
{
    SHADOW Shadow;
}



struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    float4 Tangent : TANGENT0;
    float4 BiNoramal : BINORMAL0;

};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
    float4 PosSM : POSITION1;
    float4 Normal : NORMAL0;
    float4 Tangent : TANGENT0;
    float4 BiNoramal : BINORMAL0;

;
};


//=============================================================================
// 頂点シェーダ
//=============================================================================
VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    matrix wvp;
    wvp = mul(View, Projection);
    output.Position = mul(input.Position, wvp);

    output.TexCoord = input.TexCoord;

    output.WorldPos = input.Position;

    output.Diffuse = input.Diffuse;
	
    output.Normal = input.Normal;
    output.Tangent = input.Tangent;
    output.BiNoramal = input.BiNoramal;

    float4 pos4 = mul(input.Position, Shadow.wvp);
    pos4.xyz = pos4.xyz / pos4.w;
    output.PosSM.x = (pos4.x + 1.0) / 2.0;
    output.PosSM.y = (-pos4.y + 1.0) / 2.0;
    output.PosSM.z = pos4.z;
    output.PosSM.w = 1.0f;

    return output;
}

struct GS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
    float4 PosSM : POSITION1;
    float4 Normal : NORMAL0;
    float4 Tangent : TANGENT0;
    float4 BiNormal : BINORMAL0;
};



[maxvertexcount(3)]
void GSmain(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> TriStream)
{
    GS_OUTPUT output;

    
    //法線
    float3 edge1 = input[1].Position.xyz - input[0].Position.xyz;
    float3 edge2 = input[2].Position.xyz - input[0].Position.xyz;
    float3 faceNormal = normalize(cross(edge1, edge2));

    //tangent
    float2 deltaUV1 = input[1].TexCoord - input[0].TexCoord;
    float2 deltaUV2 = input[2].TexCoord - input[0].TexCoord;

    float3 tangent = (deltaUV2.y * edge1 - deltaUV1.y * edge2) / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    tangent = normalize(tangent);

    
    //binormal
    float3 binormal = cross(faceNormal, tangent);
    binormal = normalize(binormal);

    // Output vertices with the calculated normal, tangent, and binormal
    for (int i = 0; i < 3; ++i)
    {
        output.Position = input[i].Position;
        output.Diffuse = input[i].Diffuse;
        output.Normal = float4(faceNormal, 1.0f);
        output.Tangent = float4(tangent, 1.0f);
        output.BiNormal = float4(binormal, 1.0f);
        output.TexCoord = input[i].TexCoord;
        output.WorldPos = input[i].WorldPos;
        output.PosSM = input[i].PosSM;
        
        TriStream.Append(output);
    }
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
    variance = saturate(variance + Shadow.facter); // facterを追加して安定性を向上

    float fragDepth = shadowCoord.z;
    float md = fragDepth - depth.x; // t - μ
    float p = variance / (variance + (md * md)); // σ^2 / (σ^2 + (t - μ)^2)

    return saturate(max(p, fragDepth <= depth.x)); // P(x >= t)を満たすときのみ
}

struct PSout
{
    float4 Diffuse : SV_Target;
};


PSout PSmain(GS_OUTPUT input)
{
    float4 color;

    float sma = 1.0;
    bool shadow;
    
    
    float4 normal = input.Normal;
    
    if (Material.noNormalTex == 0)
    {
        // Sample the normal map
        float3 normalMap = NormalTex.Sample(WrapSampler, input.TexCoord).rgb;
        
        normalMap.x = 1.0 - normalMap.x;
        normalMap.y = 1.0 - normalMap.y;
        //normalMap.z = 1.0 - normalMap.z;
       
        normalMap = normalize(normalMap * 2.0 - 1.0);
        
        
        
        float3x3 TBN = float3x3(input.Tangent.xyz, input.BiNormal.xyz, input.Normal.xyz);
        normal.xyz = mul(normalMap, TBN);
        
        
    }
    
    					//影
    if (Shadow.enable == 1)
    {
        if (input.PosSM.z > 1.0)
        {
            sma = 1.0;
        }
        else if (Shadow.mode == 0)
        {
            float sm0 = ShadowMapNear.Sample(BorderSampler, input.PosSM.xy);

            
            if (input.PosSM.z - 0.0002 > sm0)
            {
                sma = 0.5;


            }
        }
        else if (Shadow.mode == 1)
        {
            sma = GetVarianceDirectionalShadowFactor(input.PosSM);
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
        color = DiffuseTexture.Sample(WrapSampler, input.TexCoord);

        color *= input.Diffuse;
    }
    else
    {
        color = input.Diffuse;
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

                float3 v = normalize(Camera.xyz - input.WorldPos.xyz);
                        
                        
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
		

    PSout output;
    output.Diffuse = color;
    return output;
}

        
