Texture2D g_txDiffuse  : register (t0);
SamplerState sample0 : register (s0);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};


struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD0;
};



VS_OUTPUT VS( float4 p : POSITION, float2 t : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.t = t*cosf(g_Timer);
    return output;
}


struct PS_INPUT
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD0;
};



float4 PS(PS_INPUT input) : SV_Target
{
	//PS_OUT psout;
	//g_txDiffuse.Sample(sample0, input.t);
	//psOut.c =lerp(vClolrA,vColorB,g_color1.w)

	return  g_txDiffuse.Sample(sample0, input.t);	
			//psout
}




