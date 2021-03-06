#pragma once
#include "AChrForm.h"

// AChrPane

class AChrPane : public CDockablePane
{
	DECLARE_DYNAMIC(AChrPane)
public:
	AChrForm* m_hWndChrForm;
public:
	AChrPane();
	virtual ~AChrPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


