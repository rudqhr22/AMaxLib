#pragma once
#include "ADxStd.h"
#include "AStd.h"


namespace DX
{
	
	class ADxState
	{
	public:
		static ComPtr<ID3D11RasterizerState> g_pRSWireFrame;
		static ComPtr<ID3D11RasterizerState> g_pRSSolid;
		static ComPtr<ID3D11RasterizerState> g_pRSBackCullSolid;
		static ComPtr<ID3D11RasterizerState> g_pRSNoneCullSolid;
		static ComPtr<ID3D11RasterizerState> g_pRSFrontCullSolid;
		static ComPtr<ID3D11RasterizerState> g_pRSSlopeScaledDepthBias;
			//static ID3D11RasterizerState* g_pRS[6];

		static ComPtr<ID3D11BlendState>	  g_pAlphaBlend;
		static ComPtr<ID3D11BlendState>	  g_pNoAlphaBlend;
		static ComPtr<ID3D11BlendState>	  g_pBSColorOne;
		static ComPtr<ID3D11BlendState>	  g_pBSOneOne;
		static ComPtr<ID3D11BlendState>	  g_pBSOneZero;
		static ComPtr<ID3D11BlendState>	  g_pBSAlphaOne;
		static ComPtr<ID3D11BlendState>	  g_pBSMaxOneOne;
		static ComPtr<ID3D11BlendState>	  g_pBSMaxZeroOne;
		//ZeroOne

		//static ID3D11BlendState*      g_pBS[7];

		static ComPtr<ID3D11SamplerState>	  g_pTexSS;
		static ComPtr<ID3D11SamplerState>    g_pSSWrapLinear;
		static ComPtr<ID3D11SamplerState>    g_pSSWrapPoint;
		static ComPtr<ID3D11SamplerState>    g_pSSMirrorLinear;
		static ComPtr<ID3D11SamplerState>    g_pSSMirrorPoint;
		static ComPtr<ID3D11SamplerState>    g_pSSClampLinear;
		static ComPtr<ID3D11SamplerState>    g_pSSClampPoint;
		static ComPtr<ID3D11SamplerState>    g_pSSShadowMap;
		static ComPtr<ID3D11SamplerState>    g_pSSAnisoTropic;
		//static ID3D11SamplerState*    g_pSS[8];

		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthEnable;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthDisable;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthEnableNoWrite;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthDisableNoWrite;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthStencilAdd;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthAlways;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSDepthStencilAlways;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSA;
		static  ComPtr<ID3D11DepthStencilState>	g_pDSSB;

		//static  ID3D11DepthStencilState*	g_pDSS[6];

		static HRESULT SetState(ComPtr<ID3D11Device>	pd3dDevice);
		static bool  Release();
	public:
		~ADxState();
	};

	static void ApplyRS(ComPtr<ID3D11DeviceContext>  pContext,
		ComPtr<ID3D11RasterizerState> pState)
	{
		pContext->RSSetState(pState.Get());
	}
	static void ApplyDSS(ComPtr<ID3D11DeviceContext>   pContext,
		ComPtr<ID3D11DepthStencilState>	pDepthStencilState,
		UINT iRef = 0x01)
	{
		pContext->OMSetDepthStencilState(pDepthStencilState.Get(), iRef);

	};
	static void ApplyBS(ComPtr<ID3D11DeviceContext>   pContext,
		ComPtr<ID3D11BlendState>     pBlendState,
		const FLOAT fBlendFactor[] = 0,
		UINT iMask = 0xffffffff)
	{
		pContext->OMSetBlendState(pBlendState.Get(), fBlendFactor, iMask);
	}

	static void ApplySS(ComPtr<ID3D11DeviceContext>   pContext, ComPtr<ID3D11SamplerState>      pSamplerState,
		UINT iSlot = 0, UINT iArray = 1)
	{
		pContext->PSSetSamplers(iSlot, iArray, &pSamplerState);
	}
}

