Texture2D		g_txDiffuse: register (t0);
Texture2D		g_txShadow1: register (t1);

SamplerState g_samLinear: register (s0);
SamplerState g_samShadowMap: register (s1);
cbuffer cb0: register (b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
cbuffer cb1: register (b1)
{
	float4x4		g_matShadow1;
}

struct PNCT_VS_INPUT
{
    float4 p		: POSITION;
    float3 n		: NORMAL;
    float4 c	    : COLOR0;
    float2 t		: TEXCOORD0;
};
struct PCT_VS_OUTPUT
{
    float4 p		: SV_POSITION;
    float4 c		: COLOR0;
    float2 t		: TEXCOORD0;
	float4 TexShadow1 : TEXCOORD1;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PCT_VS_OUTPUT VS( PNCT_VS_INPUT input )
{
    PCT_VS_OUTPUT output = (PCT_VS_OUTPUT)0;
    output.p = mul( input.p, g_matWorld );   
	output.p = mul( output.p, g_matView );
	output.p = mul( output.p, g_matProj );  
    output.t = input.t;
    output.c = input.c;
    // 텍스처좌표
	output.TexShadow1 = mul( input.p, g_matShadow1 );
    return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PCT_VS_OUTPUT input) : SV_Target
{
	//W성분으로 텍스처 좌표 X,Y를 나구고 텍스처 좌표로 사용한다.
	float4 shadow1  = g_txShadow1.Sample( g_samShadowMap, input.TexShadow1.xy / input.TexShadow1.w ); 
	float4 FinalColor = g_txDiffuse.Sample( g_samLinear, input.t );
	if( shadow1.r > 0.01f)
	{
		FinalColor = FinalColor * float4( 0.5f, 0.0f, 0.0f, 1.0f );
		FinalColor.w = 1.0f;
	}	
	return  FinalColor;
}

//float CalcShadowTermSoftPCF(float fLightDepth, float4 vTexCoord, int iSqrtSamples)
//{
//	float fShadowTerm = 0.0f;  
//		
//	float fRadius = (iSqrtSamples - 1.0f) / 2;
//	
//	float BIAS = 0.002f;
//	
//	for (float y = -fRadius; y <= fRadius; y++)
//	{
//		for (float x = -fRadius; x <= fRadius; x++)
//		{
//			float2 vOffset = 0;
//			vOffset = float2(x, y);				
//			vOffset /= cSize;
//			float4 vSamplePoint = vTexCoord + float4(vOffset, 0, 0);			
//			float fDepth = tex2Dproj(cShadowSampler, vSamplePoint).x; // ???
//				
//			fShadowTerm += fLightDepth <= fDepth + BIAS;
//		}											
//	}		
//	
//	fShadowTerm /= (iSqrtSamples * iSqrtSamples);
//	
//	return fShadowTerm;
//}
//--------------------------------------------------------------------------------------
