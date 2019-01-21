//정점쉐이더틑 정점버퍼의 정점1개의 단위로 처리된다.

struct VS_INPUT
{
	float4 p : POSITION;
	float2 t : TEXCOORD;
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD0;
};
typedef PS_INPUT VS_OUTPUT;

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = Input.p;
	output.t = Input.t;
	return output;
}



//float4 VS(in float3 p : POSITION) : SV_POSITION
//{
//	return float4(p.x,p.y,p.z,1.0f);
//}
//
//
//float4 PS(in float3 p : POSITION) : SV_POSITION
//{
//	return float4(p.x,p.y,p.z,1.0f);
//}