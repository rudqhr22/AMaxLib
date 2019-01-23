#include "AModelObj.h"

bool AModelObj::Set(ID3D11Device* pd3dDevice, int iIndex)
{
	Init();
	AModel* const pModel = I_OBJMGR.GetPtr(iIndex);
	if (pModel == NULL) return false;
	cbData.vColor = D3DXVECTOR4(1, 1, 1, 1);
	m_dxObj.g_pConstantBuffer.Attach(DX::CreateConstantBuffer(pd3dDevice, &cbData, 1, sizeof(VS_CONSTANT_BUFFER)));

	// 멀티 스레드로 처리되기 때문에 오브젝트 단위로 에미메이션 및 최종 행렬(인덱스)을 별도로 저장해 둔다. 
	ObjWM* pWM = new ObjWM(pModel->m_pMesh.size());
	//for (DWORD dwObject = 0; dwObject < pModel->m_pMesh.size(); dwObject++)
	//{
	//	pWM->m_iParent[dwObject] = -1;
	//	if (pModel->m_pMesh[dwObject]->m_pParent)
	//	{
	//		pWM->m_iParent[dwObject] = pModel->m_pMesh[dwObject]->m_pParent->m_iIndex;
	//	}
	//}
	pWM->m_pModel = pModel;
	pWM->m_Scene = pModel->m_Scene;
	m_pModelList.push_back(pWM);
	return true;
}

void AModelObj::Add(ID3D11Device* pDevice, AModelObj* pObj)
{
	m_pSubObjList.push_back(pObj);
}

void AModelObj::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	AModel::SetMatrix(pWorld, pView, pProj);
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		m_pModelList[iModel]->m_pModel->SetMatrix(pWorld, pView, pProj);
	}
	//for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	//{
	//	m_pSubObjList[iObj]->SetMatrix(pWorld, pView, pProj);		
	//}
}

bool AModelObj::Init()
{
	return true;
}

bool AModelObj::Load(ID3D11Device* pd3dDevice,
	const TCHAR* strFileName,
	const TCHAR* strShaderName,
	bool bThread)
{
	int iIndex = I_OBJMGR.LoadObj(pd3dDevice, strFileName, strShaderName, bThread);
	if (iIndex < 0)
	{
		I_OBJMGR.Delete(iIndex);
		return -1;
	}
	return Set(pd3dDevice, iIndex);
}

bool AModelObj::ObjectFrame()
{
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		ObjWM* pUnit = m_pModelList[iModel];
		pUnit->m_fElapseTime += g_fSPF * pUnit->m_Scene.iFrameSpeed * pUnit->m_Scene.iTickPerFrame;
		if (pUnit->m_fElapseTime >= pUnit->m_Scene.iLastFrame* pUnit->m_Scene.iTickPerFrame)
		{
			pUnit->m_fElapseTime = pUnit->m_Scene.iFirstFrame * pUnit->m_Scene.iTickPerFrame;
		}

		for (int iMesh = 0; iMesh < pUnit->m_Scene.iNumMesh; iMesh++)
		{
			D3DXMatrixIdentity(&pUnit->m_matAnim[iMesh]);
			D3DXMatrixIdentity(&pUnit->m_matCalculation[iMesh]);
			int iParent = pUnit->m_iParent[iMesh];
		
			D3DXMATRIX mat;
			
			if (iParent >= 0)
			{
				pUnit->m_matAnim[iMesh] = m_Animation.Update(pUnit->m_fElapseTime,
					pUnit->m_pModel->m_pMesh[iMesh],
					pUnit->m_matAnim[iParent],
					pUnit->m_Scene);
			}
			else
			{
				pUnit->m_matAnim[iMesh] = m_Animation.Update(pUnit->m_fElapseTime,
					pUnit->m_pModel->m_pMesh[iMesh],
					mat,
					pUnit->m_Scene);
			}
			D3DXMatrixMultiply(&pUnit->m_matCalculation[iMesh], &pUnit->m_matAnim[iMesh], &m_matWorld);
		}
	}
	return true;
}

bool AModelObj::ObjectRender(ID3D11DeviceContext*    pContext, bool bCommand)
{
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		ObjWM* pUnit = m_pModelList[iModel];
		PreDraw(pContext, pUnit, bCommand);
		PostDraw(pContext, pUnit, bCommand);
	}
	return true;
}

