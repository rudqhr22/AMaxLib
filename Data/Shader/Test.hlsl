Texture2D g_txDiffuse  : register (t0);		//텍스처 슬롯0
SamplerState sample0   : register (s0);		//샘플러 슬롯0

cbuffer cb0
{
	float Color : packoffset(c0);
	float fTime : packoffset(c1.x);
}


struct VS_INPUT
{
	float4 p : POSITION;
//	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
//	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};


VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = Input.p;
	//output.p = Input.c;
	output.t = Input.t;// *fTime;
	return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	return  g_txDiffuse.Sample(sample0, input.t);
}




//샘플링 표본을 추출 한다.
//정점과 정점 사이에 해당하는 픽셍들 -> 픽셀들 사이 값을 uv값을 캐치해낸다.
//
//필터링 - 픽셀과 픽셀간격을 연산
//1.최근점  - 가장 근접한 픽셀을 출력
//2.리너 - 선형보간 필터링 (중간값을 찾아서 보간하겠다)	//끝으로 갈경우 결과가 이상할수 있다.
// 앞에선 픽셀 대 면적이크지만 멀리 갈 수록 면적이 작아진다. 그래서 보간결과가 달라진 수 밖에 없다.
//3.이방성 필터링 - 리너 보간 방법을 개선 한것()


//필터링이 들어갈떄와 안들어갈떄,
//MIN 축소 
//MAG 확대
//MIP 민맵

//샘플러 - 어떻게 샘플링을 할것인가.

//WRAP
//x방향 y방향 z방향