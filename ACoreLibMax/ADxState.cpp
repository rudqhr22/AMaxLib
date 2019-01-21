#include "ADxState.h"
//정적클래스

namespace DX
{
	ComPtr<ID3D11RasterizerState> ADxState::g_pRSWireFrame = 0;
	ComPtr<ID3D11RasterizerState> ADxState::g_pRSSolid = 0;
	ComPtr<ID3D11RasterizerState> ADxState::g_pRSBackCullSolid = 0;
	ComPtr<ID3D11RasterizerState> ADxState::g_pRSNoneCullSolid = 0;
	ComPtr<ID3D11RasterizerState> ADxState::g_pRSFrontCullSolid = 0;
	ComPtr<ID3D11RasterizerState> ADxState::g_pRSSlopeScaledDepthBias;
//	ID3D11RasterizerState* ADxState::g_pRS[6] = { 0, };

	ComPtr<ID3D11SamplerState>	 ADxState::g_pTexSS = 0;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSWrapLinear = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSWrapPoint = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSMirrorLinear = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSMirrorPoint = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSClampLinear = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSClampPoint = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSShadowMap = NULL;
	ComPtr<ID3D11SamplerState>  ADxState::g_pSSAnisoTropic = NULL;
	//ID3D11SamplerState*  ADxState::g_pSS[8] = { 0, };

	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthEnable = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthDisable = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthEnableNoWrite = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthDisableNoWrite = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthStencilAdd = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthAlways = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSDepthStencilAlways = 0;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSA;
	ComPtr<ID3D11DepthStencilState>	ADxState::g_pDSSB;

	//ID3D11DepthStencilState*	ADxState::g_pDSS[6] = { 0, };

	ComPtr<ID3D11BlendState>	  ADxState::g_pAlphaBlend = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pNoAlphaBlend = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pBSColorOne = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pBSOneOne = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pBSOneZero = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pBSAlphaOne = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pBSMaxOneOne = 0;
	ComPtr<ID3D11BlendState>	  ADxState::g_pBSMaxZeroOne = 0;
	//ID3D11BlendState*	  ADxState::g_pBS[7] = { 0, };




