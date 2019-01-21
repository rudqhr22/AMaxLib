#pragma once
//#include "Maxstd.h"
#include "MaxWriter.h"
//#include <Windows.h>


extern HINSTANCE hInstance;

#define KKB _declspec(dllexport)
#define MaxUTIL_CLASS_ID  Class_ID(0x46b909e8, 0x4e97925)




class maxUtil : public UtilityObj 
{
public:
	//MaxWriter	m_Writer;
	HWND			hPanel;
	IUtil			*iu;
	Interface		*ip;
public:
	maxUtil();
	virtual ~maxUtil();

public:
	virtual void DeleteThis() { }

	virtual void BeginEditParams(Interface *ip, IUtil *iu);
	virtual void EndEditParams(Interface *ip, IUtil *iu);

	virtual void Init(HWND hWnd);
	virtual void Destroy(HWND hWnd);

	void SelectionSetChanged(Interface *ip, IUtil *iu);

	// Singleton access
	static maxUtil* GetInstance() {
		static maxUtil thetbsUtilExp;
		return &thetbsUtilExp;
	}

private:
	//대화상자 프로시저
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};


class maxClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) { return maxUtil::GetInstance(); }
	virtual const TCHAR *	ClassName() { return _T("kkbUtilExporter"); }
	virtual SClass_ID SuperClassID() { return UTILITY_CLASS_ID; }
	virtual Class_ID ClassID() { return MaxUTIL_CLASS_ID; }
	virtual const TCHAR* Category() { return _T("maxUtil");; }

	virtual const TCHAR* InternalName() { return _T("maxUtil"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() { return hInstance; }					// returns owning module handle
};

