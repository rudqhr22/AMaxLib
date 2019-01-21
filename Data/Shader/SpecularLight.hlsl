#define WIDEN(x) g_##x
#define HALF_VECTOR
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
SamplerState g_samLinear: register (s0);
cbuffer cb0: register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
//Lighting Variables
//--------------------------------------------------------------------------------------
cbuffer cb1: register(b1)
{
	matrix				g_matWorldInverse: packoffset(c0);
	float4				g_cAmbientMaterial: packoffset(c4);
	float4				g_cDiffuseMaterial: packoffset(c5);
	float4				g_cSpecularMaterial: packoffset(c6);
	float4				g_cEmissionMaterial: packoffset(c7);

	float4				g_cAmbientLightColor : packoffset(c8);
	float4				g_cDiffuseLightColor: packoffset(c9);
	float4				g_cSpecularLightColor: packoffset(c10);
	float3				g_vLightDir : packoffset(c11);
	float			    g_fDamping : packoffset(c11.w);
	float3				g_vLightPos : packoffset(c12);
	float			    g_fRadius : packoffset(c12.w);
	float3				g_vEyeDir : packoffset(c13);
	float			    g_fIntensity : packoffset(c13.w);
	float3				g_vEyePos : packoffset(c14);
	float			    g_fEyeRadius : packoffset(c14.w);
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
	float4 v : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	float4 vWorld = mul(vIn.p, WIDEN(matWorld));
	vOut.p = mul(vWorld, WIDEN(matView));
	vOut.p = mul(vOut.p, WIDEN(matProj));
	vOut.n = normalize(mul(vIn.n, (float3x3)g_matWorldInverse));
	vOut.t = vIn.t * 10;
	vOut.c = vIn.c * g_MeshColor;
	vOut.v = vWorld;
	return vOut;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 Diffuse(float3 vNormal)
{
	float fIntensity = max(0, dot(vNormal, normalize(-g_vLightDir)));
	float4 diffuse = g_cAmbientMaterial * g_cAmbientLightColor +
		(g_cDiffuseMaterial * g_cDiffuseLightColor * fIntensity);
	return diffuse;
}
float4 Specular(float3 vNormal)
{
	// Specular Lighting
	float  fPower = 0.0f;
#ifndef HALF_VECTOR
	float3 R = reflect(g_vLightDir, vNormal);
	fPower = pow(saturate(dot(R, -g_vEyeDir)), g_fIntensity);
#else
	float3 vHalf = normalize(-g_vLightDir + -g_vEyeDir);
	fPower = pow(saturate(dot(vNormal, vHalf)), g_fIntensity);
#endif
	float4 specular = g_cSpecularMaterial * g_cSpecularLightColor * fPower;
	return specular;
}
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float4 vTexColor = g_txDiffuse.Sample(g_samLinear, vIn.t);
	float4 vFinalColor = vTexColor * (Diffuse(vIn.n) + Specular(vIn.n)) * vIn.c;
	vFinalColor.a = 1.0f;
	return vTexColor *(Diffuse(vIn.n) + Specular(vIn.n));
}
float4 PS_Texture(VS_OUTPUT vIn) : SV_Target
{
	return g_txDiffuse.Sample(g_samLinear, vIn.t);
}
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
	float4 vColor = Specular(vIn.n);
	vColor.a = 1.0f;
	return vColor;
}
float4 Rim( VS_OUTPUT vIn)
{
	float fRimLightcolor = smoothstep(1.0f - vIn.v.w, 1.0f, 1.0f - max(0, dot(vIn.n, vIn.v.xyz)));
	float4 vFinalColor = float4(fRimLightcolor, fRimLightcolor, fRimLightcolor, 1.0f);	
	return vFinalColor;
}
float Geom(VS_OUTPUT vIn)
{
	float3 vReflect = normalize(-g_vLightDir + -g_vEyeDir);
	float fDot  = pow(saturate(dot(vIn.n, vReflect)), 1);

	float NdotL = saturate(dot(vIn.n, -g_vLightDir));
	float NdotH = saturate(dot(vIn.n, vReflect));
	float NdotV = saturate(dot(vIn.n, -g_vEyeDir));
	float VdotH = saturate(dot(-g_vEyeDir, vReflect));

	float Geom = min(min(2 * (NdotH*NdotV / VdotH), 2 * (NdotH*NdotL / VdotH)), 1.0f) / max(0.1f, NdotV);
	return Geom;
}