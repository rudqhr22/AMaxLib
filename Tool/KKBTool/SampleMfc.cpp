#include "stdafx.h"
#include "SampleMfc.h"


 AModelObj*				m_ModelObj;
 ABoneObj*				m_BoneObj;
 AHeroObj*				m_CharObj;


CString m_StrSkin;
CString m_StrMatrix;
CString m_StrChar;


bool SampleMfc::Init()
{
	m_pLine = new ALineShape;
	m_pLine->Set(g_pd3dDevice.Get(), L"../../data/shader/Line.hlsl", 0, true);

	m_DrawHelper.DrawInit();
	
	LoadMatrixList();

	return true;
}

bool SampleMfc::Frame()
{

	if (I_INPUT.KeyCheck(DIK_F6) == KEY_UP)
	{
		m_CharObj->m_bBoneRender = !m_CharObj->m_bBoneRender;
	}


	if (m_CharObj != nullptr)
	{
		m_CharObj->Frame();


		AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);
		//AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);

	}

	if (m_ModelObj != nullptr)
	{
		m_ModelObj->Frame();
	}



	return true;
}

bool SampleMfc::Render()
{
	if (m_CharObj != nullptr)
	{
		m_CharObj->SetMatrix(m_pMainCamera->GetWorldMatrix(), m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
		m_CharObj->Render(g_pImmediateContext.Get());
	}

	if (m_ModelObj != nullptr)
	{
		m_ModelObj->SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
		m_ModelObj->Render(m_pImmediateContext.Get());
	}


	ACore::DrawDebug();
	return true;
}

bool SampleMfc::Release() 
{
	return true;
}



SampleMfc::SampleMfc()
{
}


SampleMfc::~SampleMfc()
{
}

//void AChrForm::OnBnClickedFileOpen()
//{
//
//	//static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.ASE,*.JPG) | *.ASE;*.JPG;*.ASE;*.jpg;");
//	//CFileDialog dlg(TRUE, _T("*.ase"), _T("image"), OFN_HIDEREADONLY, szFilter);
//
//
//	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.skm) | *.SKM; *.skm;");
//	CFileDialog fileDlg(TRUE, _T("*.skm"), 0, OFN_HIDEREADONLY, szFilter);
//
//
//	if (IDOK == fileDlg.DoModal())
//	{
//		m_AseFileName = fileDlg.GetPathName();
//	}
//
//	m_ModelObj = new AModelObj;
//	m_ModelObj->Load(g_pd3dDevice.Get(), m_AseFileName, _T("../Data/Shader/SkinViewer.hlsl"), 0);
//
//
//}



void SampleMfc::LoadMatrixList()
{
	filesystem::path fileDir = L"../data/MODEL/Guard_02/";
	if (filesystem::is_directory(fileDir))
	{
		for (const auto& iter : filesystem::directory_iterator(fileDir))
		{
			// 파일이면 추가
			if (filesystem::is_regular_file(iter.status()))
			{
				iter.path().filename().c_str();      // 파일 경로
				iter.path().filename();            // 파일 이름

				AChrForm::GetInstance()->m_MatrixList.AddString(iter.path().filename().c_str());
			}
			//// 디렉터리일시
			//else if (filesystem::is_directory(iter.status()))
			//{
			//	// 파일 추가
			//	for (const auto& inIter : filesystem::directory_iterator(iter.path()))
			//	{
			//		if (filesystem::is_regular_file(inIter.status()))
			//		{
			//			// 폴더 내부에 폴더의 파일
			//		}
			//	}
			//}
		}
	}
}


void AChrForm::OnLbnDblclkList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int indexSel = AChrForm::GetInstance()->m_MatrixList.GetCurSel();

	CString str;
	AChrForm::GetInstance()->m_MatrixList.GetText(indexSel,str);

	m_BoneObj = new ABoneObj;

	CString temp = 0;
	temp = "../data/MODEL/Guard_02/";

	temp += str;

	int index = I_OBJMGR.LoadObj(g_pd3dDevice.Get(), temp, _T("../../Data/Shader/MatrixViewer.hlsl"));
	m_BoneObj = (ABoneObj*)I_OBJMGR.GetPtr(index);

	m_CharObj->m_pBoneObject = m_BoneObj;
	m_CharObj->SetActionFrame(
		m_CharObj->m_pBoneObject->m_Scene.iFirstFrame, 
		m_CharObj->m_pBoneObject->m_Scene.iLastFrame);

	AChrForm::GetInstance()->m_SliderBar.SetRange(
		m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
		m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);
	m_iMin = m_CharObj->m_pBoneObject->m_Scene.iFirstFrame;
	m_iMax = m_CharObj->m_pBoneObject->m_Scene.iLastFrame;
}


