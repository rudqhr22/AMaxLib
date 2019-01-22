#pragma once
#include "ACore.h"
#include "AAseObj.h"
#include "kkbObj.h"

#include "AModelObj.h"


#include "AFPSCamera.h"
#include "AModelObj.h"
//#include "AModelobj"
#include "ACharMgr.h"
#include "ASkinObj.h"
#include "AHeroObj.h"

//#include <cstdio>
#include <future>





class Sample : public ACore
{
public:
	AHeroObj		testobj;



public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

	bool LoadFileDlg(const TCHAR* szExt,const TCHAR* szTitle);

	vector<T_STR>		m_LoadFiles;



public:
	Sample();
	virtual ~Sample();
};




#define GAME_INT int WINAPI wWinMain(HINSTANCE hN, HINSTANCE hP, LPWSTR lpCmd, int nCmd){	Sample core; 	WCHAR filName[] = L"KKB";	LPWSTR title = filName;	if (core.SetWindow(hN, title, 1024, 768))
#define GAME_RUN 		   core.GameRun();
#define GAME_END }






