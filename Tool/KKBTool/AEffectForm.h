#pragma once


// AEffectForm 대화 상자

class AEffectForm : public CFormView
{
	DECLARE_DYNAMIC(AEffectForm)

public:
	static AEffectForm*	CreateOne(CWnd* pParent);

public:
	AEffectForm();   // 표준 생성자입니다.
	virtual ~AEffectForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
