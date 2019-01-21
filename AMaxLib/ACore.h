#pragma once
#include "AInput.h"
#include "ATimer.h"
#include "AWindow.h"
#include "ADevice.h"
#include "ADxState.h"
#include "ADxWrite.h"
#include "ACamera.h"
#include "ADxShape.h"
#include <future>



class ACore : public  AWindow
{
public:
	ADirectionLineShape				m_AxisLine;
	ADxWrite						m_Font;
	
	ACamera							m_Camera;
	ACamera*						m_pMainCamera;

	void SwapCamera(ACamera* pCamera);
	void DefaultCamera();// {};
public:
	virtual bool	Init() { return true; };
	virtual bool	Frame() { return true; };
	virtual bool	Render() { return true; };
	virtual bool	Release() { return true; };


	//virtual LRESULT WndProc(HWND hWnd,		UINT msg,		WPARAM wParam,		LPARAM lParam);
	void   MsgEvent(MSG msg);

public:
	bool GameRun();
	///////////////////
	bool GameInit();
	bool GameFrame();
	bool GameRender();
	bool GameRelease();
	////////////////////
	bool PreInit();
	bool PostInit();
	////////////////////
	bool PreFrame();
	bool PostFrame();
	////////////////////
	bool PreRender();
	bool PostRender();
	////////////////////
	bool PostRelease();
	bool	DrawDebug();


	virtual		HRESULT		CreateDxResource();
	virtual		HRESULT		DeleteDxResource();
	virtual int		WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	ACore();
	virtual ~ACore();
};

