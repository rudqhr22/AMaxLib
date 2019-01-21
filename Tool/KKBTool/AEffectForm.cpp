// AEffectForm.cpp: 구현 파일
//


//IDD_DIALOG1 MAP
//IDD_DIALOG2 EFFECT
//IDD_DIALOG3 UI


#include "stdafx.h"
#include "MapTool.h"
#include "AEffectForm.h"
#include "afxdialogex.h"


// AEffectForm 대화 상자

IMPLEMENT_DYNAMIC(AEffectForm, CFormView)

AEffectForm::AEffectForm(): CFormView(IDD_DIALOG2)
{

}

AEffectForm::~AEffectForm()
{
}

AEffectForm*	AEffectForm::CreateOne(CWnd* pParent)
{
	AEffectForm* pForm = new AEffectForm;
	pForm->Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent,			//중요
		0, NULL);
	return pForm;
}


void AEffectForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(AEffectForm, CFormView)
END_MESSAGE_MAP()


// AEffectForm 메시지 처리기
