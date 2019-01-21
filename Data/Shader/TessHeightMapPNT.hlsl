#define MAX_POINTS 3	//정점이 최대 3개가 넘어 올 수있다. 제어점 단위로 돌아간다.

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
cbuffer HS_TESSELLATION_BUFFER: register(b1)	//패치상수?
{
	float	tessellationAmount;
	float3	padding;			//4바이트를 맞추기 위해서 추가함
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
	float edges[3] : SV_TessFactor;			//시스템밸류 값
	float inside :   SV_InsideTessFactor;	//시스템밸류 값

		// Geometry cubic generated control points
	float3 f3B210    : POSITION3;
	float3 f3B120    : POSITION4;
	float3 f3B021    : POSITION5;
	float3 f3B012    : POSITION6;
	float3 f3B102    : POSITION7;
	float3 f3B201    : POSITION8;
	float3 f3B111    : CENTER;

	// Normal quadratic generated control points
	float3 f3N110    : NORMAL3;
	float3 f3N011    : NORMAL4;
	float3 f3N101    : NORMAL5;
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
[patchconstantfunc("HS_PNTrianglesConstant")]	
[outputcontrolpoints(3)]
[maxtessfactor(9)]					
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 3> I, uint uCPID : SV_OutputControlPointID)
{
	HS_OUTPUT O = (HS_OUTPUT)0;

	// Just pass through inputs = fast pass through mode triggered
	O.p = I[uCPID].p;
	O.n = I[uCPID].n;
	O.c = I[uCPID].c;
	O.t = I[uCPID].t;
	return O;
}
////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////

HS_CONSTANT_OUTPUT HS_PNTrianglesConstant(InputPatch<VS_OUTPUT, 3> I)
{
	HS_CONSTANT_OUTPUT O = (HS_CONSTANT_OUTPUT)0;
	// Now setup the PNTriangle control points...

	O.edges[0] = tessellationAmount;
	O.edges[1] = tessellationAmount;
	O.edges[2] = tessellationAmount;

	// Assign Positions
	float3 f3B003 = I[0].p;
	float3 f3B030 = I[1].p;
	float3 f3B300 = I[2].p;
	// And Normals
	float3 f3N002 = I[0].n;
	float3 f3N020 = I[1].n;
	float3 f3N200 = I[2].n;

	// Compute the cubic geometry control points
	// Edge control points
	O.f3B210 = ((2.0f * f3B003) + f3B030 - (dot((f3B030 - f3B003), f3N002) * f3N002)) / 3.0f;
	O.f3B120 = ((2.0f * f3B030) + f3B003 - (dot((f3B003 - f3B030), f3N020) * f3N020)) / 3.0f;
	O.f3B021 = ((2.0f * f3B030) + f3B300 - (dot((f3B300 - f3B030), f3N020) * f3N020)) / 3.0f;
	O.f3B012 = ((2.0f * f3B300) + f3B030 - (dot((f3B030 - f3B300), f3N200) * f3N200)) / 3.0f;
	O.f3B102 = ((2.0f * f3B300) + f3B003 - (dot((f3B003 - f3B300), f3N200) * f3N200)) / 3.0f;
	O.f3B201 = ((2.0f * f3B003) + f3B300 - (dot((f3B300 - f3B003), f3N002) * f3N002)) / 3.0f;
	// Center control point
	float3 f3E = (O.f3B210 + O.f3B120 + O.f3B021 + O.f3B012 + O.f3B102 + O.f3B201) / 6.0f;
	float3 f3V = (f3B003 + f3B030 + f3B300) / 3.0f;
	O.f3B111 = f3E + ((f3E - f3V) / 2.0f);

	// Compute the quadratic normal control points, and rotate into world space
	float fV12 = 2.0f * dot(f3B030 - f3B003, f3N002 + f3N020) / dot(f3B030 - f3B003, f3B030 - f3B003);
	O.f3N110 = normalize(f3N002 + f3N020 - fV12 * (f3B030 - f3B003));
	float fV23 = 2.0f * dot(f3B300 - f3B030, f3N020 + f3N200) / dot(f3B300 - f3B030, f3B300 - f3B030);
	O.f3N011 = normalize(f3N020 + f3N200 - fV23 * (f3B300 - f3B030));
	float fV31 = 2.0f * dot(f3B003 - f3B300, f3N200 + f3N002) / dot(f3B003 - f3B300, f3B003 - f3B300);
	O.f3N101 = normalize(f3N200 + f3N002 - fV31 * (f3B003 - f3B300));

	// Inside tess factor is just the average of the edge factors
	O.inside = (O.edges[0] + O.edges[1] + O.edges[2]) / 3.0f;

	return O;
}

