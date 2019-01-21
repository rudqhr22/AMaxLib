//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D	g_txDiffuse : register (t0);
Texture2D	g_txRefractMap: register (t1);
Texture2D	g_txReflectMap: register (t2);
Texture2D	g_txNormalMap: register (t3);
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

struct P3N3CT2_VS_INPUT
{
	float3 p		: POSITION;
	float3 n		: NORMAL;
	float4 c	    : COLOR0;
	float2 t		: TEXCOORD0;
	uint   id      : SV_VERTEXID;
};
struct P4CT2_VS_OUTPUT
{
	float4 p		: SV_POSITION;
	float3 n		: NORMAL;
	float4 c		: COLOR0;
	float2 t		: TEXCOORD0;
	float2 tW		: TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
P4CT2_VS_OUTPUT VS(P3N3CT2_VS_INPUT input)
{
	P4CT2_VS_OUTPUT output = (P4CT2_VS_OUTPUT)0;
	float4 worldPos = mul(float4(input.p, 1), g_matWorld);
	// 클립플랜 높이 보다 작을 경우에만 높이를 조정한다.
	worldPos.y = min( worldPos.y, worldPos.y + cos(g_MeshColor.w*input.p.x) + sin(g_MeshColor.w*input.p.z));

	float4 cameraPos = mul(worldPos, g_matView);
	output.p = mul(cameraPos, g_matProj);	
	output.n = input.n;
	output.c = input.c;
	output.t = input.t * 1;
	// 투영공간 굴절 및 반사 텍스쳐 좌표
	float2 tex;
	tex.x = 0.5f*output.p.x / output.p.w + 0.5f;
	tex.y = -0.5f*output.p.y / output.p.w + 0.5f;
	output.tW = tex;//normalize( (Out.p - 0.5f) * 2.0f );	
	return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(P4CT2_VS_OUTPUT input) : SV_Target
{
	float4 vWaterColor = g_txDiffuse.Sample(g_samLinear, input.t);
	float4 normal = g_txNormalMap.Sample(g_samLinear, input.t*10.0f);
		   normal = normalize((normal - 0.5f) * 2.0f);

	float4 vRefractColor = g_txRefractMap.Sample(g_samLinear, input.tW + normal.xy*0.01f);
	float4 vReflectColor = g_txReflectMap.Sample(g_samLinear, input.tW + normal.xy*0.01f);
	float4 vColor = vWaterColor * vRefractColor * vReflectColor * input.c;
	vColor.a = 1.0f;
	return vColor;
}