Texture2D g_txDiffuse: register (t0);
Texture2D g_txTex0: register (t1);
Texture2D g_txTex1: register (t2);
Texture2D g_txTex2: register (t3);
Texture2D g_txTex3: register (t4);
Texture2D g_txTex4: register (t5);

SamplerState samLinear: register (s0);
cbuffer cb0
{
	float4x4	g_matWorld		: packoffset(c0);
	float4x4	g_matView		: packoffset(c4);
	float4x4	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 p : POSITION;
	float4 c : COLOR;
	float2 t : TEXCOORD;
	 float4x4 mTransform : TRANSFORM;
	float4 m[4] : ANIMATION;
	float4 mesh : MESHCOLOR;
	uint  iVertex : SV_VertexID;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
// float4  정점을 반환해야 한다.
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output;
	output.p = mul(vIn.p, vIn.mTransform);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.t = vIn.m[vIn.iVertex].xy;
	output.c = vIn.mesh;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float4 cColorA = g_txDiffuse.Sample(samLinear, vIn.t) *vIn.c;
	return cColorA;
}
