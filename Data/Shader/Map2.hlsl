//Texture2D g_txDiffuse  : register (t0);
//SamplerState sample0 : register (s0);
//
//cbuffer cb0
//{
//	matrix	g_matWorld		: packoffset(c0);
//	matrix	g_matView		: packoffset(c4);
//	matrix	g_matProj		: packoffset(c8);
//	float4	g_color1		: packoffset(c12);
//	//float4	g_color2		: packoffset(c13);
//};
//
//struct VS_INPUT
//{
//	float4 p : POSITION;
//	float3 n : NORMAL;
//	float4 c : COLOR0;
//	float2 t : TEXCOORD;
//};
// 
//
//struct VS_OUTPUT
//{
//	float4 p : SV_POSITION;
//	float3 n : NORMAL;
//	float4 c : COLOR;
//	float2 t : TEXCOORD;
//};
//
//
//
//VS_OUTPUT VS(VS_INPUT vIn)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//	output.p = mul(vIn.p, g_matWorld);
//	output.p = mul(output.p, g_matView);
//	output.p = mul(output.p, g_matProj);
//	output.n = vIn.n;
//	output.t = vIn.t *10;
//	output.c = vIn.c * g_color1;
//	return output;
//}
//
//
//float4 PS(VS_OUTPUT input) : SV_Target
//{	
////	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
//
//	//float4 vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
//	float3 vLight = float3(500, 500, 0);
//	normalize(vLight);
//
//	//float fDot = max(0.0f, dot(vLight, input.n));
//	float fDot =  dot(vLight, input.n);
//
//	//vColor.r = fDot;
//	//vColor.g = fDot;
//	//vColor.b = fDot;
//	//vColor.a = 0;
//
//	//input.c.x = fDot;//
//	//input.c.y = fDot;//
//	//input.c.z = fDot;//
//	//input.c.w =1.0f;//
//
//	//vColor;//;
//	
//
//	//max(1,1);
//
//	float4 finalColor = g_txDiffuse.Sample(sample0, input.t) *input.c;
//	return finalColor;// *input.c;// *vColor;
//
//
//	//return  g_txDiffuse.Sample(sample0, input.t)*input.c; //g_txDiffuse.Sample(sample0, input.t);// *input.c;
//}



Texture2D g_txDiffuse: register (t0);
SamplerState samLinear: register (s0);
cbuffer cb0
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
	float4		g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
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
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT vIn)
{
//	float4 vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
//	float3 vLight = float3(500, 500, 0);
//	normalize(vLight);
//	float fDot = max(0.0f, dot(vIn.n, vLight));
////	float fDot = dot(vIn.n, vLight);
//	vColor.x = fDot;
//	vColor.y = fDot;
//	vColor.z = fDot;
//	vColor.w = 1;
//	//vIn.c = vColor;


	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t*1;
	output.c = vIn.c * g_MeshColor;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	//float4 vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//float3 vLight = float3(500, 500, 0);
	//normalize(vLight);
	//
	//float fDot = max(0.0f, dot(vIn.n, vLight));
	////float fDot = dot(vIn.n, vLight);
	//vColor.x = vColor.x*fDot;
	//vColor.y = vColor.y*fDot;
	//vColor.z = vColor.z*fDot;
	//vColor.w = 1;
	//
	//vIn.c = vColor;

	return g_txDiffuse.Sample(samLinear, vIn.t) *vIn.c;
}

