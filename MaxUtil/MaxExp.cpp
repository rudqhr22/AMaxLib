#include "MaxExp.h"
#include "MaxSkinExp.h"
HINSTANCE hInstance;

maxUtil::maxUtil()
	: hPanel(nullptr)
	, iu(nullptr)
{

}

maxUtil::~maxUtil()
{

}
//익스포터 열릴떄 호출
void maxUtil::BeginEditParams(Interface* ip, IUtil* iu)
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(					//이 함수가 다이얼 로그를 붙여준다.
		hInstance,
		MAKEINTRESOURCE(IDD_DIALOG1),
		DlgProc,
		_T("Parameters"),
		0);
}
//익스포터 닫일떄 호출
void maxUtil::EndEditParams(Interface* ip, IUtil*)
{
	this->iu = NULL;
	this->ip = NULL;
	this->iu = nullptr;
	ip->DeleteRollupPage(hPanel);
	hPanel = nullptr;
}
void maxUtil::Init(HWND hWnd)
{
	if (I_SKINEXP.Init() == false) return;
	if (I_WRITER.Initialize(ip) == false) return;

	if (I_WRITER.m_MatrixMap.Count() > 0)
	{
		EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_SKIN), true);
		EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_MATRIX), true);
	//	EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_OBJEXPORT), true);
	//	EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_UPDATE), false);
	}
	else
	{
		EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_SKIN), false);
		EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_MATRIX), false);
	//	EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_OBJEXPORT), false);
	//	EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_UPDATE), true);
	}
	//SendMessage(tbsUtilExp::GetInstance()->hPanel, BM_SETCHECK, BST_CHECKED, 0);
	//CheckDlgButton(theTbsUtilExporter.hPanel, IDC_BINDPOSE, TRUE );
	//EnableWindow(GetDlgItem(tbsUtilExp::GetInstance()->hPanel, IDC_BINDPOSE), false);

}

void maxUtil::Destroy(HWND hWnd)
{
	I_SKINEXP.Release();
	I_WRITER.Release();
}

void maxUtil::SelectionSetChanged(Interface *ip, IUtil *iu)
{
	if (ip->GetSelNodeCount() <= 0)
	{
		return;
	}
	if (I_WRITER.Initialize(ip) == false)
	{
		return;
	}

	I_SKINEXP.Release();

	for (int iSelectObj = 0; iSelectObj < ip->GetSelNodeCount(); iSelectObj++)
	{
		I_SKINEXP.GetNodeInfo(ip->GetSelNode(iSelectObj), ip->GetTime());
	}

	if (I_WRITER.m_MatrixMap.Count() > 0)
	{
		EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_MATRIX), true);
		if (ip->GetSelNodeCount() > 0)
		{
			EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_SKIN), true);
		}
		else
		{
			EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_SKIN), false);
		}
	}
	else
	{
		EnableWindow(GetDlgItem(maxUtil::GetInstance()->hPanel, IDC_MATRIX), false);
	}
}

INT_PTR CALLBACK maxUtil::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		maxUtil::GetInstance()->Init(hWnd);

		break;
	case WM_DESTROY:
		maxUtil::GetInstance()->Destroy(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EXPORT:
		{
			I_WRITER.Initialize(maxUtil::GetInstance()->ip);
			I_WRITER.Export();
		}break;
		case IDC_SKIN:
		{
			I_WRITER.SetBindPose(true);
			I_WRITER.Initialize(maxUtil::GetInstance()->ip);
			I_SKINEXP.m_Scene = I_WRITER.m_Scene;
			I_SKINEXP.SkinExport();
		}break;
		case IDC_MATRIX:
		{
			I_WRITER.Initialize(maxUtil::GetInstance()->ip);
			I_WRITER.SetBindPose(true);
			I_WRITER.ExpMatrix();
		}break;
		}break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		GetCOREInterface()->RollupMouseMessage(hWnd, msg, wParam, lParam);
		break;

	default:
		return 0;
	}
	return 1;
}




//도구상제에서 버튼을 추가 하고, 누르면 EXPORT 되도록 한다.




ClassDesc2* GetExportDesc()
{
	static maxClassDesc maxDesc;
	return &maxDesc;
}
