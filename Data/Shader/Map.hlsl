Texture2D g_txDiffuse  : register (t0);
SamplerState sample0 : register (s0);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4	g_color1		: packoffset(c12);
	float4	g_color2		: packoffset(c13);
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



VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t;
	output.c = vIn.c;// g_color2;// lerp(g_color1, g_color2,g_color1.w);
	return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	//float4 fBlendA = g_txDiffuse.Sample(sample0, input.t);

	//fFinalColor = lerp(g_color1, g_color2, fBlendA.w)*input.c;

	return  g_txDiffuse.Sample(sample0, input.t);// *input.c;
}

//
//#define WIDEN(x) g_##x
////--------------------------------------------------------------------------------------
//// Constant Buffer Variables
////--------------------------------------------------------------------------------------
//Texture2D g_txDiffuse: register (t0);
//SamplerState g_samLinear: register (s0);
//cbuffer cb0: register(b0)
//{
//	matrix	g_matWorld		: packoffset(c0);
//	matrix	g_matView		: packoffset(c4);
//	matrix	g_matProj		: packoffset(c8);
//	float4  g_MeshColor     : packoffset(c12);
//};
////--------------------------------------------------------------------------------------
////Lighting Variables
////--------------------------------------------------------------------------------------
//cbuffer cb1: register(b1)
//{
//	matrix				g_matWorldInverse: packoffset(c0);
//	float4				g_AmbientMaterial: packoffset(c4);
//	float4				g_DiffuseMaterial: packoffset(c5);
//	float4				g_SpecularMaterial: packoffset(c6);
//	float4				g_EmissionMaterial: packoffset(c7);
//
//	float4				g_AmbientLightColor : packoffset(c8);
//	float4				g_DiffuseLightColor: packoffset(c9);
//	float4				g_cSpecularLightColor: packoffset(c10);
//
//	float3				g_vLightDir : packoffset(c11);
//	float			    g_fDamping : packoffset(c11.w);
//	float3				g_vLightPos : packoffset(c12);
//	float			    g_fRadius : packoffset(c12.w);
//	float3				g_vEyeDir : packoffset(c13);
//	float			    g_fIntensity : packoffset(c13.w);
//	float3				g_vEyePos : packoffset(c14);
//	float			    g_fEyeRadius : packoffset(c14.w);
//};
//
////--------------------------------------------------------------------------------------
//struct VS_INPUT
//{
//	float4 p : POSITION;
//	float3 n : NORMAL;
//	float4 c : COLOR;
//	float2 t : TEXCOORD;
//};
//struct VS_OUTPUT
//{
//	float4 p : SV_POSITION;
//	float3 n : NORMAL;
//	float4 c : COLOR0;
//	float2 t : TEXCOORD0;
//	float4 v : TEXCOORD1;
//};
//
////--------------------------------------------------------------------------------------
//// Vertex Shader
////--------------------------------------------------------------------------------------
//VS_OUTPUT VS(VS_INPUT vIn)
//{
//	VS_OUTPUT vOut = (VS_OUTPUT)0;
//	vOut.p = mul(vIn.p, WIDEN(matWorld));
//	vOut.p = mul(vOut.p, WIDEN(matView));
//	vOut.p = mul(vOut.p, WIDEN(matProj));
//	vOut.n = normalize(mul(vIn.n, (float3x3)g_matWorldInverse));
//	//vOut.n = vIn.n;
//	vOut.t = vIn.t;// *5;// *2;// *10;
//	vOut.c = vIn.c *g_MeshColor;
//	return vOut;
//}
//
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 Diffuse(float3 vNormal)
//{
//	float fIntensity = max(0, dot(vNormal, normalize(-g_vLightDir)));
//	float4 diffuse = g_AmbientMaterial * g_AmbientLightColor +
//		(g_DiffuseMaterial * g_DiffuseLightColor * fIntensity);
//	return diffuse;
//}
//
//float4 ComputePointLight(float3 vVertexPos, float3 vVertexNormal)
//{
//	int nNumLights = 1;
//	float4 vPointLightColor = float4(0, 0, 0, 1);
//	for (int iLight = 0; iLight < nNumLights; iLight++)
//	{
//		float4 vLight;
//		vLight.xyz = normalize(vVertexPos - g_vLightPos.xyz);
//		vLight.w = distance(vVertexPos, g_vLightPos.xyz);
//
//		float fLuminance = smoothstep(vLight.w - 5, vLight.w, g_fRadius);
//		float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));
//		vPointLightColor += float4(g_DiffuseLightColor.rgb *  fLuminance, 1.0f);
//	}
//	return  vPointLightColor;
//}
//
//float4 PS(VS_OUTPUT vIn) : SV_Target
//{
//	return g_txDiffuse.Sample(g_samLinear, vIn.t);
////	float4 vTexColor = g_txDiffuse.Sample(g_samLinear, vIn.t);
////	float4 vFinalColor = /*ComputePointLight(vIn.v, vIn.n) *vTexColor; vIn.c;*/  vTexColor *Diffuse(vIn.n);// *vIn.c;
////	vFinalColor.a = 1.0f;
////	return vFinalColor;
//}
//
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 PS_Texture(VS_OUTPUT vIn) : SV_Target
//{
//	return g_txDiffuse.Sample(g_samLinear, vIn.t);
//}
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 PS_Color(VS_OUTPUT vIn) : SV_Target
//{
//	//float4 vFinalColor = ComputePointLight(vIn.v, vIn.n);
//	//return vFinalColor;
//	return vIn.c;
//}
//
//
//
//
//
//
////--------------------------------------------------------------------------------------
//// Constant Buffer Variables
////--------------------------------------------------------------------------------------
////Texture2D	g_txDiffuse : register (t0);
////Texture2D	g_txNormalMap	: register (t1);
////SamplerState g_samLinear: register (s0);
////
////cbuffer cb0 : register(b0)
////{
////	matrix	g_matWorld		: packoffset(c0);
////	matrix	g_matView		: packoffset(c4);
////	matrix	g_matProj		: packoffset(c8);
////	float4  g_MeshColor     : packoffset(c12);
////};
////
//////-----------------------------------------------------------------------------
//////상수 버퍼
//////-----------------------------------------------------------------------------
////cbuffer cbObjectChangesEveryFrame: register(b1)
////{
////	// Global
////	matrix				g_matNormal;
////	float3				cb_vLightVector;
////	float3				cb_vEyePos;
////	float3				cb_vEyeDir;
////};
////
////cbuffer cbObjectNeverChanges: register(b2)
////{
////	// Global
////	float4				cb_AmbientLightColor: packoffset(c0);
////	float4				cb_DiffuseLightColor: packoffset(c1);
////	float3				cb_SpecularLightColor: packoffset(c2);
////	float				cb_SpecularPower : packoffset(c2.w);
////};
////
////struct PNCT2_VS_INPUT
////{
////	float4 p		: POSITION;
////	float3 n		: NORMAL;
////	float4 c		: COLOR0;
////	float2 t		: TEXCOORD0;
////	float3 Tan		: TANGENT;
////};
////
////struct PCT4_PS_INPUT
////{
////	float4 p			: SV_POSITION;
////	float4 c			: COLOR0;
////	float2 t			: TEXCOORD0;
////	float3 vEye			: TEXCOORD1;
////	float3 vHalf		: TEXCOORD2;
////	float3 vLightDir	: TEXCOORD3;
////};
////
////
//////--------------------------------------------------------------------------------------
////// Vertex Shader
//////--------------------------------------------------------------------------------------
////PCT4_PS_INPUT VS(PNCT2_VS_INPUT input)
////{
////	PCT4_PS_INPUT output = (PCT4_PS_INPUT)0;
////	float4 vWorldPos = mul(input.p, g_matWorld);
////	float4 vViewPos = mul(vWorldPos, g_matView);
////	output.p = mul(vViewPos, g_matProj);
////
////	output.t = input.t*30.0f;
////	output.c = input.c;
////
////	float3 vNormal = normalize(mul(input.n, (float3x3)g_matNormal));
////	output.vEye = normalize(cb_vEyeDir);
////
////	float3 T = normalize(mul(input.Tan, (float3x3)g_matNormal));
////	float3 B = normalize(cross(vNormal, T));
////
////	float3x3 tanMat = { T.x, B.x, vNormal.x,
////		T.y, B.y, vNormal.y,
////		T.z, B.z, vNormal.z };
////	float3 vLightDir = cb_vLightVector.xyz;// normalize(cb_vLightVector.xyz - vWorldPos);
////	output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), tanMat));
////	output.vLightDir = normalize(mul(-vLightDir, tanMat));
////	output.vEye = normalize(mul(output.vEye, tanMat));
////	return output;
////}
//
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 PS(PCT4_PS_INPUT input) : SV_Target
//{
//	// 기본 텍스쳐 컬러 
//	/*	float4 normal		=	g_txNormalMap.Sample( g_samLinear, input.t );
//	float1 x			=	input.t.x+normal.x*0.1f*cb_vLightVector.z;
//	float1 y			=	input.t.y+normal.y*0.1f*cb_vLightVector.x;
//	float2 uv			=	float2(input.t.y, y);
//	*/
//	float4 DiffuseColor = g_txDiffuse.Sample(g_samLinear, input.t);
//
//	// 디퓨즈 조명 
//	float4 normal = g_txNormalMap.Sample(g_samLinear, input.t);
//	normal = normalize((normal - 0.5f) * 2.0f);
//	float  fDot = saturate(dot(normal.xyz, input.vLightDir));
//	float3 LightColor = cb_DiffuseLightColor.rgb * fDot;
//
//	// 스펙큘러 조명 		
//	float3 R = reflect(-input.vLightDir, normal.xyz);
//	float3 SpecColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R, input.vEye)), cb_SpecularPower);
//	//float3 SpecColor	=	cb_SpecularLightColor.rgb * pow( saturate(dot( input.vHalf, normal.xyz )), cb_SpecularPower );
//
//	// 전체 컬러 조합  	
//	float4 vFinalColor = DiffuseColor * float4(LightColor + SpecColor, 1.0f);
//	//float4 vFinalColor = DiffuseColor;
//	return vFinalColor;
//}
//float4 DEFAULT_PS(PCT4_PS_INPUT input) : SV_Target
//{
//	return g_txDiffuse.Sample(g_samLinear, input.t);
//}



