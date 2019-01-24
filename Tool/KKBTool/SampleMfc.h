#pragma once
#include "ACore.h"
#include "AAseObj.h"
#include "AChrForm.h"
#include "AChrForm2.h"
#include "DrawHelper.h"

#include "AModelObj.h"
#include "ABoneObj.h"
#include "AHeroObj.h"

#include "ACharacter.h"

#include <filesystem>

using namespace std;



class ATreeMesh
{
public:
	T_STR			 m_strName;
	ATreeMesh*	     m_pParent;
	list<ATreeMesh> m_ChildList;

public:
	ATreeMesh() {};
	~ATreeMesh() {};
};



class SampleMfc : public ACore
{
	static SampleMfc* instance;
public:
	static SampleMfc* GetInstance()
	{
		return instance;
	}
	 ALineShape* m_pLine;// = new;
	 DrawHelper  m_DrawHelper;

	 bool m_bPlay;
	


public:
	bool Init();

	bool Frame();
	bool Render();
	bool Release();


	void LoadMatrixList();
	void AddTreeMesh(AMesh* pObject, const HTREEITEM& parent, const int& level);
	void AddTreeItem(AMesh* pObject);
public:
	SampleMfc();
	virtual ~SampleMfc();
};




//for (int i = 0; i< pObj->m_MeshList.size(); i++)
//{
//	if (pObj->m_MeshList[i]->m_bSelect == true)
//	{
//		m_pLine->SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
//		D3DXVECTOR4 vColor = vColor = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
//		D3DXVECTOR3 vPoint[16];
//		vPoint[0] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[1] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[2] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[3] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[4] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[5] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[6] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[7] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[8] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[9] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[10] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[11] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[12] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[13] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		vPoint[14] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//		vPoint[15] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[0], vPoint[1], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[1], vPoint[3], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[2], vPoint[3], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[0], vPoint[2], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[4], vPoint[5], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[5], vPoint[7], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[6], vPoint[7], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[4], vPoint[6], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[8], vPoint[9], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[9], vPoint[11], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[10], vPoint[11], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[8], vPoint[10], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[12], vPoint[13], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[13], vPoint[15], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[14], vPoint[15], vColor);
//		m_pLine->Draw(m_pImmediateContext.Get(), vPoint[12], vPoint[14], vColor);
//	}
//}

//
//void AddTreeNode(AMesh* pObject, const HTREEITEM& parent, const int& level);
//
//void AddTreeMesh(AMesh* pObject, const HTREEITEM& parent, const int& level)
//{
//	for (auto& iter : pObject->m_pChildMesh)
//	{
//		auto ppparent = AChrForm2::GetInstance()->m_TreeList.InsertItem(iter->m_strNodeName.c_str(), 0, level, parent, TVI_SORT);
//
//		AChrForm2::GetInstance()->m_TreeList.SetItemData(ppparent, (DWORD_PTR)iter);
//
//		if (!iter->m_pChildMesh.empty())
//		{
//			AddTreeMesh(iter, ppparent, level + 1);
//		}
//	}
//}
//
//void AddTreeItem(AMesh* pObject)
//{
//	auto parent = AChrForm2::GetInstance()->m_TreeList.InsertItem(pObject->m_strNodeName.c_str(), 0, 0, TVI_ROOT, TVI_LAST);
//	AChrForm2::GetInstance()->m_TreeList.SetItemData(parent, (DWORD_PTR)pObject);
//	
//	AddTreeNode(pObject, parent, 1);
//}
//


