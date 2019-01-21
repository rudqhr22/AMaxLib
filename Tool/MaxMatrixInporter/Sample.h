#pragma once
#include "ACore.h"
#include "AAseObj.h"
#include "kkbObj.h"

#include "ASkinObj.h"
#include "ABoneObj.h"

class Sample : public ACore
{
public:
	//MaxAseObj		testobj;
	ABoneObj*		testobj;


public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

	bool LoadFileDlg(const TCHAR* szExt,const TCHAR* szTitle);
	bool Load();
	vector<T_STR>		m_LoadFiles;
public:
	Sample();
	virtual ~Sample();
};

#define GAME_INT int WINAPI wWinMain(HINSTANCE hN, HINSTANCE hP, LPWSTR lpCmd, int nCmd){	Sample core; 	WCHAR filName[] = L"KKB";	LPWSTR title = filName;	if (core.SetWindow(hN, title, 1024, 768))
#define GAME_RUN 		   core.GameRun();
#define GAME_END }






