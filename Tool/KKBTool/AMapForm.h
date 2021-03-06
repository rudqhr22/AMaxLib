#pragma once


// AMapForm 대화 상자

class AMapForm : public CFormView
{
	DECLARE_DYNAMIC(AMapForm)

public:
	AMapForm();   // 표준 생성자입니다.
	virtual ~AMapForm();

	static AMapForm*	CreateOne(CWnd* pParent);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_Width;
	float m_height;
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHegiht();
	afx_msg void OnBnClickedButton_TEST();
	afx_msg void OnBnClickedButtonOpenFile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
