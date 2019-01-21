#include "ADxWrite.h"
#include<math.h>
#pragma comment (lib, "winmm.lib")  



bool	ADxWrite::Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* pSurface)
{
	HRESULT hr;
	m_hWnd = hWnd;
	if (!Init())	
	{
		return false;
	}
	m_hWnd = hWnd;

	CreateDeviceIndependentResources();  //비독립
	CreateDeviceResources(pSurface);	 //독립

	//SetText(D2D1::Point2F(iWidth, iHeight), L"TBasisSample!", D2D1::ColorF(1, 1, 1, 1));

	return true;
}
//
//RectF(
//	FLOAT left = 0.f,
//	FLOAT top = 0.f,
//	FLOAT right = 0.f,
//	FLOAT bottom = 0.f

HRESULT	ADxWrite::DrawText(RECT rc, const wchar_t* pText, D2D1::ColorF vColor)
{
	if (m_pRT&&m_pSolidColoBrush.Get())
	{
		D2D1_RECT_F layoutRect = D2D1::RectF(
			//static_cast<FLOAT>
			(rc.left), // m_fDPIScaleX,
			//static_cast<FLOAT>
			(rc.top), // m_fDPIScaleY,
			//static_cast<FLOAT>
			(rc.right), // m_fDPIScaleX,
			//static_cast<FLOAT>
			(rc.bottom)); // m_fDPIScaleY);
	
		m_pSolidColoBrush.Get()->SetColor(vColor);
		m_pRT.Get()->DrawText(pText, wcslen(pText), m_pTextFormat.Get(), layoutRect, m_pSolidColoBrush.Get());
	}
	return S_OK;
}

//IdentityMatrix = Matrix3x2F(
//	FLOAT _11,
//	FLOAT _12,
//	FLOAT _21,
//	FLOAT _22,
//	FLOAT _31,
//	FLOAT _32

bool	ADxWrite::Begin()
{
	if (m_pRT)
	{
		m_pRT.Get()->BeginDraw();
		m_pRT.Get()->SetTransform(D2D1::IdentityMatrix());
	}
	return true;
}

bool	ADxWrite::End()
{
	if (m_pRT &&FAILED(m_pRT.Get()->EndDraw()))
	{
		return false;
	}
	return true;
}


bool	ADxWrite::Init()
{
	HRESULT hr;
	 hr =CreateDeviceIndependentResources();	

	return true;
}

bool	ADxWrite::Release()
{
	m_wszText.clear();
	m_wszFontFamily.clear();
	DiscardDeviceIndependentResources();	//비독립 소멸
	DiscardDeviceResources();				//독립소멸
	return true;
}

//객체생성 및 소멸//
HRESULT		ADxWrite::CreateDeviceIndependentResources()  //독립 생성 
{
	HRESULT hr;
	// Create Direct2D factory. 옵션은 싱글, 그리고 팩토리
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pD2DFactory.GetAddressOf());
	
	m_pD2DFactory.Get()->GetDesktopDpi(&m_fdpiX, &m_fdpiY);
	m_fDPIScaleX = m_fdpiX / 96.0f;
	m_fDPIScaleY = m_fdpiY / 96.0f;

	// Create a shared DirectWrite factory.
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_pDWriteFactory.GetAddressOf()));
	}
	// Create a text format using Gabriola with a font size of 72.
    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory.Get()->CreateTextFormat(
            L"궁서",                 // Font family name.
            NULL,                    // Font collection (NULL sets it to use the system font collection).
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20.0f,
            L"ko-kr",				//L"en-us"
            m_pTextFormat.GetAddressOf());
    }   
	return hr;
}

void		ADxWrite::DiscardDeviceIndependentResources() //독립 해제
{ 
	m_pD2DFactory.Get()->Release();
	m_pDWriteFactory.Get()->Release();
	m_pTextFormat.Get()->Release();
	
}


HRESULT		ADxWrite::CreateDeviceResources(IDXGISurface1*	pSurface) //비독립 생성
{
	HRESULT hr = S_OK;


	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		//static_cast<float>
		(m_fdpiX),
		//static_cast<float>
		(m_fdpiY));


	m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, (const D2D1_RENDER_TARGET_PROPERTIES *)&props, m_pRT.GetAddressOf());
	
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), m_pSolidColoBrush.GetAddressOf());
	

	return S_OK;
}

void		ADxWrite::DiscardDeviceResources()	//비독립 해제
{
	m_pRT.Get()->Release();
	m_pSolidColoBrush.Get()->Release();
}



//화면사이즈변경
void		ADxWrite::OnReSize(UINT width, UINT height, IDXGISurface1*pSurface)
{
	DiscardDeviceResources();
	CreateDeviceResources(pSurface);
//	SetText(D2D1::Point2F(width, height), L"TBasisSample!", D2D1::ColorF(1, 1, 1, 1));
}
 
ADxWrite::ADxWrite()
{
	m_pTextFormat.Get() == NULL;
	m_pSolidColoBrush.Get() == NULL;
	m_pDWriteFactory.Get() == NULL;
	m_pD2DFactory.Get() == NULL;
	m_pRT.Get() == NULL;
	m_hWnd = NULL;
}


ADxWrite::~ADxWrite()
{
	
}
