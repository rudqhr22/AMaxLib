#pragma once
#include "AStd.h"
#include "ADxStd.h"

//각종 입력장치 관리제어 담당
//상속을 안하고 포함으로 해도됨

#define KEYSTATECOUNT 256

enum KeyState
{
	KEY_FREE=0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD
};


class AInput: public ASingleton<AInput>
{
private:
	friend class ASingleton<AInput>;		//싱글톤을 접근 할라면

public:
	HWND   m_hWnd;
	//  인풋 객체
	LPDIRECTINPUT8		m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // 키보드
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// 마우스
	BYTE				  m_KeyState[KEYSTATECOUNT];
	BYTE				  m_KeyStateOld[KEYSTATECOUNT];
	DIMOUSESTATE		  m_DIMouseState;
	vector<wstring>  m_strList;
	LPCWSTR		m_csBuffer[MAX_PATH];
	BYTE		m_MouseState[3];
	BYTE		m_BeforeMouseState[3];
	POINT		m_MousePos;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	void		MsgEvent(MSG msg);
	BYTE		KeyCheck(BYTE dwKey);
	bool		InitDirectInput();
	void		DeviceAcquire();
	void		DeviceUnacquire();


private:		//싱글톤이라서
	AInput();
public:
	virtual ~AInput();
};

#define I_INPUT AInput::GetInstance()
//0x80(down), 0x40(once), 0x20(up)