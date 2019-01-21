#pragma once
#include "ADxStd.h"


namespace DX
{

	class ADxRT
	{


	public:
		ComPtr<ID3D11RenderTargetView> m_pRTV;
		ComPtr<ID3D11DepthStencilView> m_pDSV;
		ComPtr<ID3D11ShaderResourceView> m_pSRV;
		ComPtr<ID3D11ShaderResourceView> m_pRTV_SRV;
		ComPtr<ID3D11ShaderResourceView> m_pDSV_SRV;// ±Ì¿Ãπˆ∆€ ∏Æº“Ω∫∫‰
		ComPtr<ID3D11Texture2D> m_pTexture;

		D3D11_DEPTH_STENCIL_VIEW_DESC			dsvDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC			srvDesc;
		
		D3D11_TEXTURE2D_DESC					m_TexDesc;
		DXGI_FORMAT								m_DSFormat;
		D3D11_VIEWPORT							m_ViewPort;
		UINT					m_nViewPorts;

		D3DXMATRIX								m_matProj;



	public:
		D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];

		ID3D11RenderTargetView* m_pOldRTV;
		ID3D11DepthStencilView* m_pOldDSV;


	public:
		HRESULT	Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
	//	HRESULT CreateCubeMap(ID3D11Device*	pd3dDevice, FLOAT fWidth = 1024, FLOAT fHeight = 1024);


		void	SetViewPort(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
			FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
		HRESULT UpdateDepthStencilView(ID3D11Device* pDevice, UINT dwWidth, UINT dwHeight);
		void	Apply(ID3D11DeviceContext*    pImmediateContext,
			ID3D11RenderTargetView* pRTV = NULL,
			ID3D11DepthStencilView* pDepthStencilView = NULL,
			bool bDepthClear = true, bool bStencilClear = true);
		bool	Begin(ID3D11DeviceContext*    pImmediateContext, D3DXVECTOR4 vColor,
			bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
		bool	Clear(ID3D11DeviceContext*    pImmediateContext, D3DXVECTOR4 vColor,
			bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
		bool	End(ID3D11DeviceContext*    pImmediateContext);
		//	HRESULT SaveFile(ID3D11DeviceContext*    pContext, T_STR name);
		//	HRESULT SaveBackBuffer(ID3D11DeviceContext*    pContext, T_STR name);
		ADxRT();
		~ADxRT() {};
	};

}