//Texture2D g_txDiffuse: register (t0);
//Texture2D g_txSandDiffuse: register (t1);
//Texture2D g_txGrassDiffuse: register (t2);
//Texture2D g_txRockDiffuse: register (t3);
//Texture2D g_txSnowDiffuse: register (t4);
//
//SamplerState samLinear: register (s0);
//
//cbuffer cb0
//{
//	float4x4	g_matWorld	: packoffset(c0);
//	float4x4	g_matView		: packoffset(c4);
//	float4x4	g_matProj		: packoffset(c8);
//	float  g_fZonePlains : packoffset(c12.x);
//	float  g_fZoneHills : packoffset(c12.y);
//	float  g_fZoneAlpine : packoffset(c12.z);
//	float  g_fZoneMountain : packoffset(c12.w);
//};
//
////--------------------------------------------------------------------------------------
//struct VS_INPUT
//{
//	float4 p : POSITION;
//	float3 n : NORMAL;
//	float4 c : COLOR;
//	float2 t : TEXCOORD;
//};
//struct VS_OUTPUT
//{
//	float4 p : SV_POSITION;
//	float3 n : NORMAL;
//	float4 c : COLOR0;
//	float2 t : TEXCOORD0;
//};
//
////--------------------------------------------------------------------------------------
//// Vertex Shader
////--------------------------------------------------------------------------------------
//VS_OUTPUT VS(VS_INPUT vIn)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//	float4 vWorld = mul(vIn.p, g_matWorld);
//	output.p = mul(vWorld, g_matView);
//	output.p = mul(output.p, g_matProj);
//	output.n = vIn.n;
//	output.t = vIn.t * 100;
//	output.c = vIn.c;
//	//g_fMaxElev = 250*200
//	output.c.w = clamp(vIn.p.y / (255 * 20.0f), 0.0f, 1.0f);
//	return output;
//}
//
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 TexturePresenceSand(in float2 texCoord,
//	in float lowB, //-0.1
//	in float upB, // 0.3
//	in float opt, // 0.0f
//	in float elev) //0.3
//{
//	//  e < -0.1f || e > 0.3f
//	if ((elev < lowB) || (elev > upB))
//	{
//		return float4(0.0f, 0.0f, 0.0f, 0.0f);
//	}
//
//	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);
//
//	return k * g_txSandDiffuse.Sample(samLinear, texCoord);// tex2D(SandTextureSampler, texCoord);
//}
//
//float4 TexturePresenceGrass(in float2 texCoord,
//	in float lowB,
//	in float upB,
//	in float opt,
//	in float elev)
//{
//	if ((elev < lowB) || (elev > upB))
//	{
//		return float4(0.0f, 0.0f, 0.0f, 0.0f);
//	}
//
//	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);
//
//	return k * g_txGrassDiffuse.Sample(samLinear, texCoord); //tex2D(GrassTextureSampler, texCoord);
//}
//
//float4 TexturePresenceRock(in float2 texCoord,
//	in float lowB,
//	in float upB,
//	in float opt,
//	in float elev)
//{
//	if ((elev < lowB) || (elev > upB))
//	{
//		return float4(0.0f, 0.0f, 0.0f, 0.0f);
//	}
//
//	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);
//
//	return k * g_txRockDiffuse.Sample(samLinear, texCoord); //tex2D(RockTextureSampler, texCoord);
//}
//
//float4 TexturePresenceSnow(in float2 texCoord,
//	in float lowB,
//	in float upB,
//	in float opt,
//	in float elev)
//{
//	if ((elev < lowB) || (elev > upB))
//	{
//		return float4(0.0f, 0.0f, 0.0f, 0.0f);
//	}
//
//	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);
//
//	return k * g_txSnowDiffuse.Sample(samLinear, texCoord); //tex2D(SnowTextureSampler, texCoord);
//}
//
//
//float4 PS(VS_OUTPUT vIn) : SV_Target
//{
//	float4 vColor = float4(0,0,0,1);
//
//	vColor = TexturePresenceSand(vIn.t,	-0.1f,	g_fZoneHills,	g_fZonePlains,	vIn.c.w);
//	vColor += TexturePresenceGrass(vIn.t,g_fZonePlains,	g_fZoneAlpine,	g_fZoneHills,vIn.c.w);
//	vColor += TexturePresenceRock(vIn.t,g_fZoneHills,g_fZoneMountain,g_fZoneAlpine,	vIn.c.w);
//	vColor += TexturePresenceSnow(vIn.t,g_fZoneAlpine,100.1f,g_fZoneMountain,	vIn.c.w);
//
//	vColor *= vIn.c;
//	vColor.a = 1.0f;
//	return (g_txDiffuse.Sample(samLinear, vIn.t) + vIn.c.w) *vColor *float4(vIn.c.rgb, 1.0f);
//}
