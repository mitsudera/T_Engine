struct Fade
{
    float4 fade;

};

cbuffer FadeBuffer : register(b9)
{
    Fade fade;

}





Texture2D Texture : register(t0);
SamplerState smpWrap : register(s0);
SamplerState smpBorder : register(s1);

void PS_Main(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		

		out float4 outDiffuse : SV_Target)
{
    outDiffuse = fade.fade.x * Texture.Sample(smpWrap, float2(inTexCoord));
    outDiffuse.a = 1.0f;
}
