#include "stdafx.h"
#include "SampleMfc.h"

SampleMfc* SampleMfc::instance = nullptr;


 //AModelObj*				m_ModelObj;
 //ABoneObj*				m_BoneObj;
 //AHeroObj*				m_CharObj;
CString m_StrSkin;
CString m_StrMatrix;
CString m_StrChar;

void SampleMfc::AddTreeMesh(AMesh* pObject, const HTREEITEM& parent, const int& level)
{
	for (auto& iter : pObject->m_pChildMesh)
	{
		auto ppparent = AChrForm2::GetInstance()->m_TreeList.InsertItem(iter->m_strNodeName.c_str(), 0, level, parent, TVI_SORT);

		AChrForm2::GetInstance()->m_TreeList.SetItemData(ppparent, (DWORD_PTR)iter);

		if (!iter->m_pChildMesh.empty())
		{
			AddTreeMesh(iter, ppparent, level + 1);
		}
	}
}


void SampleMfc::AddTreeItem(AMesh* pObject)
{
	auto parent = AChrForm2::GetInstance()->m_TreeList.InsertItem(pObject->m_strNodeName.c_str(), 0, 0, TVI_ROOT, TVI_LAST);
	//AChrForm2::GetInstance()->m_TreeList.SetItemData(parent, (DWORD_PTR)pObject);

	AddTreeMesh(pObject, parent, 1);
}


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

	
	if (m_CharObj != nullptr && m_bPlay == true)
	{
		m_CharObj->Frame();


		AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);
		//AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);

	}

	if (m_ModelObj != nullptr)
	{
		//m_ModelObj->Frame();
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
	instance = this;
	m_bPlay = true;
}


SampleMfc::~SampleMfc()
{
}






void SampleMfc::LoadMatrixList()
{
	filesystem::path fileDir = L"../data/MODEL/Guard_02/";
	if (filesystem::is_directory(fileDir))
	{
		for (const auto& iter : filesystem::directory_iterator(fileDir))
		{
			// �����̸� �߰�
			if (filesystem::is_regular_file(iter.status()))
			{
				iter.path().filename().c_str();      // ���� ���
				iter.path().filename();            // ���� �̸�

				AChrForm::GetInstance()->m_MatrixList.AddString(iter.path().filename().c_str());
			}
			//// ���͸��Ͻ�
			//else if (filesystem::is_directory(iter.status()))
			//{
			//	// ���� �߰�
			//	for (const auto& inIter : filesystem::directory_iterator(iter.path()))
			//	{
			//		if (filesystem::is_regular_file(inIter.status()))
			//		{
			//			// ���� ���ο� ������ ����
			//		}
			//	}
			//}
		}
	}
}


void AChrForm::OnLbnDblclkList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int indexSel = AChrForm::GetInstance()->m_MatrixList.GetCurSel();

	CString str;
	AChrForm::GetInstance()->m_MatrixList.GetText(indexSel,str);

	SampleMfc::GetInstance()->m_BoneObj = new ABoneObj;

	CString temp = 0;
	temp = "../data/MODEL/Guard_02/";

	temp += str;

	int index = I_OBJMGR.LoadObj(g_pd3dDevice.Get(), temp, _T("../../Data/Shader/MatrixViewer.hlsl"));
	SampleMfc::GetInstance()->m_BoneObj = (ABoneObj*)I_OBJMGR.GetPtr(index);

	SampleMfc::GetInstance()->m_CharObj->m_pBoneObject = SampleMfc::GetInstance()->m_BoneObj;
	SampleMfc::GetInstance()->m_CharObj->SetActionFrame(
		SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
		SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame);

	AChrForm::GetInstance()->m_SliderBar.SetRange(
		SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
		SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	AChrForm::GetInstance()->m_SliderBar.SetPos(SampleMfc::GetInstance()->m_CharObj->m_iCurrentFrame);
	m_iMin = SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame;
	m_iMax = SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame;
}

//�θ��̸��� ã�Ƽ� ������
AMesh* SampleMfc::SearchToCollects(T_STR	m_strParentName)
{
	for (DWORD dwGroup = 0; dwGroup < m_CharObj->m_pBoneObject->m_pMesh.size(); dwGroup++)
	{
		if (m_CharObj->m_pBoneObject->m_pMesh[dwGroup]->m_strNodeName == m_strParentName)
		{
			return  m_CharObj->m_pBoneObject->m_pMesh[dwGroup];
		}
	}
	return NULL;
}


HTREEITEM FindTreeData(CTreeCtrl* pTree, HTREEITEM hItem, DWORD dwData)
{
	HTREEITEM hitemFind, hItemChile, hItemSibling;
	hitemFind = hItemChile = hItemSibling = NULL;

	AChrForm2::GetInstance()->m_TreeList;

	if (AChrForm2::GetInstance()->m_TreeList.GetItemData(hItem) == dwData)
	{
		hitemFind = hItem;
	}
	else
	{
		// �ڽ� ��带 ã�´�.
		hItemChile = AChrForm2::GetInstance()->m_TreeList.GetChildItem(hItem);
		if (hItemChile)
		{
			hitemFind = FindTreeData(&AChrForm2::GetInstance()->m_TreeList, hItemChile, dwData);

		}

		// ������带 ã�´�.
		hItemSibling = AChrForm2::GetInstance()->m_TreeList.GetNextSiblingItem(hItem);
		if (hitemFind == NULL && hItemSibling)
		{
			hitemFind = FindTreeData(&AChrForm2::GetInstance()->m_TreeList, hItemSibling, dwData);
		}
	}

	return hitemFind;
}