bool AModelObj::PreDraw(ID3D11DeviceContext*    pContext, ObjWM* pUnit, bool bCommand)
{
	pUnit->m_pModel->PreRender(pContext);
	return true;
}
bool AModelObj::PostDraw(ID3D11DeviceContext*    pContext, ObjWM* pUnit, bool bCommand)
{
	Draw(pContext, pUnit, bCommand);
	return true;
}



void AModelObj::SetConstantBuffers(ID3D11DeviceContext* pContext, ObjWM* pUint, int iMesh)
{
	//D3DXMatrixMultiply(&m_pObjWM->m_matCalculation[iMesh], &m_pObjWM->m_matAnim[iMesh], &m_matWorld);
	if (m_bDefferedMultiThread)
	{
		//D3DXMatrixMultiply(&m_pObjWM->m_matCalculation[iMesh], &m_pObjWM->m_matAnim[iMesh], &m_matWorld);
		// m_pModel은 공용 메쉬의 상수버퍼이기 때문에 동일한 오브젝트를 사용할 경우 멀티쓰래드에서 문제가 생긴다.
		// 그렇기 때문에 오브젝트 단위의 상수버퍼를 사용한다.		
		SetMatrix(&pUint->m_matCalculation[iMesh], &m_matView, &m_matProj);
		pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
		pContext->VSSetConstantBuffers(0, 1, m_dxObj.g_pConstantBuffer.GetAddressOf());
	}
	else
	{
		pUint->m_pModel->SetMatrix(&pUint->m_matCalculation[iMesh], &m_matView, &m_matProj);
		UpdateConstantBuffer(pContext, pUint->m_pModel);
		//D3DXMatrixMultiply(&pUint->m_matCalculation[iMesh], &pUint->m_matAnim[iMesh], &m_matWorld);
		//pUint->m_pModel->SetMatrix(&pUint->m_matCalculation[iMesh], &m_matView, &m_matProj);
		//UpdateConstantBuffer(pContext, pUint->m_pModel);
	}
}
bool AModelObj::Draw(ID3D11DeviceContext* pContext, ObjWM* pUint, bool bCommand)
{

	AModel* pModel = pUint->m_pModel;
	for (DWORD iMesh = 0; iMesh < pModel->m_pMesh.size(); iMesh++)
	{
		auto pMesh = pModel->m_pMesh[iMesh];
		if (bCommand == false)
		{
			SetConstantBuffers(pContext, pUint, iMesh);
		}

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub];
				if (pSubMesh->m_iNumFace < 1) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_dxObj.m_iNumIndex,
					pSubMesh->m_dxObj.m_iBeginIB,
					pSubMesh->m_dxObj.m_iBeginVB);
		//		if (bCommand) 	SetCommmandList(pContext, pSubMesh, true);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;
			pContext->PSSetShaderResources(0, 1, pMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_dxObj.m_iNumIndex,
				pMesh->m_dxObj.m_iBeginIB,
				pMesh->m_dxObj.m_iBeginVB);
//			if (bCommand) 	SetCommmandList(pContext, pMesh, true);
		}
	}
	
	return true;
}

//bool AModelObj::DrawGroup(ID3D11DeviceContext* pContext, AModel* pObj, int iObject,  bool bCommand)
//{
//	/*if (pObj->m_pMesh[iObject]->m_pSubMesh.size() > 0)
//	{
//		for (DWORD dwSubCount = 0; dwSubCount < pObj->m_pMesh[iObject]->m_pSubMesh.size(); dwSubCount++)
//		{
//			AMesh* pSubMesh = (AMesh*)pObj->m_pMesh[iObject]->m_pSubMesh[dwSubCount];
//			if (DrawMesh(pContext, pObj, pSubMesh, pApply))
//			{
//				if (bCommand) 	SetCommmandList(pContext, pSubMesh, true);
//			}
//		}
//	}
//	else
//	{
//		if (DrawMesh(pContext, pObj, pObj->m_pMesh[iObject], pApply))
//		{
//			if (bCommand) 	SetCommmandList(pContext, pObj->m_pMesh[iObject], true);
//		}
//	}*/
//	return true;
//}

