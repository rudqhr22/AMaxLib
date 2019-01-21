#include "ADevice.h"
#include "ADxState.h"

UINT g_iClientWidth;
UINT g_iClientHeight;
ComPtr<ID3D11Device>			g_pd3dDevice;
ComPtr<ID3D11DeviceContext>		g_pImmediateContext;
ComPtr<ID3D11DeviceContext>		g_pDeferredContext;

HRESULT		ADevice::InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{
	HRESULT hr;
	if (FAILED(hr = CreateDevice()))
	{
		MessageBox(0, _T("CreateDevice  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}
	if (FAILED(hr = CreateGIFactory()))
	{
		MessageBox(0, _T("CreateGIFactory  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}


	if (FAILED(hr = CreateSwapChain(hWnd, iWidth, iHeight, IsFullScreen)))
	{
		MessageBox(0, _T("CreateSwapChain  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	if (FAILED(hr = UpdataDepthStncilView()))
	{
		MessageBox(0, _T("UpdataDepthStncilView  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	

	if (FAILED(hr = SetRenderTargetView()))
	{
		MessageBox(0, _T("SetRenderTargetView  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}


	m_RT.SetViewPort(m_pd3dDevice.Get(),0,0, g_iClientWidth, g_iClientHeight,0,1);

	//if (FAILED(hr = SetViewPort()))	{
	//	MessageBox(0, _T("SetViewPort  실패"), _T("Fatal error"), MB_OK);
	//	return hr;	}

	if (FAILED(hr = CreateDxResource()))
	{
		return hr;
	}


	// Alt + Enter 키를 막는다.
	if (FAILED(hr = m_pGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		return hr;
	}

	CreateDeferredContext();


	return hr;
}

HRESULT ADevice::CreateDeferredContext()
{
	HRESULT hr;

	D3D11_FEATURE_DATA_THREADING th;
	hr=m_pd3dDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &th, sizeof(th));
	hr = m_pd3dDevice->CreateDeferredContext(0, &m_pDeferredContext);
	g_pDeferredContext = m_pDeferredContext;

	//ID3D11CommandList * pd3dCommandList = NULL;
	//hr = m_pDeferredContext->FinishCommandList(FALSE, &m_pCommandList);

	//ID3D11DeviceContext*   g_pd3dDeferredContext[3] = { NULL }; 
	//for (int iInstance = 0; iInstance < 3; ++iInstance) 
	//{
	//	m_pd3dDevice->CreateDeferredContext(0, &g_pd3dDeferredContext[iInstance]));
	//}
	//ID3D11CommandList * pd3dCommandList = NULL;  
	//pDeferredContext->FinishCommandList(FALSE, &pd3dCommandList);
	//g_iClientWidth = m_SwapChainDesc.BufferDesc.Width;
	//g_iClientHeight = m_SwapChainDesc.BufferDesc.Height;
	return hr;
}

HRESULT ADevice::CreateDevice()
{
	HRESULT hr;
	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;				//디버깅 용도로 쓰겟다

#endif

	createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	hr = D3D11CreateDevice(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,						//createDeviceFlags
		&FeatureLevels,
		1,										//m_FeatureLevel			
		D3D11_SDK_VERSION,
		m_pd3dDevice.GetAddressOf(),
		NULL,
		m_pImmediateContext.GetAddressOf());

	


	g_pd3dDevice = m_pd3dDevice;
	g_pImmediateContext = m_pImmediateContext;





	return hr;
}

//현재 컴퓨터에 들어가 있는 정보
HRESULT ADevice::CreateGIFactory()
{
	//글로벌 DXGI를 가져오는 함수
	//if (FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory))))
	//{
	//	return false;
	//}

	//__uuidof 고유한 수치값으로 반환한다.

	HRESULT hr;
	IDXGIDevice * pDXGIDevice;
	//DXGI에 할당된 디바이스를 가져온다
	hr = m_pd3dDevice.Get()->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	//DXGI에 할당된 디바이스에 연결된 아답터를 가져온다
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	//	IDXGIFactory * pIDXGIFactory;	멤버로 있음
	//DXGI에 할당된 아답터에 연결된 DXGI팩토리를 가져온다
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)m_pGIFactory.GetAddressOf());

	pDXGIDevice->Release();
	pDXGIAdapter->Release();

	return hr;
}

HRESULT ADevice::CreateSwapChain(HWND hWnd, UINT iwidth, UINT iheigh, BOOL IsFullScreen)
{
	HRESULT hr;
	//DESC 만한 백버퍼가 만들어진다
	DXGI_SWAP_CHAIN_DESC	sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = iwidth;
	sd.BufferDesc.Height = iheigh;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferCount = 1;		//버퍼가 여러개 해도됨. 단 어느 걸 할지 선택 필요
	sd.OutputWindow = g_hWnd;

	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;	//백버퍼 갯수
	sd.SampleDesc.Quality = 0;

	sd.Windowed = TRUE;	//윈도우로 띄울래 말래

	sd.SampleDesc.Count = 1;										//픽셀 단위의 멀티 샘플링의 수 //주의할점 DXGI_SWAP_CHAIN_DESC 구조체 선언 후에 ZeoriMemoty함수로 초기화 할경우,반드시 Count=1으로 설정해야 한다.
	sd.SampleDesc.Quality = 0;										//이미지 품질 수준이 반환.		


	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	m_pGIFactory->CreateSwapChain(m_pd3dDevice.Get(), &sd, m_pSwapChain.GetAddressOf());

	if (FAILED(hr = m_pSwapChain->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}
	g_iClientWidth = m_SwapChainDesc.BufferDesc.Width;
	g_iClientHeight = m_SwapChainDesc.BufferDesc.Height;



	return true;
}



HRESULT ADevice::SetRenderTargetView()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D *pResource;

	hr = m_pSwapChain.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pResource); //비트맵이라 봐도 됨

	hr = m_pd3dDevice->CreateRenderTargetView(pResource, NULL, m_RT.m_pRTV.GetAddressOf());
	pResource->Release();		//사용처가 끝났으면 반드시 릴리즈를 해주어야 한다.

	hr = m_pSwapChain.Get()->GetDesc(&m_SwapChainDesc);

	hr = UpdataDepthStncilView();


	return hr;
}

//화면에 뿌릴 영역을 선택하는 것이 뷰포트
HRESULT ADevice::SetViewPort()
{
	//뷰포트 사이즈
	m_RT.m_ViewPort.Width = g_iClientWidth;
	m_RT.m_ViewPort.Height = g_iClientHeight;
	m_RT.m_ViewPort.MinDepth = 0.0f;
	m_RT.m_ViewPort.MaxDepth = 1.0f;
	
	m_RT.m_ViewPort.TopLeftX = 0;
	m_RT.m_ViewPort.TopLeftY = 0;

	//	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	return true;
}

HRESULT		ADevice::UpdataDepthStncilView()
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> pDepthStencilTexture = NULL;
	//ID3D11Texture2D* pDepthStencilTexture = NULL;
	D3D11_TEXTURE2D_DESC t2d;
	ZeroMemory(&t2d, sizeof(t2d));

	t2d.Width =  m_SwapChainDesc.BufferDesc.Width;
	t2d.Height = m_SwapChainDesc.BufferDesc.Height;

	t2d.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	t2d.Usage = D3D11_USAGE_DEFAULT;
	t2d.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	t2d.MipLevels = 1;
	t2d.ArraySize = 1;
	t2d.SampleDesc.Count = 1;
	t2d.SampleDesc.Quality = 0;

	m_pd3dDevice.Get()->CreateTexture2D(&t2d, NULL, &pDepthStencilTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC	dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = t2d.Format;								//텍스쳐랑 같은 포맷
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Texture2D.MipSlice = 0;

	m_pd3dDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), &dsvd, m_RT.m_pDSV.GetAddressOf());

	//SAFE_RELEASE(pDepthStencilTexture);

	return hr;
}






bool	ADevice::ClearDevice()
{
	/*m_pImmediateContext->ClearState();
	m_pRenderTargetView->Release();
	m_pSwapChain->Release();
	m_pImmediateContext->Release();
	m_pGIFactory->Release();
	*/
	//m_pd3dDevice = NULL;
	//m_pSwapChain = NULL;
	//m_pRenderTargetView = NULL;
	//m_pImmediateContext = NULL;
	//m_pGIFactory = NULL;


	return true;
}

bool  ADevice::PreRender()
{

	//bcolor[2] = (cosf(I_TIME.m_fAccTime) *0.5 )+ 0.5;
	m_RT.Apply(m_pImmediateContext.Get(), m_RT.m_pRTV.Get(), m_RT.m_pDSV.Get(), true, false);
//	m_pImmediateContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY);

	//m_RT.Begin(m_pImmediateContext.Get(), bcolor, true, true, false);
	m_RT.Clear(m_pImmediateContext.Get(), bcolor, true, true, false);
	
	//m_pImmediateContext.Get()->ClearRenderTargetView(m_pRenderTargetView.Get(), bcolor);
	//m_pImmediateContext.Get()->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//m_pImmediateContext.Get()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	//m_pImmediateContext.Get()->RSSetViewports(1, &m_ViewPort);	//여기서 바뀌어서 뿌릴 수도 있다.

	return true;
}


bool	ADevice::PostRender()
{

	m_pSwapChain.Get()->Present(0, 0);

	return true;
}




HRESULT ADevice::ResizeDevice(UINT iwidth, UINT iheight)
{
	HRESULT hr;
	if (m_pd3dDevice == NULL) return true;

	hr = DeleteDxResource();

	m_pImmediateContext.Get()->OMSetRenderTargets(0, NULL, NULL);

	m_RT.m_pRTV.ReleaseAndGetAddressOf();


	m_pSwapChain.Get()->GetDesc(&m_SwapChainDesc);

	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	m_pSwapChain.Get()->ResizeBuffers(m_SwapChainDesc.BufferCount,
		iwidth, iheight, m_SwapChainDesc.BufferDesc.Format,
		m_SwapChainDesc.Flags);

	g_rtWindow.right = m_SwapChainDesc.BufferDesc.Width;
	g_rtWindow.bottom = m_SwapChainDesc.BufferDesc.Height;



	if (FAILED(hr = SetRenderTargetView()))
	{
		return hr;
	}


	// 뷰포트를 세팅하고 적용한다.
	m_RT.SetViewPort(m_pd3dDevice.Get(), 0, 0, g_iClientWidth, g_iClientHeight, 0, 1);
	//if (FAILED(hr = SetViewPort()))
	//{
	//	return hr;
	//}

	if (FAILED(hr = CreateDxResource()))
	{
		return hr;
	}



	return S_OK;

}



HRESULT ADevice::CreateDxResource()
{
	return S_OK;
}
HRESULT ADevice::DeleteDxResource()
{
	return S_OK;
}



ADevice::ADevice()
{
	bcolor[0] = 0.3f;
	bcolor[1] = 0.3f;
	bcolor[2] = 0.3f;
	bcolor[3] = 1;


}


ADevice::~ADevice()
{
}
