#pragma once
#include "ACore.h"
#include "AHeroObj.h"
#include "DrawHelper.h"
#include "CollCheck.h"

enum ANIMSTATE
{
	IDLE,
	FORWARD,
	BACK
};

class Sample : public ACore
{
public:


	AHeroObj		m_CharObj;

	AHeroObj		m_zombie;

	DrawHelper		m_DrawLine;
	DrawHelper		m_DrawLine2;

	ANIMSTATE	   m_State;
	bool			bNormal = false;
	bool			m_bMove = false;
	bool			m_bIdle = false;


	bool		bForward;
	bool		bIdle;
	bool		bRight;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();


public:
	Sample();
	virtual ~Sample();
};





#define GAME_INT int WINAPI wWinMain(HINSTANCE hN, HINSTANCE hP, LPWSTR lpCmd, int nCmd){	Sample core; 	WCHAR filName[] = L"KKB";	LPWSTR title = filName;	if (core.SetWindow(hN, title, 1024, 768))
#define GAME_RUN 		   core.GameRun();
#define GAME_END }






