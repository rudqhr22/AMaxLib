#define FOGMODE_NONE    0
#define FOGMODE_LINEAR  1
#define FOGMODE_EXP     2
#define FOGMODE_EXP2    3
#define E 2.71828
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D	g_txDiffuse : register (t0);
SamplerState g_samLinear: register (s0);
//-----------------------------------------------------------------------------
//상수 버퍼
//-----------------------------------------------------------------------------
cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
cbuffer cbFog: register(b1)
{
	float4				g_ClipPlane:	packoffset(c0);
	//float4				g_vCameraPos:	packoffset(c1);
	//float4				g_FogColor:		packoffset(c2);
	//float				g_FogMode :		packoffset(c3.x);		 // = FOGMODE_LINEAR;
	//float				g_FogDensity :	packoffset(c3.y);
	//float				g_FogStart :	packoffset(c3.z);
	//float				g_FogEnd :		packoffset(c3.w);
};

struct P3N3CT2_VS_INPUT
{
	float3 p		: POSITION;
	float3 n		: NORMAL;
	float4 c	    : COLOR0;
	float2 t		: TEXCOORD0;
};
struct P4CT2_VS_OUTPUT
{
	float4 p		: SV_POSITION;
	float4 c		: COLOR0;
	float2 t		: TEXCOORD0;
	float4 FogDist  : TEXCOORD1;
	float  clip : SV_ClipDistance0;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
P4CT2_VS_OUTPUT VS(P3N3CT2_VS_INPUT input)
{
	P4CT2_VS_OUTPUT output = (P4CT2_VS_OUTPUT)0;

	float4 worldPos = mul(float4(input.p, 1), g_matWorld);
	float4 cameraPos = mul(worldPos, g_matView);
	output.p = mul(cameraPos, g_matProj);

	output.t = input.t * 100;
	output.c = input.c;

	worldPos.w = 1;
	output.clip = dot(worldPos, g_ClipPlane);		//월드좌표?, 클립 내적?

	return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(P4CT2_VS_OUTPUT input) : SV_Target
{

	float4 vTexColor = g_txDiffuse.Sample(g_samLinear, input.t);
	
	return vTexColor * input.c;; //(vTexColor + vFogColor)  * input.c;
}
