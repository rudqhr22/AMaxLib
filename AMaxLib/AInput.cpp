#pragma once
#include "AInput.h"


//if (I_INPUT.KeyCheck(DIK_A))
//{
//	GetCursorPos(&I_INPUT.m_MousePos);
//	ScreenToClient(g_hWnd, &I_INPUT.m_MousePos);
//}
//if (I_INPUT.m_DIMouseState.rgbButtons[0])
//{
//	GetCursorPos(&I_INPUT.m_MousePos);
//	ScreenToClient(g_hWnd, &I_INPUT.m_MousePos);
//}




INPUT_MAP g_InputData;


bool   AInput::InitDirectInput()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = DirectInput8Create(g_hInstance,	DIRECTINPUT_VERSION,IID_IDirectInput8,
		(void**)&m_pDI, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard,	&m_pKeyDevice, NULL)))
	{
		return false;
	}
	//GetActiveView SetCooperativeLevel

	// 장치별 반환 데이터 설정
	m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	// 장치별 독점 및 비독점 설정(협조레벨)			//m_hWnd
	if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(m_hWnd,  DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{			
		return false;
	}

	while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);


	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse,	&m_pMouseDevice, NULL)))
	{
		return false;
	}
	m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
														//m_hWnd
	if (FAILED(hr = m_pMouseDevice->SetCooperativeLevel(m_hWnd,	DISCL_NONEXCLUSIVE |DISCL_FOREGROUND)))
	{
		return true;
	}
	while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
	return S_OK;
}


BYTE AInput::KeyCheck(BYTE dwKey)
{
	// GetKeyState()		// 동기적
	// GetAsyncKeyState()	// 비동기적
	BYTE sKey;
#ifdef WIN_KEY_USE
	sKey = GetAsyncKeyState(dwKey);
#else
	sKey = m_KeyState[dwKey];
#endif
	// 0x8000 =  10000000 00000000
	if (sKey & 0x80)
	{
		if (m_KeyStateOld[dwKey] == KEY_FREE)
		{
			m_KeyStateOld[dwKey] = KEY_PUSH;
		}
		else
		{
			m_KeyStateOld[dwKey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_KeyStateOld[dwKey] == KEY_PUSH || m_KeyStateOld[dwKey] == KEY_HOLD)
		{
			m_KeyStateOld[dwKey] = KEY_UP;
		}
		else
		{
			m_KeyStateOld[dwKey] = KEY_FREE;
		}
	}
	return m_KeyStateOld[dwKey];
}
// 클래스 멤버 변수 초기화
bool		AInput::Init()
{
	InitDirectInput();
	ZeroMemory(m_KeyState, sizeof(BYTE) * 256);
	ZeroMemory(m_MouseState, sizeof(BYTE) * 3);
	ZeroMemory(m_BeforeMouseState, sizeof(BYTE) * 3);

	return true;
};
// 매 프레임에서 계산을 담당한다.
bool		AInput::Frame()
{
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);	//m_hWnd 안님?
#ifndef WIN_KEY_USE
	HRESULT hr;
	if (m_pMouseDevice == NULL || m_pKeyDevice == NULL) return false;

	if (FAILED(hr = m_pKeyDevice->GetDeviceState(KEYSTATECOUNT, m_KeyState)))
	{
		while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);
		//return true;
	}

	if (FAILED(hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
	{
		while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
		//return true;
	}
	for (int iButton = 0; iButton < 3; iButton++)
	{
		m_BeforeMouseState[iButton] = m_DIMouseState.rgbButtons[iButton];
	}

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_BeforeMouseState[iButton] & 0x80)
		{
			if (m_MouseState[iButton] == KEY_FREE)
				m_MouseState[iButton] = KEY_PUSH;
			else
				m_MouseState[iButton] = KEY_HOLD;
		}
		else
		{
			if (m_MouseState[iButton] == KEY_PUSH ||
				m_MouseState[iButton] == KEY_HOLD)
				m_MouseState[iButton] = KEY_UP;
			else
				m_MouseState[iButton] = KEY_FREE;
		}
	}
#else
	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_MouseState[iButton] == KEY_PUSH &&
			m_MouseState[iButton] == KEY_HOLD &&
			m_MouseState[iButton] == KEY_UP)
		{
			m_MouseState[iButton] = KEY_FREE;
		}
	}
