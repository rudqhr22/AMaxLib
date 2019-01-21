#include "Sample.h"

//CreateVertexBuffer 와 인덱스버퍼 확인하기


bool Sample::Load()
{
	if (!LoadFileDlg(_T("mat"), _T("MAT Viewer")))
	{
		return false;
	}
	int iLoad = m_LoadFiles.size() - 1;

	delete testobj;
	testobj = new ABoneObj;

	if (!testobj->Load(m_pd3dDevice.Get(), m_LoadFiles[iLoad].c_str(), L"../../Data/Shader/MatrixViewer.hlsl"))
	{
		return false;
	}

	return true;
}


bool Sample::LoadFileDlg(const TCHAR* szExt,const TCHAR* szTitle)
{
	OPENFILENAME    ofn;
	TCHAR           szFile[MAX_PATH] = { 0, };
	TCHAR			szFileTitle[MAX_PATH] = { 0, };
	static TCHAR    *szFilter;

	TCHAR lpCurBuffer[256] = { 0, };
	GetCurrentDirectory(256, lpCurBuffer);

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	_tcscpy_s(szFile, _T("*."));
	_tcscat_s(szFile, szExt);
	_tcscat_s(szFile, _T("\0"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = _T("../../DATA/ASE/");
	ofn.lpstrTitle = szTitle;
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = szExt;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}
	TCHAR* load = _tcstok(szFile, _T("\n"));
	T_STR dir = szFile;
	load = &load[_tcslen(load) + 1];
	if (*load == 0)
	{
		m_LoadFiles.push_back(dir);
	}

	while (*load != 0)
	{
		T_STR dir = szFile;
		load = _tcstok(load, _T("\n"));
		dir += _T("\\");
		dir += load;
		m_LoadFiles.push_back(dir);
		load = &load[_tcslen(load) + 1];
	}
	SetCurrentDirectory(lpCurBuffer);
	return true;
}


bool		Sample::Init()
{
	//"../../Data/ASE/BOX.ASE"
	//"bbox.kkb"
	//turret0
	//
	//box.hlsl
	//testobj.Load(m_pd3dDevice.Get(), _T("../../Data/ASE/skin1.skm"), _T("../../Data/Shader/SkinViewer.hlsl"), 0);
	//testobj.Load(m_pd3dDevice.Get(), _T("../../Data/ASE/ms1.skm"), _T("../../Data/Shader/SkinViewer.hlsl"));
	
	if (!LoadFileDlg(_T("mat"), _T("MAT Viewer")))
	{
		return false;
	}
	int iLoad = m_LoadFiles.size() - 1;
	testobj = new ABoneObj;
	testobj->Load(m_pd3dDevice.Get(), m_LoadFiles[iLoad].c_str(), L"../../Data/Shader/MatrixViewer.hlsl");





	return true;
}

bool		Sample::Frame()
{
	testobj->Frame();
	if (I_INPUT.KeyCheck(DIK_X)==KEY_PUSH)
	{
		Load();
	}

	return true;
}



bool		Sample::Render()
{




	testobj->SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	testobj->Render(m_pImmediateContext.Get());




	return true;
}



bool		Sample::Release()
{
	delete testobj;
	return true;
}



Sample::Sample()
{
}

Sample::~Sample()
{
}




GAME_INT
GAME_RUN
GAME_END

