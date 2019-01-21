#include "AWindow.h"

static AWindow*		g_Window = NULL;

RECT				g_rtWindow;
HWND				g_hWnd;
HINSTANCE			g_hInstance;


bool	AWindow::WindowMessageRun()
{
	ZeroMemory(&m_Msg, sizeof(m_Msg));

	if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_Msg);
		DispatchMessage(&m_Msg);
		
		//MsgEvent(m_Msg);
		return true;
	}

	return false;
}

LRESULT WINAPI StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_Window->WindowMsgProc(hWnd, msg, wParam, lParam);	//자식타고 갈수 있게됨
}

int	AWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return -1;
}

LRESULT CALLBACK AWindow::WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int iReturn = WndProc(hWnd, msg, wParam, lParam);
	if (iReturn >= 0)		//이거 안하면 IME 리턴에서 바로 메시지 처리로 넘어버림
	{
		return iReturn;
	}
	switch (msg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_SIZE:
		if (SIZE_MINIMIZED != wParam) 
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			GetWindowRect(m_hwnd, &m_rcWindowBounds);
			GetClientRect(m_hwnd, &m_rcWindowClient);
			ResizeDevice(width, height);
			return 0;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			// 윈도우 크기(해상도) 및 포맷 변경
			case '1':
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(desc));
					desc.Width = 1024;
					desc.Height = 768;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					// 윈도우 크기 및 해상도( 화면모드 ) 변경
					m_pSwapChain->ResizeTarget(&desc);
				}
			}
			break;
			// 전체화면 모드로 전환 및 해상도 변경

			case '2':
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(desc));
					desc.Width = 800;
					desc.Height = 600;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					// 윈도우 크기 및 해상도( 화면모드 ) 변경
					m_pSwapChain->ResizeTarget(&desc);
				}
			}break;
			case '3':
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(desc));
					desc.Width = 800;
					desc.Height = 600;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

					// 윈도우 크기(해상도) 변경
					m_pSwapChain->ResizeTarget(&desc);
					m_pSwapChain->SetFullscreenState(TRUE, NULL);
				}
			}
			break;
			case '4':
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(desc));
					desc.Width = 800;
					desc.Height = 600;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

					// 윈도우 크기(해상도) 변경
					m_pSwapChain->ResizeTarget(&desc);
					m_pSwapChain->SetFullscreenState(false, NULL);
				}
			}break;

			case VK_ESCAPE:
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
			}
			break;
		case VK_ESCAPE:
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		}


		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}



bool AWindow::SetWindow(HINSTANCE histnace, LPWSTR pTitleName, int iWidth, int iHeight)
{
	m_hinstance = histnace;
		 
	g_hInstance = histnace;
	WNDCLASSEX	wcex;						
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;	//가로로 세로로 다시 뿌려달라.
	wcex.lpfnWndProc = StaticWndProc;			//윈도우프로시저 ||  위의 콜백함수
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = LoadIcon(NULL,IDI_HAND);
	wcex.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	//내장된 컬러,반환값은 브러시
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"ACORE";	

	if (RegisterClassEx(&wcex) == false) return false;

	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,				
		L"ACORE",						
		pTitleName,
		WS_OVERLAPPEDWINDOW,			//dwExstyle	 WS_OVERLAPPEDWINDOW	 WS_POPUPWINDOW
		0, 0,
		iWidth, iHeight,
		NULL, NULL, m_hinstance, NULL);
	

	if (m_hwnd == NULL) return false;

	g_hWnd = m_hwnd;
	
	g_rtWindow = m_rtWindow;
	GetClientRect(m_hwnd, &m_rtWindow);
	CenterWindow();

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);				//PAINT가 호출이 됨

	g_iClientWidth = m_rcWindowClient.right - m_rcWindowClient.left;
	g_iClientHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

	
	return true;
}

void AWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	MoveWindow(m_hwnd, x, y, m_rtWindow.right, m_rtWindow.bottom, true);
	g_rtWindow = m_rtWindow;
}




AWindow::AWindow()
{

	g_Window = this;
	m_hinstance = 0;
}



