#include "ACore.h"
HDC		g_hOffScreenDC;
HDC		g_hScreenDC;


void ACore::SwapCamera(ACamera* pCamera) 
{ 
	m_pMainCamera = pCamera;
};		
														
void ACore::DefaultCamera() 
{
	m_pMainCamera = &m_Camera;
};


bool ACore::PreInit()
{
	I_TIME.Init();
	I_INPUT.Init();
	m_Font.Init();

	InitDevice(m_hwnd, g_iClientWidth, g_iClientHeight);
	DX::ADxState::SetState(m_pd3dDevice);
	m_AxisLine.Set(m_pd3dDevice.Get(), _T("../../Data/Shader/Line.hlsl"), 0,true);

	m_Camera.SetViewMatrix(D3DXVECTOR3(0.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pMainCamera = &m_Camera;
	return true;
}

bool ACore::PostInit()
{
	//m_pSwapChaion->GetBuffer()
	//m_write.CreateDeviceResource



	//m_pMainCamera = make_shared<ACamera>();

	return true;
}


bool ACore::PreFrame()
{
	I_INPUT.Frame();
	I_TIME.Frame();
	m_AxisLine.SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());



	return true;
}

bool ACore::PostFrame()
{
	//m_pImmediateContext.Get()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pMainCamera->Frame();
	return true;
}

bool  ACore::PreRender()
{
	//m_RT.Apply(m_pImmediateContext.Get(), m_RT.m_pRTV.Get(), m_RT.m_pDSV.Get(), true, false);
	//m_RT.Clear(m_pImmediateContext.Get(), bcolor, true, true, false);
	ADevice::PreRender();
	//WrapLinear

	return true;
}

bool  ACore::PostRender()
{
	DX::ApplySS(g_pImmediateContext.Get(), DX::ADxState::g_pSSWrapPoint);	//g_pSSWrapLinear ,g_pSSWrapPoint
	DX::ApplyBS(g_pImmediateContext.Get(), DX::ADxState::g_pAlphaBlend);
	DX::ApplyRS(m_pImmediateContext.Get(), DX::ADxState::g_pRSBackCullSolid); // g_pRSSolid

	DX::ApplyDSS(g_pImmediateContext.Get(), DX::ADxState::g_pDSSDepthEnable);

	if (I_INPUT.KeyCheck(DIK_F1))
	{
		DX::ApplyRS(m_pImmediateContext.Get(), DX::ADxState::g_pRSNoneCullSolid);
	}
	if (I_INPUT.KeyCheck(DIK_F2))
	{
		DX::ApplyRS(m_pImmediateContext.Get(), DX::ADxState::g_pRSWireFrame);
	}
	//if (I_INPUT.KeyCheck(DIK_F3))
	//{
	//	DX::ApplyRS(m_pImmediateContext.Get(), DX::ADxState::g_pRSFrontCullSolid);
	//}


	//if (I_INPUT.KeyCheck(DIK_F4))
	//{
	//	DX::ApplyDSS(m_pImmediateContext.Get(), DX::ADxState::g_pDSSDepthDisable);
	//}



	ADevice::PostRender();
	return true;
}


bool	ACore::DrawDebug()
{
	wchar_t pBuffer[256];
	//memset(pBuffer, 0, sizeof(LPCWSTR) * 256);
	_stprintf_s(pBuffer, _T("FPS: %d  SecPerFrame: %10.5f Elapsed: %10.2f"),
		I_TIME.GetFPS(), I_TIME.GetSPF(), I_TIME.GetElapsedTime());

	m_Font.Begin();
	////==버퍼(타이머) 폰트 출력
	m_Font.m_pTextFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_Font.m_pTextFormat.Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	RECT rc1 = { 0,0, m_rtWindow.right ,m_rtWindow.bottom };
	m_Font.DrawTextW(rc1, pBuffer, D2D1::ColorF(0.5, 0.5, 0.5, 1)); //pBuffer =시간 얻은값

																	//														   //우측 폰트 출력
																	////m_Font.m_pTextFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
																	////m_Font.m_pTextFormat.Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
																	////RECT rc2 = { 0,0, m_rtWindow.right ,m_rtWindow.bottom / 2 };
																	////m_Font.DrawTextW(rc2, "X축 우측 정렬, Y축 하단 정렬", D2D1::ColorF(1, 1, 1, 1));
	m_Font.End();
	m_AxisLine.Render(m_pImmediateContext.Get());
	return true;
}



bool ACore::PostRelease()
{
	I_TIME.Release();
	I_INPUT.Release();
	DX::ADxState::Release();
	//m_Font.Release();
	return true;
}

HRESULT ACore::CreateDxResource()
{
	IDXGISurface1*		pBackBuffer = NULL;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
	m_Font.Set(g_hWnd, m_rtWindow.right, m_rtWindow.bottom, pBackBuffer);
	
	if (pBackBuffer)	pBackBuffer->Release();

	if (FAILED(hr = m_pSwapChain->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}
	if (&m_Camera != NULL)
	{
		m_Camera.SetProjMatrix((float)D3DX_PI * 0.25f, m_SwapChainDesc.BufferDesc.Width / (FLOAT)m_SwapChainDesc.BufferDesc.Height, 1.0f, 10000.0f);
	}

	m_SwapChainDesc.BufferDesc.Width;
	m_SwapChainDesc.BufferDesc.Height;

	if (FAILED(hr = m_RT.UpdateDepthStencilView(m_pd3dDevice.Get(), m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height)))
	{
		return hr;
	}

//	UpdataDepthStncilView();
	return S_OK;
}

HRESULT ACore::DeleteDxResource()
{
	m_Font.Release();

	return S_OK;
}



ACore::ACore()
{

}


ACore::~ACore()
{

}



