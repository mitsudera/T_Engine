Texture2D Texture : register(t0);
SamplerState smpWrap : register(s0);

void PSMain(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		

		out float4 outDiffuse : SV_Target)
{
    outDiffuse = Texture.Sample(smpWrap, float2(inTexCoord));
}
