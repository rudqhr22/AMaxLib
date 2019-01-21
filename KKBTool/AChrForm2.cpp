// AChrForm2.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "AChrForm2.h"
#include "afxdialogex.h"


// AChrForm2 대화 상자

IMPLEMENT_DYNAMIC(AChrForm2, CFormView)


AChrForm2* AChrForm2::instance = nullptr;

AChrForm2::AChrForm2()	: CFormView(IDD_DIALOG5)
{
	instance = this;
}

AChrForm2::~AChrForm2()
{
}




AChrForm2*	AChrForm2::CreateOne(CWnd* pParent)
{
	
	AChrForm2* pForm = new AChrForm2;

	pForm->Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent,			//중요
		0, NULL);

	return pForm;
}


void AChrForm2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeList);
}


BEGIN_MESSAGE_MAP(AChrForm2, CFormView)
END_MESSAGE_MAP()


// AChrForm2 메시지 처리기
