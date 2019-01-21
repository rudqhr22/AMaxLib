#pragma once
#include "AStd.h"
#include "ADxStd.h"

class ADxWrite
{
public:

	float m_fDPIScaleX;
	float m_fDPIScaleY;
	FLOAT m_fdpiX;
	FLOAT m_fdpiY;
	HWND  m_hWnd;
	

	//d2d//
	//////////디바이스 비독립적이다./////////
	ComPtr<ID2D1RenderTarget>		m_pRT;

	//////////디바이스 독립적이다./////////
	ComPtr<ID2D1Factory>			m_pD2DFactory;
	//dwrite//
	ComPtr<IDWriteFactory>			m_pDWriteFactory;
	ComPtr<IDWriteTextFormat>		m_pTextFormat;
	//color
	ComPtr<ID2D1SolidColorBrush>	m_pSolidColoBrush;

public:
	//초기화//
	bool	Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* pSurface);
	bool	Init();

	//랜더링//
	bool	Begin();
	HRESULT	DrawText(RECT rc, const wchar_t* pText, D2D1::ColorF vColor = D2D1::ColorF(1, 0, 0, 1));
	bool				End();

	// 전체소멸//
	bool				Release();

	//객체생성 및 소멸//
	HRESULT				CreateDeviceIndependentResources();		//독립생성
	void				DiscardDeviceIndependentResources();	//독립소멸

	HRESULT				CreateDeviceResources(IDXGISurface1*	m_pSurface); //독립생성
	void				DiscardDeviceResources();							 //독립소멸

	//화면사이즈변경
	void				OnReSize(UINT width, UINT height, IDXGISurface1*pSurface);
	//pSurface 백버퍼?

//	HRESULT	SetText(D2D1_POINT_2F size, wchar_t *text, D2D1::ColorF vColor);

public:
	DWRITE_FONT_WEIGHT		m_fontWeight;
	DWRITE_FONT_STYLE		m_fontStyle;
	BOOL					m_fontUnderline;
	wstring					m_wszFontFamily;
	float					m_fontSize;
	wstring					m_wszText;
	UINT32					m_cTextLength;

public:
	ADxWrite();
	~ADxWrite();
};



