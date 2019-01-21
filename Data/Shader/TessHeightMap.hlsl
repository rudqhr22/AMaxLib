
#define MAX_POINTS 3
//--------------------------------------------------------------------------------------
// 전역 변수
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
SamplerState g_samLinear: register (s0);

cbuffer cb0: register(b0)
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
// 상수 버퍼
//--------------------------------------------------------------------------------------
cbuffer HS_TESSELLATION_BUFFER: register(b1)
{
	float	tessellationAmount;
	float3	padding;
};
//--------------------------------------------------------------------------------------
// 구조체
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
//--------------------------------------------------------------------------------------
// Hull-Shader 구조
//--------------------------------------------------------------------------------------
struct HS_CONSTANT_OUTPUT
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};
struct HS_OUTPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
//--------------------------------------------------------------------------------------
// Domain-Shader 구조
//--------------------------------------------------------------------------------------
struct DS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
//--------------------------------------------------------------------------------------
// Pixel-Shader 구조
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
	float4 c : SV_Target;
};



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT output;
    output.p = input.p;    
    output.n = input.n;    
	output.c = input.c;  
	output.t = input.t;  
    return output;
}
//////////////////////////////////////////////////////////////////////////////////
//// Hull Shader
//////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
[maxtessfactor(64.0f)]
HS_OUTPUT HS(	InputPatch< VS_OUTPUT, MAX_POINTS> patch, 
				uint pointId : SV_OutputControlPointID, 
				uint patchId : SV_PrimitiveID)
{
    HS_OUTPUT output;
    output.p = patch[pointId].p;
	output.n = patch[pointId].n;
    output.c = patch[pointId].c;	
	output.t = patch[pointId].t;
    return output;
}
////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
HS_CONSTANT_OUTPUT PatchConstantFunction(	InputPatch<VS_OUTPUT, MAX_POINTS> inputPatch, 
											uint patchId : SV_PrimitiveID)
{    
    HS_CONSTANT_OUTPUT output;
    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;
    output.inside = tessellationAmount;
    return output;
}
////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
DS_OUTPUT DS(	HS_CONSTANT_OUTPUT input, 
				float3 BarycentricCoordinates : SV_DomainLocation, 
				const OutputPatch<HS_OUTPUT, 3> patch)
{
	float3 vertexPosition;
	DS_OUTPUT output;

    float3 vWorldPos = BarycentricCoordinates.x * patch[0].p + 
                       BarycentricCoordinates.y * patch[1].p + 
                       BarycentricCoordinates.z * patch[2].p;    

    output.t = BarycentricCoordinates.x * patch[0].t + 
                      BarycentricCoordinates.y * patch[1].t + 
                      BarycentricCoordinates.z * patch[2].t;                      

    output.c = BarycentricCoordinates.x * patch[0].c + 
                      BarycentricCoordinates.y * patch[1].c + 
                      BarycentricCoordinates.z * patch[2].c;  

    output.n = BarycentricCoordinates.x * patch[0].n + 
                      BarycentricCoordinates.y * patch[1].n + 
                      BarycentricCoordinates.z * patch[2].n;

    output.p = mul(float4(vWorldPos, 1.0f), g_matWorld);
    output.p = mul(output.p, g_matView);
    output.p = mul(output.p, g_matProj);

    return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS(DS_OUTPUT input)
{
	PS_OUTPUT output;
    output.c = g_txDiffuse.Sample( g_samLinear, input.t ) * input.c;	
	return output;
}