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
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//	afx_msg void OnBnClickedOk();

	CString m_AseFileName;


//	afx_msg void OnBnClickedFileOpen();
//	afx_msg void OnBnClickedFileCreate();
//	afx_msg void OnBnClickedFileDelete();
//	afx_msg void OnBnClickedOk();

//	CListBox m_FIleNameList;
//	CButton m_bSkin;
//
//	CButton m_bMatrix;
	//afx_msg void OnBnClickedRadio1();
	//afx_msg void OnBnClickedRadio2();
	//CButton m_iSkm;
//	afx_msg void OnBnClickedRadio1();
	//CButton m_iMatrix;
//	afx_msg void OnBnClickedRadio2();
//	afx_msg void OnBnClickedButtonMat();
	afx_msg void OnBnClickedButtonChar();
	afx_msg void OnBnClickedBone();
//	afx_msg void OnLbnSelchangeList1();
//	afx_msg void OnLbnSetfocusList1();
	CListBox m_MatrixList;
	afx_msg void OnLbnDblclkList1();
	CSliderCtrl m_SliderBar;

	CEdit m_FRAME_MN;
	CEdit m_FRAME_MX;

	int   m_iMin;
	int   m_iMax;


	afx_msg void OnEnChangeFrameMn();
	afx_msg void OnEnChangeFrameMx();
	afx_msg void OnBnClickedPlayButton();
	afx_msg void OnBnClickedPauseButton();
	afx_msg void OnBnClickedStopButton();
};

//
//AChrForm::GetInstance()