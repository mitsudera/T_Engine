

void VS_Main(
		in float4 inPosition : POSITION0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,


		out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0
)
{
    outPosition = inPosition;

    outTexCoord = inTexCoord;
    outDiffuse = inDiffuse;
}


