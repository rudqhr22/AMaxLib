#pragma once
#include "AUiForm.h"

// AUiPane

class AUiPane : public CDockablePane
{
	DECLARE_DYNAMIC(AUiPane)
public:
	AUiForm* m_hWndUiForm;

public:
	AUiPane();
	virtual ~AUiPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};



//8월 21일 1차
//9월 7일  2차
//10월 5일 3차
//11월 2일 4차