void AChrForm::OnBnClickedButtonChar()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.cit) | *.CIT; *.cit;");
	CFileDialog fileDlg(TRUE, _T("*.cit"), 0, OFN_HIDEREADONLY, szFilter);


	if (IDOK == fileDlg.DoModal())
	{
		m_StrChar = fileDlg.GetPathName();
	}

	I_CHARMGR.Init();
	I_CHARMGR.Load(g_pd3dDevice.Get(), g_pImmediateContext.Get(), m_StrChar);
	
	m_CharObj = new AHeroObj;

	if (m_ModelObj != nullptr)
	{
		SAFE_DEL(m_ModelObj);
	}

	m_ModelObj = new AModelObj;


	m_CharObj->m_pChar = I_CHARMGR.GetPtr(0);
	m_ModelObj->Load(g_pd3dDevice.Get(), m_CharObj->m_pChar->m_pModelList[0]->m_pModel->m_szName.c_str(), _T("../../Data/Shader/SkinViewer.hlsl"), 0);
	
	m_CharObj->m_pBoneObject = m_CharObj->m_pChar->m_pBoneObject;
	m_CharObj->SetActionFrame(m_CharObj->m_pBoneObject->m_Scene.iFirstFrame, m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	
	AChrForm::GetInstance()->m_SliderBar.SetRange(
		m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
		m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);
	m_iMin = m_CharObj->m_pBoneObject->m_Scene.iFirstFrame;
	m_iMax = m_CharObj->m_pBoneObject->m_Scene.iLastFrame;


	AChrForm2::GetInstance()->m_TreeList;


	HTREEITEM top=0;
	HTREEITEM top2=0;
	T_STR prevParent;
	T_STR prevNode;

	TCHAR szItem[256];

	TVITEM tvItem;
	tvItem.cchTextMax = 256;
	tvItem.pszText = szItem;
	tvItem.mask = TVIF_TEXT | TVIF_HANDLE;
	for (int i = 0; i < m_CharObj->m_pBoneObject->m_pMesh.size(); i++) 
	{
		auto strNode  = m_CharObj->m_pBoneObject->m_pMesh[i]->m_strNodeName;
		auto strParent = m_CharObj->m_pBoneObject->m_pMesh[i]->m_strParentName;
		
		if (strParent == L""|| strParent== L"장면 루트")
		{
			top = AChrForm2::GetInstance()->m_TreeList.InsertItem(strNode.c_str(), TVI_ROOT, TVI_LAST);
		}
		else
		{
			//AChrForm2::GetInstance()->RecursiveFunction(top);
			if (prevParent== strParent)
			{
				AChrForm2::GetInstance()->m_TreeList.InsertItem(strNode.c_str(), TVI_ROOT, TVI_LAST);
			}
			else
			{
				top = AChrForm2::GetInstance()->m_TreeList.InsertItem(strNode.c_str(), TVI_ROOT, TVI_LAST);
			}
			
		}



		if (prevParent != strParent) {
			prevParent = m_CharObj->m_pBoneObject->m_pMesh[i]->m_strParentName;
		}
		else
		{
			prevParent;
		}
	}
}


void AChrForm2::RecursiveFunction(HTREEITEM hItem)
{
	if (hItem == NULL) return;

	while (hItem) 
	{
			// 수행할 기능 추가

			// 자식 노드 존재 확인

			if (m_TreeList.ItemHasChildren(hItem)) 
			{

				// 재귀함수 다시 호출
				RecursiveFunction(m_TreeList.GetChildItem(hItem));
			}

		// 자식 노드가 없으면 형제 노드 사용
		hItem = m_TreeList.GetNextSiblingItem(hItem);
	}
}



void AChrForm::OnBnClickedBone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.mat) | *.MAT; *.mat;");
	CFileDialog fileDlg(TRUE, _T("*.mat"), 0, OFN_HIDEREADONLY, szFilter);


	if (IDOK == fileDlg.DoModal())
	{
		m_StrChar = fileDlg.GetPathName();
	}

	m_BoneObj = new ABoneObj;

	
	int index = I_OBJMGR.LoadObj(g_pd3dDevice.Get(), m_StrChar, _T("../../Data/Shader/MatrixViewer.hlsl"));
	m_BoneObj = (ABoneObj*)I_OBJMGR.GetPtr(index);

	m_CharObj->m_pBoneObject = m_BoneObj;
	m_CharObj->SetActionFrame(m_CharObj->m_pBoneObject->m_Scene.iFirstFrame, m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	AChrForm::GetInstance()->m_SliderBar.SetRange(m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
	m_CharObj->m_pBoneObject->m_Scene.iLastFrame);

	AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);

}




//void AChrForm::OnLbnSetfocusList1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	filesystem::directory_iterator end;
//	for (filesystem::directory_iterator iter(filesystem::current_path()); iter != end; iter++)
//	{
//
//
//
//
//	}
//
//
//
//}





