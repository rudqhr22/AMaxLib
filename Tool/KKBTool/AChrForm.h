#pragma once
#include "AChrForm2.h"

// AChrForm 대화 상자


class AChrForm : public CFormView
{
	DECLARE_DYNAMIC(AChrForm)
	static AChrForm* instance;

public:
	static AChrForm* GetInstance()
	{
		return instance;
	}
private:
	AChrForm();   
	// 표준 생성자입니다.
	//AChrForm(const AChrForm& other);
public:
	static AChrForm* CreateOne(CWnd* pParent);
	virtual ~AChrForm();
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_AseFileName;

	CListBox m_MatrixList;
	CSliderCtrl m_SliderBar;

	CEdit m_FRAME_MN;
	CEdit m_FRAME_MX;
	CSpinButtonCtrl m_FrameSpeed;

	int   m_iMin;
	int   m_iMax;
	bool  m_bStop;
public:
	afx_msg void OnBnClickedButtonChar();
	afx_msg void OnBnClickedBone();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnEnChangeFrameMn();
	afx_msg void OnEnChangeFrameMx();
	afx_msg void OnBnClickedPlayButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDeltaposSpeedSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
