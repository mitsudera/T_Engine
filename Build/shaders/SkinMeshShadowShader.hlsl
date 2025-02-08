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
    int enable;
    int mode;
    float facter;
    int dummy;
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
// ���_�V�F�[�_
//=============================================================================


Texture2D Texture : register(t0);
SamplerState smpWrap : register(s0);
SamplerState smpBorder : register(s1);

void VSmain(in float4 inPosition : POSITION0,
						  in float4 inNormal : NORMAL0,
						  in float4 inDiffuse : COLOR0,
						  in float2 inTexCoord : TEXCOORD0,
                          in float4 inTangent : TANGENT0,
                          in float4 inBiNoramal : BINORMAL0,
                          in float4 inBoneWeight : BONEWEIGHT,
                          in uint4 inBoneIndex : BONEINDEX,

						  out float4 outPosition : SV_POSITION,
						  out float2 outTexCoord : TEXCOORD0,
						  out float4 outDiffuse : COLOR0
)
{
    
    matrix World = matrix(
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0);
    
    for (int i = 0; i < 4; i++)
    {
        if (inBoneWeight[i] == 0.0f)
            continue;
        
        World += boneMtx[inBoneIndex[i]] * inBoneWeight[i];
    }
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);

    outTexCoord = inTexCoord;


    outDiffuse = inDiffuse;
	
	


}


//�V���h�E�}�b�v�p�s�N�Z���V�F�[�_�[
void PS_SM(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,


		out float4 outDiffuse : SV_Target)
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

