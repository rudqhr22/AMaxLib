#include "ADxRT.h"



namespace DX
{

	void ADxRT::SetViewPort(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth)
	{
		m_ViewPort.TopLeftX = TopLeftX;
		m_ViewPort.TopLeftY = TopLeftY;
		m_ViewPort.Width = Width;
		m_ViewPort.Height = Height;
		m_ViewPort.MinDepth = MinDepth;
		m_ViewPort.MaxDepth = MaxDepth;
		D3DXMatrixPerspectiveFovLH(&m_matProj, (float)D3DX_PI * 0.25f, Width / Height, 0.1f, 1000.0f);
	}

	HRESULT ADxRT::Create(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height)
	{
		HRESULT hr = S_OK;
		SetViewPort(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

		m_TexDesc.Width = (UINT)Width;
		m_TexDesc.Height = (UINT)Height;
		m_TexDesc.MipLevels = 1;
		m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_TexDesc.SampleDesc.Count = 1;
		m_TexDesc.SampleDesc.Quality = 0;
		m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
		m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		m_TexDesc.CPUAccessFlags = 0;
		m_TexDesc.MiscFlags = 0;
		m_TexDesc.ArraySize = 1;

		if (FAILED(hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf())))
		{
			return hr;
		}
		if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf())))
		{
			return hr;
		}
		if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRTV.GetAddressOf())))
		{
			return hr;
		}


		// ������� ũ�Ⱑ ����Ǹ� �ݵ�� ���̽��ٽ� �� �ٽ� �ۼ��Ǿ�� �Ѵ�.		
		if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height)))
		{
			return hr;
		}
		return hr;
	}


	HRESULT ADxRT::UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height)
	{
		HRESULT hr;
		if (m_pDSV != nullptr)
		{
			m_pDSV.ReleaseAndGetAddressOf();
		}

		m_ViewPort.Width = Width;
		//m_ViewPort.Height = Height;	//�ּ� ���ϸ� ���̸� �۾���

		ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
		D3D11_TEXTURE2D_DESC DescDepth;
		ZeroMemory(&DescDepth, sizeof(DescDepth));

		DescDepth.Width = Width;
		DescDepth.Height = Height;

		DescDepth.MipLevels = 1;
		DescDepth.ArraySize = 1;
		DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DescDepth.SampleDesc.Count = 1;
		DescDepth.SampleDesc.Quality = 0;
		DescDepth.Usage = D3D11_USAGE_DEFAULT;

		// �� ���� ���� �� ���ٽ� ���� ����
		if (DescDepth.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		{
			DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}
		else // ���̸� ���� ���̸� ����
		{
			DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			DescDepth.CPUAccessFlags = 0;
			DescDepth.MiscFlags = 0;
		}

		if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
		{
			return hr;
		}

		///// ���̴� ���ҽ� ���� : ���� �� �����쿡�� ����Ѵ�. ///
		//D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		switch (DescDepth.Format)
		{
		case DXGI_FORMAT_R32_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		}
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		if (FAILED(hr = pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, m_pDSV.GetAddressOf())))
		{
			return hr;
		}

		// ���̸� �� ��츸 �����Ѵ�.
		if (DescDepth.Format == DXGI_FORMAT_R32_TYPELESS)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			pDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, &m_pDSV_SRV);
		}

		m_pDSV->GetDesc(&dsvDesc);
		return hr;
	}

	void ADxRT::Apply(ID3D11DeviceContext*    pImmediateContext,
		ID3D11RenderTargetView* pRTV,
		ID3D11DepthStencilView* pDepthStencilView,
		bool bDepthClear, bool bStencilClear)
	{
		assert(pImmediateContext);
		assert(pRTV);
		ID3D11RenderTargetView* pNullRTV = NULL;
		pImmediateContext->OMSetRenderTargets(1, &pNullRTV, NULL);

		if (pRTV != NULL)
		{
			if (pDepthStencilView != NULL)
				pImmediateContext->OMSetRenderTargets(1, &pRTV, pDepthStencilView);
			else
				pImmediateContext->OMSetRenderTargets(1, &pRTV, m_pDSV.Get());
		}
		else
		{
			if (pDepthStencilView != NULL)
			{
				pImmediateContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), pDepthStencilView);
			}
			else
			{
				pImmediateContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());
			}
		}
		if (bDepthClear)
		{
			if (pDepthStencilView != NULL)
				pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
			else
				pImmediateContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);

		}
		if (bStencilClear)
		{
			if (pDepthStencilView != NULL)
				pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0, 0);
			else
				pImmediateContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
		}
		pImmediateContext->RSSetViewports(1, &m_ViewPort);
	}



	bool	ADxRT::Begin(ID3D11DeviceContext*    pContext, D3DXVECTOR4 vColor, bool bTargetClear, bool bDepthClear, bool bStencilClear)
	{
		m_nViewPorts = 1;
		pContext->RSGetViewports(&m_nViewPorts, m_vpOld);
		pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);

		ID3D11RenderTargetView* pNullRTV = NULL;
		pContext->OMSetRenderTargets(1, &pNullRTV, NULL);
		pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());

		Clear(pContext, vColor, bTargetClear, bDepthClear, bStencilClear);
		pContext->RSSetViewports(1, &m_ViewPort);
		return true;
	}

	bool	ADxRT::Clear(ID3D11DeviceContext*    pContext, D3DXVECTOR4 vColor, bool bTargetClear, bool bDepthClear, bool bStencilClear)
	{
		if (bTargetClear)
		{
			pContext->ClearRenderTargetView(m_pRTV.Get(), vColor);
		}
		if (bDepthClear)
		{
			pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
		}
		if (bStencilClear)
		{
			pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
		}
		pContext->RSSetViewports(1, &m_ViewPort);
		return true;
	}

	bool	ADxRT::End(ID3D11DeviceContext*    pContext)
	{
			pContext->RSSetViewports(m_nViewPorts, m_vpOld);
			pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
			m_pOldRTV->Release();
			m_pOldDSV->Release();
			if (m_pOldRTV) { (m_pOldRTV)->Release(); (m_pOldRTV) = NULL; }
			if (m_pOldDSV) { (m_pOldDSV)->Release(); (m_pOldDSV) = NULL; }
		return true;
	}


	ADxRT::ADxRT()
	{
		m_pRTV = nullptr;
		m_pDSV = nullptr;
		m_pSRV = nullptr;
		m_pTexture = nullptr;
		m_DSFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

}