//void AChrForm::OnBnClickedCreateButton()
//{
//		obj = new AAseObj;
//		obj->Load(g_pd3dDevice.Get(), m_AseFileName, _T("../Data/Shader/box.hlsl"), 0);
//		testobj.push_back(obj);
//	
//		UpdateData(true);
//
//		MessageBox(L"파일 생성합니다!");
//}


//void AChrForm::OnBnClickedButton2()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.ASE,*.JPG) | *.ASE;*.JPG;*.ASE;*.jpg;");
//	CFileDialog dlg(TRUE, _T("*.ase"), _T("image"), OFN_HIDEREADONLY, szFilter);
//
//	if (IDOK == dlg.DoModal())
//	{
//		m_AseFileName = dlg.GetPathName();
//		MessageBox(L"파일 선택완료!");
//	}
//}
//
//


//void AChrForm::OnBnClickedOk()
//{
//	HTREEITEM hItemCur;
//	CString csName;
//
//	//hItemCur = m_ObjTreeCtrl.GetSelectedItem();
//	//m_ObjTreeCtrl.SelectItem(hItemCur);
//
//	//csName = m_ObjTreeCtrl.GetItemText(hItemCur);
//
//	//for (int it = 0; it != obj->m_MeshList.size(); it++)
//	//{
//	//	AMesh* pMesh = obj->m_MeshList[it];
//	//	pMesh->m_bSelect = false;
//	//}
//
//	//for (int it = 0; it != obj->m_MeshList.size(); it++)
//	//{
//	//	AMesh* pMesh = obj->m_MeshList[it];
//
//	//	if (pMesh->m_strNodeName.c_str() == csName)
//	//	{
//	//		pMesh->m_bSelect = true;
//	//		break;
//	//	}
//	//}
//
//	UpdateData(true);
//}


void AChrForm::OnEnChangeFrameMn()
{
	CString str;                                                  

	m_FRAME_MN.GetWindowText(str);
	
	m_iMin = _ttoi(str);

	AChrForm::GetInstance()->m_SliderBar.SetRange(m_iMin,
		m_iMax);

	m_CharObj->SetActionFrame(m_iMin, m_iMax);
}

void AChrForm::OnEnChangeFrameMx()
{
	CString str;

	m_FRAME_MX.GetWindowText(str);

	m_iMax = _ttoi(str);

	AChrForm::GetInstance()->m_SliderBar.SetRange(m_iMin,
		m_iMax);

	m_CharObj->SetActionFrame(m_iMin, m_iMax);
}


//void AChrForm::OnBnClickedFileCreate()
//{
//	m_ModelObj = new AModelObj;
//	m_ModelObj->Load(g_pd3dDevice.Get(), m_AseFileName, _T("../Data/Shader/SkinViewer.hlsl"), 0);
//
//	//m_ObjList.push_back(m_ModelObj);
//	/*if (m_FIleNameList != nullptr)
//	{
//	}*/
//	m_FIleNameList.DeleteString(0);
//	m_FIleNameList.AddString(m_AseFileName);
//
//	
//
//	HTREEITEM  hRoot, hChild;
//	hRoot = 0;
//	T_STR temp;
//	
//	temp = m_ModelObj->m_pModelList[0]->m_pModel->m_szName;
//
//
//	if (hRoot != NULL)
//	{
//		AChrForm2::GetInstance()->m_TreeList.DeleteAllItems();
//	}
//
//	hRoot =	AChrForm2::GetInstance()->m_TreeList.InsertItem(temp.c_str(), 0, 1);
//
//	for (int i = 1; i < m_ModelObj->m_pModelList.size(); i++)
//	{
//		temp = m_ModelObj->m_pModelList[i]->m_pModel->m_szName;// ->m_strNodeName;
//		AChrForm2::GetInstance()->m_TreeList.InsertItem(temp.c_str(), 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST);
//	}
//
//	UpdateData(true);
//	MessageBox(m_AseFileName,L"파일 생성합니다!");
//}

//void AChrForm::OnBnClickedFileDelete()
//{
//	int itemp =0;
//	itemp = m_FIleNameList.GetCurSel();
//
//	if (m_ModelObj != nullptr) 
//	{
//		delete m_ModelObj;
//		m_FIleNameList.DeleteString(itemp);
//
//	}
//	else 
//	{
//		MessageBox(L"삭제할 파일이 없습니다");
//	}
//}

//void AChrForm::OnBnClickedRadio1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//if (m_ModelObj != nullptr)
//	//{
//	//	m_ModelObj->m_bVisible = true;
//
//	//}
//	//else
//	//{
//
//	//}
//}

//void AChrForm::OnBnClickedRadio2()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//
//	//if (m_ModelObj != nullptr)
//	//{
//	//	m_ModelObj->m_bVisible = false;
//
//	//}
//
//}
