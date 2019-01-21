#pragma once
#include "AStd.h"
#include "ADevice.h"

class AWindow : public ADevice
{
public:
	HWND					m_hwnd;
	HINSTANCE				m_hinstance;
	RECT					m_rtWindow;
	MSG						m_Msg;
	RECT					m_rcWindowBounds;   // ������ ����
	RECT					m_rcWindowClient;   // Ŭ���̾�Ʈ ����	


public:
	LRESULT WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void MsgEvent(MSG msg) {};
	virtual int	 WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool	SetWindow(HINSTANCE histnace, LPWSTR pTitleName, int iWidth, int iHeight);
	void	CenterWindow();
	bool	WindowMessageRun();

public:
	AWindow();
	virtual ~AWindow() {};
};

