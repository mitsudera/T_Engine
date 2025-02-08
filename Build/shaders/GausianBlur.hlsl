struct GAUSSIAN
{
    float4 weight1;
    float4 weight2;

};

cbuffer GaussianBuffer : register(b9)
{
    GAUSSIAN gaus;

}





Texture2D Texture : register(t0);
SamplerState smpWrap : register(s0);
SamplerState smpBorder : register(s1);

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