bool AModelObj::Frame()
{
	if (m_pSubObjList.size() < 0)
	{
		ObjectFrame();
	}
	else 
	{
		for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
		{
			m_pSubObjList[iObj]->ObjectFrame();
		}
	}



	return true;
}
bool AModelObj::Render(ID3D11DeviceContext*    pContext)
{
	ObjectRender(pContext);
	//if (m_pSubObjList.size() < 0)
	//{
	//	ObjectRender(pContext);
	//}
	//else
	//{
	//	for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	//	{
	//		m_pSubObjList[iObj]->ObjectRender(pContext);
	//	}
	//}

	return true;
}
bool AModelObj::Release()
{
	m_pSubObjList.clear();
	return true;
}


//bool AModelObj::ResetResource()
//{
//	return true;
//}

void AModelObj::SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd)
{
	m_iStartFrame = dwStrat;
	m_iLastFrame = dwEnd;
	m_fElapseTime = (float)dwStrat;
	for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	{
		m_pSubObjList[iObj]->m_iStartFrame = dwStrat;
		m_pSubObjList[iObj]->m_iLastFrame = dwEnd;
		m_pSubObjList[iObj]->m_fElapseTime = (float)dwStrat;

	}
}

//bool AModelObj::SetCommandRender(ID3D11DeviceContext* pContext)
//{
//	//ObjectRender(pContext, true);
//
//	if (m_pSubObjList.size() < 0)
//	{
//		ObjectRender(pContext, true);
//	}
//	else
//	{
//		for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
//		{
//			m_pSubObjList[iObj]->ObjectRender(pContext, true);
//		}
//	}
//
//
//	return true;
//}

//bool AModelObj::CommandRender(ID3D11DeviceContext* pContext)
//{
//
//	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
//	{
//		ObjWM* pUnit = m_pModelList[iModel];
//		CommandDraw(pContext, pUnit);
//	}
//
//	/*for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
//	{
//	AModelObj* pObj = m_pSubObjList[iObj].get();
//	_ASSERT(pObj);
//	pObj->CommandRender(pContext);
//	}*/
//	return true;
//}
//
//bool AModelObj::CommandDraw(ID3D11DeviceContext* pContext, ObjWM* pUnit)
//{
//	HRESULT hr;
//	AModel* pModel = pUnit->m_pModel;
//	for (DWORD iMesh = 0; iMesh < pModel->m_pMesh.size(); iMesh++)
//	{
//		SetConstantBuffers(pContext, pUnit, iMesh);
//
//		if (pModel->m_pMesh[iMesh]->m_pSubMesh.size() > 0)
//		{
//			for (DWORD dwSubCount = 0; dwSubCount < pModel->m_pMesh[iMesh]->m_pSubMesh.size(); dwSubCount++)
//			{
//				AMesh* pSubMesh = (AMesh*)pModel->m_pMesh[iMesh]->m_pSubMesh[dwSubCount];
//				if (pSubMesh->m_iNumFace <= 0) continue;
//				ExecuteCommandList(pContext, pSubMesh, false);
//			}
//		}
//		else
//		{
//			if (pModel->m_pMesh[iMesh]->m_iNumFace <= 0) continue;
//			ExecuteCommandList(pContext, pModel->m_pMesh[iMesh], false);
//		}
//	}
//
//	return true;
//}
//
//HRESULT AModelObj::SetCommmandList(ID3D11DeviceContext* pContext, AMesh* pSubMesh, bool bSave)
//{
//	HRESULT hr = S_OK;
//	if (pContext)
//	{
//		return pContext->FinishCommandList(bSave, &pSubMesh->m_pd3dCommandList);
//	}
//	return E_FAIL;
//}
//
//void AModelObj::ExecuteCommandList(ID3D11DeviceContext* pContext, AMesh* pMesh, bool bClear)
//{
//	assert(pMesh && pMesh->m_pd3dCommandList != nullptr);
//	pContext->ExecuteCommandList(pMesh->m_pd3dCommandList.Get(), !bClear);
//
//	if (bClear)
//	{
//		pMesh->m_pd3dCommandList.ReleaseAndGetAddressOf();
//	}
//}

AModelObj::AModelObj(void)
{
	m_fElapseTime = 0.0f;
	m_bDefferedMultiThread = false;
}

AModelObj::AModelObj(int iIndex)
{
	m_fElapseTime = 0.0f;
	m_bDefferedMultiThread = false;
}

AModelObj::AModelObj(AModelObj &obj)
{
	m_fElapseTime = 0.0f;
	m_bDefferedMultiThread = obj.m_bDefferedMultiThread;
}

AModelObj::~AModelObj(void)
{
}