int icnt = 0;
void MakeTreeList(AMesh* pMesh , HTREEITEM item)
{

	while (item !=NULL)
	{
		HTREEITEM Item = AChrForm2::GetInstance()->m_TreeList.InsertItem(pMesh->m_strNodeName.c_str(), 0, 0, item, TVI_LAST);
		if (pMesh->m_pChildMesh.size() <= icnt)
		{
			icnt = 0;
		}
		MakeTreeList(pMesh->m_pChildMesh[icnt++], Item);
	}
}


void AChrForm::OnBnClickedButtonChar()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	static TCHAR BASED_CODE szFilter[] = _T("�̹��� ����(*.cit) | *.CIT; *.cit;");
	CFileDialog fileDlg(TRUE, _T("*.cit"), 0, OFN_HIDEREADONLY, szFilter);

	if (IDOK == fileDlg.DoModal())
	{
		m_StrChar = fileDlg.GetPathName();
	}

	I_CHARMGR.Init();
	I_CHARMGR.Load(g_pd3dDevice.Get(), g_pImmediateContext.Get(), m_StrChar);

	SampleMfc::GetInstance()->m_CharObj = new AHeroObj;

	if (SampleMfc::GetInstance()->m_ModelObj != nullptr)
	{
		SAFE_DEL(SampleMfc::GetInstance()->m_ModelObj);
	}

	SampleMfc::GetInstance()->m_ModelObj = new AModelObj;


	SampleMfc::GetInstance()->m_CharObj->m_pChar = I_CHARMGR.GetPtr(0);
	SampleMfc::GetInstance()->m_ModelObj->Load(g_pd3dDevice.Get(), SampleMfc::GetInstance()->m_CharObj->m_pChar->m_pModelList[0]->m_pModel->m_szName.c_str(), _T("../../Data/Shader/SkinViewer.hlsl"), 0);

	SampleMfc::GetInstance()->m_CharObj->m_pBoneObject = SampleMfc::GetInstance()->m_CharObj->m_pChar->m_pBoneObject;
	SampleMfc::GetInstance()->m_CharObj->SetActionFrame(SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame, SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame);

	AChrForm::GetInstance()->m_SliderBar.SetRange(SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame, SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	AChrForm::GetInstance()->m_SliderBar.SetPos(SampleMfc::GetInstance()->m_CharObj->m_iCurrentFrame);
	m_iMin = SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame;
	m_iMax = SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AChrForm2::GetInstance()->m_TreeList.DeleteAllItems();
	for (int i = 0; i < SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_pMesh.size(); i++)
	{
		AMesh* pPoint = SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_pMesh[i];

		if (!pPoint->m_strParentName.empty())
		{
			auto* pParentNode = SampleMfc::GetInstance()->SearchToCollects(SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_pMesh[i]->m_strParentName);
			if (pParentNode)
			{
				pPoint->m_pParent = pParentNode;
				pParentNode->m_pChildMesh.push_back(pPoint);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SampleMfc::GetInstance()->AddTreeItem(m_CharObj->m_pBoneObject->m_pMesh[0]);

	for (int i = 0; i < SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_pMesh.size(); i++)
	{
		if (SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_pMesh[i]->m_pChildMesh.size() > 1)
		{
			SampleMfc::GetInstance()->AddTreeItem(SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_pMesh[i]);
			break;
		}
	}

}



void AChrForm::OnBnClickedBone()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	static TCHAR BASED_CODE szFilter[] = _T("�̹��� ����(*.mat) | *.MAT; *.mat;");
	CFileDialog fileDlg(TRUE, _T("*.mat"), 0, OFN_HIDEREADONLY, szFilter);


	if (IDOK == fileDlg.DoModal())
	{
		m_StrChar = fileDlg.GetPathName();
	}

	SampleMfc::GetInstance()->m_BoneObj = new ABoneObj;

	
	int index = I_OBJMGR.LoadObj(g_pd3dDevice.Get(), m_StrChar, _T("../../Data/Shader/MatrixViewer.hlsl"));
	SampleMfc::GetInstance()->m_BoneObj = (ABoneObj*)I_OBJMGR.GetPtr(index);

	SampleMfc::GetInstance()->m_CharObj->m_pBoneObject = SampleMfc::GetInstance()->m_BoneObj;
	SampleMfc::GetInstance()->m_CharObj->SetActionFrame(SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame, SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame);
	AChrForm::GetInstance()->m_SliderBar.SetRange(SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iFirstFrame,
		SampleMfc::GetInstance()->m_CharObj->m_pBoneObject->m_Scene.iLastFrame);

	AChrForm::GetInstance()->m_SliderBar.SetPos(SampleMfc::GetInstance()->m_CharObj->m_iCurrentFrame);

}







void AChrForm::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//int i = 0;
	//i = m_SliderBar.GetPos();
	//m_CharObj->m_iCurrentFrame = i;
	//SampleMfc::GetInstance()->m_bPlay = true;
	//AChrForm::GetInstance()->m_SliderBar.SetPos(m_CharObj->m_iCurrentFrame);
	////AChrForm::GetInstance()->m_SliderBar.SetRange(m_iMin,		m_iMax);

	*pResult = 0;
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
//	MessageBox(m_AseFileName,L"���� �����մϴ�!");
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
//		MessageBox(L"������ ������ �����ϴ�");
//	}
//}

//void AChrForm::OnBnClickedRadio1()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//
//
//	//if (m_ModelObj != nullptr)
//	//{
//	//	m_ModelObj->m_bVisible = false;
//
//	//}
//
//}
