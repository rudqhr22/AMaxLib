// AUiForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "AUiForm.h"
#include "afxdialogex.h"


// AUiForm 대화 상자

IMPLEMENT_DYNAMIC(AUiForm, CFormView)



AUiForm*	AUiForm::CreateOne(CWnd* pParent)
{
	AUiForm* pForm = new AUiForm;
	pForm->Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent,			//중요
		0, NULL);

	return pForm;


}


AUiForm::AUiForm(): CFormView(IDD_DIALOG3)
{

}

AUiForm::~AUiForm()
{

}

void AUiForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AUiForm, CFormView)
//	ON_WM_CREATE()
//	ON_WM_SIZE()
//	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// AUiForm 메시지 처리기


//int AUiForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CFormView::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
//
//	return 0;
//}


//void AUiForm::OnSize(UINT nType, int cx, int cy)
//{
//	CFormView::OnSize(nType, cx, cy);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}


//int AUiForm::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//
//
//	return CFormView::OnMouseActivate(pDesktopWnd, nHitTest, message);
//}
