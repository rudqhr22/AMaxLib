#pragma once
#include "AChrForm2.h"

// AChrPane2

class AChrPane2 : public CDockablePane
{
	DECLARE_DYNAMIC(AChrPane2)
public:
	AChrForm2 * m_hWndChrForm;
public:
	AChrPane2();
	virtual ~AChrPane2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


