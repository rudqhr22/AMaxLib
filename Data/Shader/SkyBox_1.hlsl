Texture2D g_txDiffuse  : register (t0);
Texture2DArray g_txDiffuseArrary : register (t1);

SamplerState        g_samPointClamp : register(s0);
SamplerState        g_samLinearWrap : register(s1);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4	g_color1		: packoffset(c12);
};

struct VS_INPUT
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};


struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};


struct PS_INPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
	uint PrimitiveID : SV_PrimitiveID;
};




VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);

	output.n = vIn.n;
	output.t = vIn.t;
	output.c = g_color1; // vIn.t;
	return output;
}


float4 PS(PS_INPUT input):SV_Target
{
	uint index = input.PrimitiveID / 2;		//12°³¸¦ ¹Ý¶¥
	float4 fColor = float4(1, 1, 1, 1);
	

	fColor = g_txDiffuseArrary.Sample(g_samPointClamp,float3(input.t,index));
	return fColor;

	//return g_txDiffuse.Sample(sample0, input.t);// *input.c;
}



