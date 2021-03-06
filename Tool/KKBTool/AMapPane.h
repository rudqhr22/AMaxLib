#pragma once
#include "AMapForm.h"

// AMapPane

class AMapPane : public CDockablePane
{
	DECLARE_DYNAMIC(AMapPane)

public:
	AMapForm* m_hWndForm;

public:
	AMapPane();
	virtual ~AMapPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


