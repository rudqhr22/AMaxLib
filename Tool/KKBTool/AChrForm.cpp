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
END_MESSAGE_MAP()


// AChrForm 메시지 처리기








//void AChrForm::OnEnUpdateObjY()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CFormView::OnInitDialog() 함수를 재지정 
//	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
//	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnEnUpdateObjZ()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CFormView::OnInitDialog() 함수를 재지정 
//	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
//	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnEnChangeObjZ()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CFormView::OnInitDialog() 함수를 재지정 
//	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
//	// 이 알림 메시지를 보내지 않습니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnBnClickedRadio2()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnBnClickedRadio1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnBnClickedRadio2()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}
//
//
//void AChrForm::OnBnClickedButtonMat()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}
//
//
//void AChrForm::OnBnClickedButtonChar()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnBnClickedBone()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//void AChrForm::OnLbnSelchangeList1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//	//CString tpath = _T("폴더 경로 넣기") + _T("*.*");
//
//	////검색 클래스
//	//CFileFind finder;
//
//	////CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
//	//BOOL bWorking = finder.FindFile(tpath); //
//
//	//CString fileName;
//	//CString DirName;
//
//	//while (bWorking)
//	//{
//	//	//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
//	//	bWorking = finder.FindNextFile();
//	//	//파일 일때
//	//	if (finder.IsArchived())
//	//	{
//	//		//파일의 이름
//	//		CString _fileName = finder.GetFileName();
//
//	//		// 현재폴더 상위폴더 썸네일파일은 제외
//	//		if (_fileName == _T(".") ||
//	//			_fileName == _T("..") ||
//	//			_fileName == _T("Thumbs.db")) continue;
//
//	//		fileName = finder.GetFileTitle();
//	//		m_ListBox.AddString(fileName);
//	//		//읽어온 파일 이름을 리스트박스에 넣음
//	//	}
//	//	// 디렉터리 일때
//	//	//if (finder.IsDirectory())
//	//	//{
//	//			// 필요하면 여기서 처리
//	//	//DirName = finder.GetFileName();
//	//	//}
//	//}
//	//return TRUE;
//
//
//}

//AChrForm::GetInstance()->m_AniSliderCtrl.SetRange(m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
//	m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
//AChrForm::GetInstance()->m_AniSliderCtrl.SetPos(m_CharObj->m_iCurrentFrame);


//void AChrForm::OnLbnSetfocusList1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//}


//void AChrForm::OnLbnDblclkList1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


//void AChrForm::OnEnChangeFrameMn()
//{
//
//	CString str;                                                     //EditText 의 텍스트를 저장할 변수 선언
//	m_FRAME_MN.GetWindowTextW(str);                                 //EditTExt 변수 str 에 저장
//
//	//MessageBox(str);                                            // str을 메세지박스로출력}
//
//}

//void AChrForm::OnEnChangeFrameMx()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CFormView::OnInitDialog() 함수를 재지정 
//	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
//	// 이 알림 메시지를 보내지 않습니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


//void AChrForm::OnBnClickedPlayButton()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}
//
//
//void AChrForm::OnBnClickedPauseButton()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}
//
//
//void AChrForm::OnBnClickedStopButton()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

//
//void AChrForm::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}
