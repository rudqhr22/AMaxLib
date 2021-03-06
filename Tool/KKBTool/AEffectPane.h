#pragma once
#include "AEffectForm.h"

// AEffectPane

class AEffectPane : public CDockablePane
{
	DECLARE_DYNAMIC(AEffectPane)

public:
	AEffectForm * m_hWndEffectForm;

public:
	AEffectPane();
	virtual ~AEffectPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


