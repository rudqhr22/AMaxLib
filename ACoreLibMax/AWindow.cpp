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
	return g_Window->WindowMsgProc(hWnd, msg, wParam, lParam);	//�ڽ�Ÿ�� ���� �ְԵ�
}

int	AWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return -1;
}

LRESULT CALLBACK AWindow::WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int iReturn = WndProc(hWnd, msg, wParam, lParam);
	if (iReturn >= 0)		//�̰� ���ϸ� IME ���Ͽ��� �ٷ� �޽��� ó���� �Ѿ����
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
			// ������ ũ��(�ػ�) �� ���� ����
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
					// ������ ũ�� �� �ػ�( ȭ���� ) ����
					m_pSwapChain->ResizeTarget(&desc);
				}
			}
			break;
			// ��üȭ�� ���� ��ȯ �� �ػ� ����

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
					// ������ ũ�� �� �ػ�( ȭ���� ) ����
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

					// ������ ũ��(�ػ�) ����
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

					// ������ ũ��(�ػ�) ����
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
	wcex.style = CS_HREDRAW | CS_VREDRAW;	//���η� ���η� �ٽ� �ѷ��޶�.
	wcex.lpfnWndProc = StaticWndProc;			//���������ν��� ||  ���� �ݹ��Լ�
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = LoadIcon(NULL,IDI_HAND);
	wcex.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	//����� �÷�,��ȯ���� �귯��
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
	UpdateWindow(m_hwnd);				//PAINT�� ȣ���� ��

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



