#include "ACore.h"

int AASd()
{
	return 0;
}

bool ACore::GameInit()
{
	auto f = std::async(std::launch::async, AASd);

	PreInit();
	Init();
	PostInit();
	return true;
}

bool ACore::GameFrame()
{
	PreFrame();
	Frame();
	PostFrame();
	return true;
}

bool ACore::GameRender()
{
	PreRender();
	Render();
	if (I_INPUT.KeyCheck(DIK_V))		DrawDebug();
	PostRender();
	return true;
}

bool ACore::GameRelease()
{
	PostRelease();
	Release();
	return true;
}

bool ACore::GameRun()
{
	I_INPUT.m_hWnd = m_hwnd; 
	if (!GameInit()) return false;
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		//if(WindowMessageRun());
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			//MsgEvent(m_Msg);
		}
		else
		{
			GameFrame();
			GameRender();
		}
	}
	if(!GameRelease()) return false;
	return true;
}

void ACore::MsgEvent(MSG msg)
{
	//INPUT_MOUSE 메시지 컨트롤
	I_INPUT.MsgEvent(msg);	
};

int		ACore::WndProc(HWND, UINT, WPARAM, LPARAM)
{
	return -1;
}


//LRESULT ACore::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	//FALSE;	//TRUE;
//	return -1;
//}
//LRESULT ACore::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT hr = MsgProc(hWnd, msg, wParam, lParam);
//
//	if (hr >= 0) return hr;
//	
//	switch (msg)
//	{
//		case WM_DESTROY:
//		{
//			//GameRelease();
//			PostQuitMessage(0);
//			return 0;
//		}
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}

