#pragma once
//#include "AStd.h"
#include "ADxStd.h"
#include "ATimer.h"
#include "ADxRT.h"



class ADevice 
{
public:
	
	DX::ADxRT m_RT;

	float bcolor[4];

	ComPtr<ID3D11Device>					m_pd3dDevice;
	ComPtr<ID3D11DeviceContext>				m_pImmediateContext;

	ComPtr<ID3D11DeviceContext>				m_pDeferredContext;
	ComPtr<ID3D11CommandList>				m_pCommandList;

	ComPtr<IDXGISwapChain>					m_pSwapChain;
	ComPtr<IDXGIFactory>					m_pGIFactory;

	DXGI_SWAP_CHAIN_DESC					m_SwapChainDesc;
	UINT									m_FeatureLevel;

	BOOL									m_IsFullScreenMode;

public:
	HRESULT		InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen = FALSE);


public:
	HRESULT CreateDeferredContext();

	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain(HWND hWnd, UINT iwidth, UINT iheigh, BOOL IsFullScreen);
	HRESULT SetRenderTargetView();
	HRESULT SetViewPort();
	HRESULT ResizeDevice(UINT iwidth, UINT iheight);
	HRESULT	UpdataDepthStncilView();
	bool	ClearDevice();
	bool	PreRender();
	bool	PostRender();


	virtual		HRESULT		CreateDxResource();
	virtual		HRESULT		DeleteDxResource();


public:
	ADevice(void);
	virtual ~ADevice(void);
};

