// AMapPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "AMapPane.h"


// AMapPane

IMPLEMENT_DYNAMIC(AMapPane, CDockablePane)

AMapPane::AMapPane()
{

}

AMapPane::~AMapPane()
{
}


BEGIN_MESSAGE_MAP(AMapPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// AMapPane 메시지 처리기




int AMapPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	this->m_hWndForm = AMapForm::CreateOne(this);
	return 0;
}


void AMapPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_hWndForm)
	{
		m_hWndForm->SetWindowPos(
			NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int AMapPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();

	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{

		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;	//무시하겠따
}