	HRESULT ADxState::SetState(ComPtr<ID3D11Device>	pd3dDevice)
	{
		if (pd3dDevice == nullptr) return S_OK;
		if (g_pTexSS != nullptr) return S_OK;

		HRESULT hr = S_OK;
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		// 필터링 방식
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		// 텍스처 주소 지정 방식
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MaxLOD = FLT_MAX;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = FLT_MIN;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pTexSS)))
			return hr;

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;			//사용할 것이면 TRUE
		//////////////////////////////////////////////////////////////////////
		//컬러블랜딩 
		//결과 = 소스컬러 Op 대상컬러
		//소스 = 지금 뿌려질 것
		//대상 = 컬러클리어(처음) -> 이미 뿌려진 결과
		//R,G,B 서로 각각 연상이 이루어진다.
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//연산
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//원본
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//소스
		//////////////////////////////////////////////////////////////////////
		//알파블랜딩
		//결과  = 소스알파 BlendOp 대상알파
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		//연산
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//원본
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//소스

		//RGB 선택적으로 제어가능
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;				
		pd3dDevice->CreateBlendState(&blendDesc, &g_pAlphaBlend);



		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		pd3dDevice->CreateBlendState(&blendDesc, &g_pNoAlphaBlend);




		// DECAL BLENDING ONE, INV_SRC_ALPHA , ADD
		// ADDIVIVE BLENDING ONE, ONE, ADD
		// SUBRACITIVE BLENDING ONE, ONE REV_SUBTRACT
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSColorOne);

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSOneOne);

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSOneZero);

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSAlphaOne);
		////////////////////////////////////////////////////////////////////////
		//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		//blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//원본
		//blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//소스
		//pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSMaxZeroOne);//ZeroOne
		////////////////////////////////////////////////////////////////////////

		blendDesc.AlphaToCoverageEnable = TRUE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSMaxOneOne);


		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &ADxState::g_pBSMaxZeroOne);//ZeroOne 


		//레스터라이즈 상태 객체 생성
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(rsDesc));
		rsDesc.DepthClipEnable = TRUE;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &ADxState::g_pRSBackCullSolid))) return hr;


		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_BACK;// D3D11_CULL_NONE;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &ADxState::g_pRSWireFrame)))
			return hr;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &ADxState::g_pRSSolid)))
			return hr;

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &ADxState::g_pRSNoneCullSolid)))
			return hr;


		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_FRONT;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &ADxState::g_pRSFrontCullSolid)))
			return hr;

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		rsDesc.DepthBias = 10000;
		rsDesc.DepthBiasClamp = 0.0f;
		rsDesc.SlopeScaledDepthBias = 1.0f;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &ADxState::g_pRSSlopeScaledDepthBias)))
			return hr;

		//g_pRS[0] = g_pRSNoneCullSolid.Get();
		//g_pRS[1] = g_pRSFrontCullSolid.Get();
		//g_pRS[2] = g_pRSBackCullSolid.Get();


		ID3D11SamplerState* pSamplerState = nullptr;
		D3D11_SAMPLER_DESC sd;
		memset(&sd, 0, sizeof(sd));
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.MaxLOD = FLT_MAX;
		sd.MinLOD = FLT_MIN;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSWrapLinear)))
		{
			return hr;
		}
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSWrapPoint)))
		{
			return hr;
		}

		sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSMirrorLinear)))
		{
			return hr;
		}


		sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSMirrorPoint)))
		{
			return hr;
		}


		sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSClampLinear)))
		{
			return hr;
		}

		sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSClampPoint)))
		{
			return hr;
		}


		D3D11_SAMPLER_DESC SamDescShad =
		{
			D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,// D3D11_FILTER Filter;
			D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressW;
			0,//FLOAT MipLODBias;
			0,//UINT MaxAnisotropy;
			D3D11_COMPARISON_LESS , //D3D11_COMPARISON_FUNC ComparisonFunc;
			0.0,0.0,0.0,0.0,//FLOAT BorderColor[ 4 ];
			0,//FLOAT MinLOD;
			0//FLOAT MaxLOD;   
		};
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&SamDescShad, &g_pSSShadowMap)))
		{
			return hr;
		}


		sd.Filter = D3D11_FILTER_ANISOTROPIC;
		sd.MaxAnisotropy = 16;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSAnisoTropic)))
		{
			return hr;
		}



		//g_pSS[0] = g_pSSWrapLinear.Get();
		//g_pSS[1] = g_pSSWrapPoint.Get();
		//g_pSS[2] = g_pSSMirrorLinear.Get();
		//g_pSS[3] = g_pSSMirrorPoint.Get();
		//g_pSS[4] = g_pSSClampLinear.Get();
		//g_pSS[5] = g_pSSClampPoint.Get();
		//g_pSS[6] = g_pSSShadowMap.Get();
		//g_pSS[7] = g_pSSAnisoTropic.Get();

		// 깊이버퍼 상태값 세팅
		D3D11_DEPTH_STENCIL_DESC dsDescDepth;
		ZeroMemory(&dsDescDepth, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		
		dsDescDepth.StencilEnable = FALSE;
		dsDescDepth.StencilReadMask = 1;  //원본그대로	//0~255
		dsDescDepth.StencilWriteMask = 1; //원본그대로	//0~255   
		
		//스텐실 앞면 ||  뒷면
		//뎁스버퍼 이후에 발생
		dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;	//성공시
		dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//실패시
		dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; //뎁스실패시

		dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthEnable)))
		{
			return hr;
		}
		// 깊이 버퍼 비교 비 활성화.
		dsDescDepth.DepthEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthDisable)))
		{
			return hr;
		}
		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_ALWAYS;//DepthEnable = TRUE 이어야 한다.
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthAlways)))
		{
			return hr;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////
		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		dsDescDepth.StencilEnable = TRUE;
		dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;// 앞면 비교 함수
		dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		
		dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; // 뒷면 비교 함수
		dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthStencilAdd)))
		{
			return hr;
		}

		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.StencilEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthEnableNoWrite)))
		{
			return hr;
		}
		// 깊이 버퍼 비교 비 활성화.
		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDescDepth.DepthEnable = FALSE;
		dsDescDepth.StencilEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthDisableNoWrite)))
		{
			return hr;
		}

		dsDescDepth.DepthEnable = TRUE;		//Z버퍼;
		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_ALWAYS;
		dsDescDepth.StencilEnable = TRUE;

		dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
		dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthStencilAlways)))
		{
			return hr;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		ZeroMemory(&dsDescDepth, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsDescDepth.DepthEnable = FALSE;
		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDescDepth.StencilEnable = TRUE;
		dsDescDepth.StencilReadMask = 0xff;
		dsDescDepth.StencilWriteMask = 0xff;
		dsDescDepth.FrontFace.StencilFunc =			D3D11_COMPARISON_ALWAYS;
		dsDescDepth.FrontFace.StencilDepthFailOp =			D3D11_STENCIL_OP_KEEP;
		dsDescDepth.FrontFace.StencilFailOp =			D3D11_STENCIL_OP_KEEP;
		dsDescDepth.FrontFace.StencilPassOp =			D3D11_STENCIL_OP_INCR;
		dsDescDepth.BackFace.StencilFunc =			D3D11_COMPARISON_ALWAYS;
		dsDescDepth.BackFace.StencilDepthFailOp =			D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilFailOp =			D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilPassOp =			D3D11_STENCIL_OP_KEEP;

		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth,		g_pDSSA.GetAddressOf())))
		{
			return false;
		}

		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.StencilEnable = TRUE;
		dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;// D3D11_COMPARISON_LESS;
		dsDescDepth.FrontFace.StencilDepthFailOp =		D3D11_STENCIL_OP_KEEP;
		dsDescDepth.FrontFace.StencilFailOp =			D3D11_STENCIL_OP_KEEP;
		dsDescDepth.FrontFace.StencilPassOp =			D3D11_STENCIL_OP_KEEP;

		dsDescDepth.BackFace.StencilFunc =				D3D11_COMPARISON_ALWAYS;
		dsDescDepth.BackFace.StencilDepthFailOp =		D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilFailOp =			D3D11_STENCIL_OP_KEEP;
		dsDescDepth.BackFace.StencilPassOp =			D3D11_STENCIL_OP_KEEP;

		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth,		g_pDSSB.GetAddressOf())))
		{
			return false;
		}




		return hr;
	}






	bool ADxState::Release()
	{
		//if (g_pBSColorOne)		g_pBSColorOne->Release(); g_pBSColorOne = NULL;
		//if (g_pTexSS)		g_pTexSS->Release(); g_pTexSS = NULL;
		//if (g_pAlphaBlend)	g_pAlphaBlend->Release(); g_pAlphaBlend = NULL;
		//if (g_pNoAlphaBlend)g_pNoAlphaBlend->Release(); g_pNoAlphaBlend = NULL;
		//if (g_pBSOneOne)g_pBSOneOne->Release(); g_pBSOneOne = NULL;
		//if (g_pBSOneZero)g_pBSOneZero->Release(); g_pBSOneZero = NULL;
		//if (g_pBSAlphaOne)g_pBSAlphaOne->Release(); g_pBSAlphaOne = NULL;
		//if (g_pBSMaxOneOne)g_pBSMaxOneOne->Release(); g_pBSMaxOneOne = NULL;

		//if (g_pRSBackCullSolid)	g_pRSBackCullSolid->Release(); g_pRSBackCullSolid = NULL;
		//if (g_pRSWireFrame)		g_pRSWireFrame->Release(); g_pRSWireFrame = NULL;
		//if (g_pRSSolid)			g_pRSSolid->Release(); g_pRSSolid = NULL;
		//if (g_pRSNoneCullSolid)	g_pRSNoneCullSolid->Release(); g_pRSNoneCullSolid = NULL;
		//if (g_pRSFrontCullSolid)g_pRSFrontCullSolid->Release(); g_pRSFrontCullSolid = NULL;


		//if (g_pSSWrapLinear)	g_pSSWrapLinear->Release(); g_pSSWrapLinear = NULL;
		//if (g_pSSWrapPoint)		g_pSSWrapPoint->Release(); g_pSSWrapPoint = NULL;
		//if (g_pSSMirrorLinear)	g_pSSMirrorLinear->Release(); g_pSSMirrorLinear = NULL;
		//if (g_pSSMirrorPoint)	g_pSSMirrorPoint->Release(); g_pSSMirrorPoint = NULL;
		//if (g_pSSClampLinear)	g_pSSClampLinear->Release(); g_pSSClampLinear = NULL;
		//if (g_pSSClampPoint)	g_pSSClampPoint->Release(); g_pSSClampPoint = NULL;
		//if (g_pSSShadowMap)		g_pSSShadowMap->Release(); g_pSSShadowMap = NULL;
		//if (g_pRSSlopeScaledDepthBias)		g_pRSSlopeScaledDepthBias->Release(); g_pRSSlopeScaledDepthBias = NULL;

		//if (g_pDSSDepthEnable) g_pDSSDepthEnable->Release(); g_pDSSDepthEnable = NULL;
		//if (g_pDSSDepthDisable) g_pDSSDepthDisable->Release(); g_pDSSDepthDisable = NULL;
		//if (g_pDSSDepthEnableNoWrite) g_pDSSDepthEnableNoWrite->Release(); g_pDSSDepthEnableNoWrite = NULL;
		//if (g_pDSSDepthDisableNoWrite) g_pDSSDepthDisableNoWrite->Release(); g_pDSSDepthDisableNoWrite = NULL;
		//if (g_pDSSDepthStencilAdd) g_pDSSDepthStencilAdd->Release(); g_pDSSDepthStencilAdd = NULL;
		//if (g_pDSSDepthAlways) g_pDSSDepthAlways->Release(); g_pDSSDepthAlways = NULL;


		return true;
	}
	ADxState::~ADxState()
	{
		Release();
	}
	ADxState g_DxState;
}