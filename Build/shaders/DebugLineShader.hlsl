

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






//=============================================================================
// 頂点シェーダ
//=============================================================================
void VSmain(              in  float4 inPosition		: POSITION,
						  in  float4 inDiffuse		: COLOR,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outDiffuse		: COLOR0
)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);


	outDiffuse = inDiffuse;
	


}




void PSmain(in float4 inPosition : SV_POSITION,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{
    outDiffuse = float4(0.0f, 1.0f, 0.0f, 1.0f);
}

