// AChrForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "AChrForm.h"
#include "afxdialogex.h"


// AChrForm 대화 상자

IMPLEMENT_DYNAMIC(AChrForm, CFormView)

AChrForm* AChrForm::instance = nullptr;

AChrForm::AChrForm() : CFormView(IDD_DIALOG4), m_AseFileName(_T(""))
{
	//T_STR temp = L"dfgdfg";
	//m_ObjTreeCtrl.InsertItem(temp.c_str(), 0/* nImage */, 1/* nSelectedImage */, TVI_ROOT, TVI_LAST);

	instance = this;

}
AChrForm::~AChrForm()
{

}




AChrForm*	AChrForm::CreateOne(CWnd* pParent)
{
	AChrForm* pForm = new AChrForm;

	pForm->Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent,			//중요
		0, NULL);


	return pForm;
}




void AChrForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	//DDX_Control(pDX, ASE_FILE_LIST, m_FIleNameList);
	//	DDX_Control(pDX, ID_OBJ_TREE, m_ObjTreeCtrl);
	//	DDX_Control(pDX, IDC_RADIO1, m_bSkin);
	//DDX_Control(pDX, IDC_RADIO1, m_bSkin);
	//DDX_Control(pDX, IDC_RADIO2, m_bMatrix);
	//DDX_Control(pDX, IDC_RADIO1, m_iSkm);
	//DDX_Control(pDX, IDC_RADIO2, m_iMatrix);
	DDX_Control(pDX, IDC_LIST1, m_MatrixList);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderBar);
	DDX_Control(pDX, IDC_FRAME_MN, m_FRAME_MN);
	DDX_Control(pDX, IDC_FRAME_MX, m_FRAME_MX);
	DDX_Control(pDX, IDC_SPEED_SPIN, m_FrameSpeed);
}


BEGIN_MESSAGE_MAP(AChrForm, CFormView)
//ON_BN_CLICKED(ASE_FILE_OPEN, &AChrForm::OnBnClickedFileOpen)
//ON_BN_CLICKED(ASE_FILE_CREATE, &AChrForm::OnBnClickedFileCreate)
//ON_BN_CLICKED(ASE_FILE_DELETE, &AChrForm::OnBnClickedFileDelete)
//ON_BN_CLICKED(IDOK, &AChrForm::OnBnClickedOk)

//ON_BN_CLICKED(IDC_RADIO2, &AChrForm::OnBnClickedRadio2)
//ON_BN_CLICKED(IDC_RADIO1, &AChrForm::OnBnClickedRadio1)
//ON_BN_CLICKED(IDC_RADIO2, &AChrForm::OnBnClickedRadio2)
//ON_BN_CLICKED(IDC_BUTTON_MAT, &AChrForm::OnBnClickedButtonMat)
ON_BN_CLICKED(IDC_BUTTON_CHAR, &AChrForm::OnBnClickedButtonChar)
ON_BN_CLICKED(IDC_BONE, &AChrForm::OnBnClickedBone)
//ON_LBN_SELCHANGE(IDC_LIST1, &AChrForm::OnLbnSelchangeList1)
//ON_LBN_SETFOCUS(IDC_LIST1, &AChrForm::OnLbnSetfocusList1)
ON_LBN_DBLCLK(IDC_LIST1, &AChrForm::OnLbnDblclkList1)
ON_EN_CHANGE(IDC_FRAME_MN, &AChrForm::OnEnChangeFrameMn)
ON_EN_CHANGE(IDC_FRAME_MX, &AChrForm::OnEnChangeFrameMx)
ON_BN_CLICKED(IDC_PLAY_BUTTON, &AChrForm::OnBnClickedPlayButton)
//ON_BN_CLICKED(IDC_PAUSE_BUTTON, &AChrForm::OnBnClickedPauseButton)
ON_BN_CLICKED(IDC_STOP_BUTTON, &AChrForm::OnBnClickedStopButton)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &AChrForm::OnNMReleasedcaptureSlider1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPEED_SPIN, &AChrForm::OnDeltaposSpeedSpin)
END_MESSAGE_MAP()


// AChrForm 메시지 처리기


void AChrForm::OnBnClickedPlayButton()
{
	SampleMfc::GetInstance()->m_CharObj->m_fSpeed = 1.0f;
	m_bStop = false;
}

void AChrForm::OnBnClickedStopButton()
{
	SampleMfc::GetInstance()->m_CharObj->m_fSpeed = 0.0f;
	m_bStop = true;
}


void AChrForm::OnEnChangeFrameMn()
{
	CString str;

	m_FRAME_MN.GetWindowText(str);

	m_iMin = _ttoi(str);

	AChrForm::GetInstance()->m_SliderBar.SetRange(m_iMin,m_iMax);

	SampleMfc::GetInstance()->m_CharObj->SetActionFrame(m_iMin, m_iMax);
}

void AChrForm::OnEnChangeFrameMx()
{
	CString str;

	m_FRAME_MX.GetWindowText(str);

	m_iMax = _ttoi(str);

	AChrForm::GetInstance()->m_SliderBar.SetRange(m_iMin, m_iMax);

	SampleMfc::GetInstance()->m_CharObj->SetActionFrame(m_iMin, m_iMax);
}


void AChrForm::OnDeltaposSpeedSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMUpDown->iDelta < 0)
	{
		SampleMfc::GetInstance()->m_CharObj->m_fSpeed+=0.1f;
		if (SampleMfc::GetInstance()->m_CharObj->m_fSpeed >=3 ) {
			SampleMfc::GetInstance()->m_CharObj->m_fSpeed = 3;
		}
	}
	else
	{
		SampleMfc::GetInstance()->m_CharObj->m_fSpeed-=0.1f;
		if (SampleMfc::GetInstance()->m_CharObj->m_fSpeed <= 0) {
			SampleMfc::GetInstance()->m_CharObj->m_fSpeed = 0;
		}
	}
}
