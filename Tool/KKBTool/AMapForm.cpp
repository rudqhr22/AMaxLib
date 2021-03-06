// AMapForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "AMapForm.h"
#include "afxdialogex.h"


// AMapForm 대화 상자

IMPLEMENT_DYNAMIC(AMapForm, CFormView)


 AMapForm*	AMapForm::CreateOne(CWnd* pParent)
{
	AMapForm* pForm = new AMapForm;
	pForm->Create(NULL,NULL,
		WS_CHILD|WS_VISIBLE,
		CRect(0,0,500,500),
		pParent,			//중요
		0,NULL);

	return pForm;
}



AMapForm::AMapForm(): CFormView(IDD_DIALOG1)
, m_Width(0)
, m_height(0)
{

}

AMapForm::~AMapForm()
{
}

void AMapForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_Width);
	DDX_Text(pDX, IDC_EDIT_HEGIHT, m_height);
}


BEGIN_MESSAGE_MAP(AMapForm, CFormView)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &AMapForm::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEGIHT, &AMapForm::OnEnChangeEditHegiht)
	ON_BN_CLICKED(IDC_BUTTON1, &AMapForm::OnBnClickedButton_TEST)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &AMapForm::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDOK, &AMapForm::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &AMapForm::OnBnClickedCancel)
END_MESSAGE_MAP()


// AMapForm 메시지 처리기


void AMapForm::OnEnChangeEditWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void AMapForm::OnEnChangeEditHegiht()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void AMapForm::OnBnClickedButton_TEST()
{
	CString str;

	UpdateData(true);

	str.Format(_T("m_Width : %f \n  m_height : %f \n"), m_Width,m_height);

	MessageBox(str);

	m_Width;
	m_height;


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void AMapForm::OnBnClickedButtonOpenFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP,*.JPG) | *.BMP;*.JPG;*.bmp;*.jpg;");
	//static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();

		

		MessageBox(pathName);

	}
}


void AMapForm::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void AMapForm::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



