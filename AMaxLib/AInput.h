#pragma once
#include "AStd.h"
#include "ADxStd.h"

//���� �Է���ġ �������� ���
//����� ���ϰ� �������� �ص���

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
	friend class ASingleton<AInput>;		//�̱����� ���� �Ҷ��

public:
	HWND   m_hWnd;
	//  ��ǲ ��ü
	LPDIRECTINPUT8		m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // Ű����
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// ���콺
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


private:		//�̱����̶�
	AInput();
public:
	virtual ~AInput();
};

#define I_INPUT AInput::GetInstance()
//0x80(down), 0x40(once), 0x20(up)