#endif

	ZeroMemory(&g_InputData, sizeof(INPUT_MAP));

	if (m_MouseState[0] == KEY_PUSH) 	g_InputData.bLeftClick = true;
	if (m_MouseState[1] == KEY_PUSH) 	g_InputData.bRightClick = true;
	if (m_MouseState[2] == KEY_PUSH) 	g_InputData.bMiddleClick = true;

	if (m_MouseState[0] >= KEY_PUSH) 	g_InputData.bLeftHold = true;
	if (m_MouseState[1] >= KEY_PUSH) 	g_InputData.bRightHold = true;
	if (m_MouseState[2] >= KEY_PUSH) 	g_InputData.bMiddleHold = true;



	g_InputData.bAKey = KeyCheck(DIK_A) != 0;
	g_InputData.bDKey = KeyCheck(DIK_D) != 0;
	g_InputData.bWKey = KeyCheck(DIK_W) != 0;
	g_InputData.bSKey = KeyCheck(DIK_S) != 0;
	g_InputData.bQKey = KeyCheck(DIK_Q) != 0;
	g_InputData.bEKey = KeyCheck(DIK_E) != 0;
	g_InputData.bZKey = KeyCheck(DIK_Z) != 0;
	g_InputData.bCKey = KeyCheck(DIK_C) != 0;
	
	g_InputData.bDebugRender = KeyCheck(DIK_V);

	g_InputData.bLeftKey = KeyCheck(DIK_LEFT) != 0;
	g_InputData.bRightKey = KeyCheck(DIK_RIGHT) != 0;
	g_InputData.bUpKey = KeyCheck(DIK_UP) != 0;
	g_InputData.bDownKey = KeyCheck(DIK_DOWN) != 0;
	g_InputData.bExit = KeyCheck(DIK_ESCAPE) != 0;
	g_InputData.bSpace = KeyCheck(DIK_SPACE) != 0;
	g_InputData.bExit = KeyCheck(DIK_ESCAPE) != 0;


	g_InputData.iMouseValue[0] = m_DIMouseState.lX;
	g_InputData.iMouseValue[1] = m_DIMouseState.lY;
	g_InputData.iMouseValue[2] = m_DIMouseState.lZ;

	if (KeyCheck(DIK_F12 == KEY_PUSH))	g_InputData.bChangeCameraType = true;
	if (KeyCheck(DIK_F5 == KEY_PUSH))	g_InputData.bChangeFillMode = true;
	if (KeyCheck(DIK_F6 == KEY_PUSH))	g_InputData.bChangePrimitive = true;
	if (KeyCheck(DIK_F7 == KEY_PUSH))	g_InputData.bChangeCullMode = true;
	if (KeyCheck(DIK_F8 == KEY_PUSH))	g_InputData.bChangeCameraType = true;

	return true;
};
// 매 프레임에서 드로우를 담당한다.
bool		AInput::Render()
{
	ZeroMemory(m_csBuffer, sizeof(LPCWSTR)* MAX_PATH);
	m_strList.clear();

	std::wstring strData0;
	if (m_MouseState[0] == TRUE)
	{
		strData0 = L"m_MouseState[0] ";
		m_strList.push_back(strData0);
	}

	std::wstring strData1;
	if (m_MouseState[1] == TRUE)
	{
		strData1 = L"m_MouseState[1] ";
		m_strList.push_back(strData1);
	}

	std::wstring strData2;
	if (m_MouseState[2] == TRUE)
	{
		strData2 = L"m_MouseState[2] ";
		m_strList.push_back(strData2);
	}
	//#ifndef WIN_KEY_USE
	//	memcpy(m_KeyStateOld, m_KeyState, sizeof(BYTE)*KEYSTATECOUNT);
	//#endif
	return true;
};
// 클래스 멤버 변수 소멸을 담당한다.
bool		AInput::Release()
{
	if (m_pKeyDevice)m_pKeyDevice->Unacquire();
	if (m_pMouseDevice)m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pKeyDevice);
	SAFE_RELEASE(m_pMouseDevice);
	SAFE_RELEASE(m_pDI);
	return true;
};
void AInput::MsgEvent(MSG msg)
{
#ifndef WIN_KEY_USE
	return;
#endif
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
	{
		if (m_MouseState[0] == KEY_PUSH)
			m_MouseState[0] = KEY_HOLD;
		else
			m_MouseState[0] = KEY_PUSH;
	}break;
	case WM_LBUTTONUP:
	{
		m_MouseState[0] = KEY_UP;
	}break;
	case WM_RBUTTONDOWN:
	{
		if (m_MouseState[1] == KEY_PUSH)
			m_MouseState[1] = KEY_HOLD;
		else
			m_MouseState[1] = KEY_PUSH;
	}break;
	case WM_RBUTTONUP:
	{
		m_MouseState[1] = KEY_UP;
	}break;
	case WM_MBUTTONDOWN:
	{
		if (m_MouseState[2] == KEY_PUSH)
			m_MouseState[2] = KEY_HOLD;
		else
			m_MouseState[2] = KEY_PUSH;
	}break;
	case WM_MBUTTONUP:
	{
		m_MouseState[2] = KEY_UP;
	}break;
	}
};
void AInput::DeviceAcquire()
{
	if (m_pKeyDevice)
		m_pKeyDevice->Acquire();
	if (m_pMouseDevice)
		m_pMouseDevice->Acquire();
}

void AInput::DeviceUnacquire()
{
	if (m_pKeyDevice)
		m_pKeyDevice->Unacquire();
	if (m_pMouseDevice)
		m_pMouseDevice->Unacquire();
}
AInput::AInput()
{
	m_pMouseDevice = NULL;
	m_pKeyDevice = NULL;
}
AInput::~AInput()
{
}
