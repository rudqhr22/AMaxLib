#pragma once


// AUiForm 대화 상자

class AUiForm : public CFormView
{
	DECLARE_DYNAMIC(AUiForm)
public:
	static AUiForm*	CreateOne(CWnd* pParent);


public:
	AUiForm();   // 표준 생성자입니다.
	virtual ~AUiForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
