// �}�g���N�X�o�b�t�@
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

// �}�e���A���o�b�t�@
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

// ���C�g�p�o�b�t�@
struct DIREC_LIGHT
{
    float4 m_Direction[MAX_DIREC_LIGHT]; // ���C�g�̕���
    float4 m_Diffuse[MAX_DIREC_LIGHT]; // �g�U���̐F
    float4 m_Ambient[MAX_DIREC_LIGHT]; // �����̐F
    int4 m_Enable[MAX_DIREC_LIGHT];
    int4 allEnable;

};


cbuffer DirecLightBuffer : register(b4)
{
    DIREC_LIGHT direcLight;
    
    
}
// ���C�g�p�萔�o�b�t�@�\����
struct POINT_LIGHT
{
    float4 m_Position[MAX_POINT_LIGHT]; // ���C�g�̈ʒu
    float4 m_Diffuse[MAX_POINT_LIGHT]; // �g�U���̐F
    float4 m_Ambient[MAX_POINT_LIGHT]; // �����̐F
    float4 m_Attenuation[MAX_POINT_LIGHT]; // ������    
    float4 m_intensity[MAX_POINT_LIGHT]; // ���C�g�̋��x
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
cbuffer TessellationBuffer : register(b9)
{
    float cbEdgeFactor; //4�p�`�̕ӂ̕����ʂ̎w��
    float cbInsideFactor; //4�p�`�̓����̕����ʂ̎w��
    float heightFactor; //heightMap�̍����ɑ΂��Ă�����l
    float d1;
    float2 uvScale; //uv�X�P�[��
    float2 heghtMapUVScale; //heightmap��uv�X�P�[��
   
};


Texture2D DiffuseTexture : register(t0);
Texture2D NormalTex : register(t1);
Texture2D armTex : register(t2);
Texture2D ShadowMapNear : register(t3);
Texture2D ShadowMapFar : register(t4);
Texture2D HeightMap : register(t5);

SamplerState WrapSampler : register(s0);
SamplerState BorderSampler : register(s1);
SamplerState ClampSampler : register(s2);
SamplerState MirrorSampler : register(s3);







struct VS_in
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
    
};



VS_OUTPUT VSmain(VS_in vsin)
{
    VS_OUTPUT output;
    output.Position = vsin.Position;
    output.TexCoord = vsin.TexCoord;
    output.Diffuse = vsin.Diffuse;
	
	

    return output;
}

//�e�b�Z���[�V����
//�n���V�F�[�_�[
struct HS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[4] : SV_TessFactor;
    float InsideTessFactor[2] : SV_InsideTessFactor;
};



HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
  InputPatch<VS_OUTPUT, 4> ip,
  uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;
  [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        Output.EdgeTessFactor[i] = cbEdgeFactor;
    }
    Output.InsideTessFactor[0] = Output.InsideTessFactor[1] = cbInsideFactor;
    return Output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_OUTPUT HSmain(InputPatch<VS_OUTPUT, 4> patch,
  uint i : SV_OutputControlPointID,
  uint PatchID : SV_PrimitiveID)
{
    
    HS_OUTPUT Output;
    Output.Position = patch[i].Position;
    Output.TexCoord = patch[i].TexCoord;
    Output.Diffuse = patch[i].Diffuse;
    return Output;
}

struct HS_CONSTANT_DATA
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};



struct DS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
};

[domain("quad")]
DS_OUTPUT DSmain(
  HS_CONSTANT_DATA_OUTPUT input,
  float2 domain : SV_DomainLocation,
  const OutputPatch<HS_OUTPUT, 4> patch)
{
    DS_OUTPUT output;
// ���_���W
    float4 pos = float4(
    patch[0].Position * (1 - domain.x) * (1 - domain.y) +
    patch[1].Position * domain.x * (1 - domain.y) +
    patch[2].Position * (1 - domain.x) * domain.y +
    patch[3].Position * domain.x * domain.y);
            // uv

    float2 uv = float2(
    patch[0].TexCoord * (1 - domain.x) * (1 - domain.y) +
    patch[1].TexCoord * domain.x * (1 - domain.y) +
    patch[2].TexCoord * (1 - domain.x) * domain.y +
    patch[3].TexCoord * domain.x * domain.y);
   
    output.TexCoord = uv*uvScale;

    // Heightmap�̃f�[�^���g�p���č����𒲐�
    float height = HeightMap.SampleLevel(MirrorSampler, uv*heghtMapUVScale, 0).r;
    pos.y += height * heightFactor;

    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    output.Position = mul(pos, wvp);
    


    output.Diffuse = patch[0].Diffuse;
    
    output.WorldPos = mul(pos, World);

    return output;
}



struct GS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WorldPos : POSITION0;
    float4 Normal : NORMAL0;
    float4 Tangent : TANGENT0;
    float4 BiNormal : BINORMAL0;
};


//�ʂ��Ƃ̖@�����v�Z���邽�߂̃W�I���g���V�F�[�_�[

[maxvertexcount(3)]
void GSmain(triangle DS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> TriStream)
{
    GS_OUTPUT output;

    
    //�@��
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
        TriStream.Append(output);
    }
}





float GetVarianceDirectionalShadowFactor(float4 shadowCoord)
{
    
    float2 depth = ShadowMapNear.Sample(BorderSampler, shadowCoord.xy).xy;
    float depth_sq = depth.x * depth.x; // E(x)^2
    float variance = depth.y - depth_sq; // ��^2 = E(x^2) - E(x^2)
    variance = saturate(variance + Shadow.facter); // facter��ǉ����Ĉ��萫������

    float fragDepth = shadowCoord.z;
    float md = fragDepth - depth.x; // t - ��
    float p = variance / (variance + (md * md)); // ��^2 / (��^2 + (t - ��)^2)

    return saturate(max(p, fragDepth <= depth.x)); // P(x >= t)�𖞂����Ƃ��̂�
}




struct PSout
{
    float4 outDiffuse : SV_Target;
};

PSout PSmain(GS_OUTPUT input)
{
    PSout psout;

    float4 color;

    
    
    float4 normal = input.Normal;
    

    float sma = 1.0;

    float4 pos4 = mul(input.WorldPos, Shadow.wvp);
    float4 PosSM;
    PosSM.x = (pos4.x + 1.0) / 2.0;
    PosSM.y = (-pos4.y + 1.0) / 2.0;
    PosSM.z = pos4.z;
    PosSM.w = 1.0f;
    
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
    
    					//�e
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
                sma = sma*0.5;

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
    //�f�B���N�V���i�����C�g
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
                        
                        
                        
                iD = color * Material.Diffuse * light * direcLight.m_Diffuse[i]*sma;
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
		

    
    
    psout.outDiffuse = color;
    
    
    return psout;
}

        
