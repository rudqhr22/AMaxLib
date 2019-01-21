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
	//////////����̽� �񵶸����̴�./////////
	ComPtr<ID2D1RenderTarget>		m_pRT;

	//////////����̽� �������̴�./////////
	ComPtr<ID2D1Factory>			m_pD2DFactory;
	//dwrite//
	ComPtr<IDWriteFactory>			m_pDWriteFactory;
	ComPtr<IDWriteTextFormat>		m_pTextFormat;
	//color
	ComPtr<ID2D1SolidColorBrush>	m_pSolidColoBrush;

public:
	//�ʱ�ȭ//
	bool	Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* pSurface);
	bool	Init();

	//������//
	bool	Begin();
	HRESULT	DrawText(RECT rc, const wchar_t* pText, D2D1::ColorF vColor = D2D1::ColorF(1, 0, 0, 1));
	bool				End();

	// ��ü�Ҹ�//
	bool				Release();

	//��ü���� �� �Ҹ�//
	HRESULT				CreateDeviceIndependentResources();		//��������
	void				DiscardDeviceIndependentResources();	//�����Ҹ�

	HRESULT				CreateDeviceResources(IDXGISurface1*	m_pSurface); //��������
	void				DiscardDeviceResources();							 //�����Ҹ�

	//ȭ��������
	void				OnReSize(UINT width, UINT height, IDXGISurface1*pSurface);
	//pSurface �����?

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