////////////////////////////////////////////////////////////////////////////////
// Domain Shader	테셀레이션에서 작성된 제어점과 패치상수를 받는다
// 도메인으로 무게중심좌표(3점?)와 패치상수?
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
DS_OUTPUT DS(HS_CONSTANT_OUTPUT HSConstantData,
	const OutputPatch<HS_OUTPUT, 3> I,
	float3 f3BarycentricCoords : SV_DomainLocation)
{
	DS_OUTPUT O = (DS_OUTPUT)0;

	// The barycentric coordinates
	float fU = f3BarycentricCoords.x;
	float fV = f3BarycentricCoords.y;
	float fW = f3BarycentricCoords.z;

	// Precompute squares and squares * 3 
	float fUU = fU * fU;
	float fVV = fV * fV;
	float fWW = fW * fW;
	float fUU3 = fUU * 3.0f;
	float fVV3 = fVV * 3.0f;
	float fWW3 = fWW * 3.0f;

	// Compute position from cubic control points and barycentric coords
	float3 f3Position = I[0].p * fWW * fW +
		I[1].p * fUU * fU +
		I[2].p * fVV * fV +
		HSConstantData.f3B210 * fWW3 * fU +
		HSConstantData.f3B120 * fW * fUU3 +
		HSConstantData.f3B201 * fWW3 * fV +
		HSConstantData.f3B021 * fUU3 * fV +
		HSConstantData.f3B102 * fW * fVV3 +
		HSConstantData.f3B012 * fU * fVV3 +
		HSConstantData.f3B111 * 6.0f * fW * fU * fV;

	// Compute normal from quadratic control points and barycentric coords
	float3 f3Normal = I[0].n * fWW +
		I[1].n * fUU +
		I[2].n * fVV +
		HSConstantData.f3N110 * fW * fU +
		HSConstantData.f3N011 * fU * fV +
		HSConstantData.f3N101 * fW * fV;

	// Normalize the interpolated normal    
	f3Normal = normalize(f3Normal);

	O.c = f3BarycentricCoords.x * I[0].c +
		f3BarycentricCoords.y * I[0].c +
		f3BarycentricCoords.z * I[0].c;

	// Linearly interpolate the texture coords
	O.t = I[0].t * fW + I[1].t * fU + I[2].t * fV;
	O.n = f3Normal;
	// Calc diffuse color    
   // O.c.rgb = float3(1,1,1);//g_f4MaterialDiffuseColor * g_f4LightDiffuse * max( 0, dot( f3Normal, g_f4LightDir.xyz ) ) + g_f4MaterialAmbientColor;  
	O.c.a = 1.0f;

	// Transform model position with view-projection matrix
	//O.f4Position = mul( float4( f3Position.xyz, 1.0 ), g_f4x4ViewProjection );
	O.p = mul(float4(f3Position, 1.0f), g_matWorld);
	O.p = mul(O.p, g_matView);
	O.p = mul(O.p, g_matProj);
	return O;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
//PS_OUTPUT PS(DS_OUTPUT input)
//{
//	PS_OUTPUT output;
//    output.c = g_txDiffuse.Sample( g_samLinear, input.t ) * input.c;	
//	return output;
//}

float4 PS(DS_OUTPUT input) : SV_Target
{
	return g_txDiffuse.Sample(g_samLinear, input.t) * input.c;
}

