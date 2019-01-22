#include "AHeroObj.h"



//void AHeroObj::CheckAABB(A_Box targetBox)
//{
//	m_CollsionBox.vMin.x;
//
//
//	if (m_CollsionBox.vMin.x <= targetBox.vMax.x && m_CollsionBox.vMax.x >= targetBox.vMin.x &&
//		m_CollsionBox.vMin.y <= targetBox.vMax.y && m_CollsionBox.vMax.y >= targetBox.vMin.y &&
//		m_CollsionBox.vMin.z <= targetBox.vMax.z && m_CollsionBox.vMax.z >= targetBox.vMin.z)
//	{
//		aabbCollide = true;
//		return;
//	}
//
//	aabbCollide = false;
//
//}
//void AHeroObj::CheckOBBInPlane(A_Box&  targetBox)
//{
//	
//	double c[3][3];
//	double absC[3][3];
//	double d[3];
//	double r0, r1, r;
//	int i;
//	const double cutoff = 0.999999;
//	bool existsParallelPair = false;
//	D3DXVECTOR3 diff = m_CollsionBox.vCenter - targetBox.vCenter;
//
//
//	for (i = 0; i < 3; ++i)
//	{
//		c[0][i] = D3DXVec3Dot(&m_CollsionBox.vAxis[0], &targetBox.vAxis[i]);
//		absC[0][i] = abs(c[0][i]);
//		if (absC[0][i] > cutoff)
//			existsParallelPair = true;
//	}
//
//
//	d[0] = D3DXVec3Dot(&diff, &m_CollsionBox.vAxis[0]);
//	r = abs(d[0]);
//	r0 = m_CollsionBox.fExtent[0];
//	r1 = targetBox.fExtent[0] * absC[0][0] + targetBox.fExtent[1] * absC[0][1] + targetBox.fExtent[2] * absC[0][2];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//	for (i = 0; i < 3; ++i)
//	{
//		c[1][i] = D3DXVec3Dot(&m_CollsionBox.vAxis[1], &targetBox.vAxis[i]);
//		absC[1][i] = abs(c[1][i]);
//		if (absC[1][i] > cutoff)
//			existsParallelPair = true;
//	}
//	d[1] = D3DXVec3Dot(&diff, &m_CollsionBox.vAxis[1]);
//	r = abs(d[1]);
//	r0 = m_CollsionBox.fExtent[1];
//	r1 = targetBox.fExtent[0] * absC[1][0] + targetBox.fExtent[1] * absC[1][1] + targetBox.fExtent[2] * absC[1][2];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//	for (i = 0; i < 3; ++i)
//	{
//		c[2][i] = D3DXVec3Dot(&m_CollsionBox.vAxis[2], &targetBox.vAxis[i]);
//		absC[2][i] = abs(c[2][i]);
//		if (absC[2][i] > cutoff)
//			existsParallelPair = true;
//	}
//	d[2] = D3DXVec3Dot(&diff, &m_CollsionBox.vAxis[2]);
//	r = abs(d[2]);
//	r0 = m_CollsionBox.fExtent[2];
//	r1 = targetBox.fExtent[0] * absC[2][0] + targetBox.fExtent[1] * absC[2][1] + targetBox.fExtent[2] * absC[2][2];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//	r = abs(D3DXVec3Dot(&diff, &targetBox.vAxis[0]));
//	r0 = m_CollsionBox.fExtent[0] * absC[0][0] + m_CollsionBox.fExtent[1] * absC[1][0] + m_CollsionBox.fExtent[2] * absC[2][0];
//	r1 = targetBox.fExtent[0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(D3DXVec3Dot(&diff, &targetBox.vAxis[1]));
//	r0 = m_CollsionBox.fExtent[0] * absC[0][1] + m_CollsionBox.fExtent[1] * absC[1][1] + m_CollsionBox.fExtent[2] * absC[2][1];
//	r1 = targetBox.fExtent[1];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(D3DXVec3Dot(&diff, &targetBox.vAxis[2]));
//	r0 = m_CollsionBox.fExtent[0] * absC[0][2] + m_CollsionBox.fExtent[1] * absC[1][2] + m_CollsionBox.fExtent[2] * absC[2][2];
//	r1 = targetBox.fExtent[2];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	if (existsParallelPair == true)
//	{
//		obbCollide = true;
//		return;
//	}
//
//
//	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
//	r0 = m_CollsionBox.fExtent[1] * absC[2][0] + m_CollsionBox.fExtent[2] * absC[1][0];
//	r1 = targetBox.fExtent[1] * absC[0][2] + targetBox.fExtent[2] * absC[0][1];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
//	r0 = m_CollsionBox.fExtent[1] * absC[2][1] + m_CollsionBox.fExtent[2] * absC[1][1];
//	r1 = targetBox.fExtent[0] * absC[0][2] + targetBox.fExtent[2] * absC[0][0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
//	r0 = m_CollsionBox.fExtent[1] * absC[2][2] + m_CollsionBox.fExtent[2] * absC[1][2];
//	r1 = targetBox.fExtent[0] * absC[0][1] + targetBox.fExtent[1] * absC[0][0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
//	r0 = m_CollsionBox.fExtent[0] * absC[2][0] + m_CollsionBox.fExtent[2] * absC[0][0];
//	r1 = targetBox.fExtent[1] * absC[1][2] + targetBox.fExtent[2] * absC[1][1];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
//	r0 = m_CollsionBox.fExtent[0] * absC[2][1] + m_CollsionBox.fExtent[2] * absC[0][1];
//	r1 = targetBox.fExtent[0] * absC[1][2] + targetBox.fExtent[2] * absC[1][0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
//	r0 = m_CollsionBox.fExtent[0] * absC[2][2] + m_CollsionBox.fExtent[2] * absC[0][2];
//	r1 = targetBox.fExtent[0] * absC[1][1] + targetBox.fExtent[1] * absC[1][0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
//	r0 = m_CollsionBox.fExtent[0] * absC[1][0] + m_CollsionBox.fExtent[1] * absC[0][0];
//	r1 = targetBox.fExtent[1] * absC[2][2] + targetBox.fExtent[2] * absC[2][1];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
//	r0 = m_CollsionBox.fExtent[0] * absC[1][1] + m_CollsionBox.fExtent[1] * absC[0][1];
//	r1 = targetBox.fExtent[0] * absC[2][2] + targetBox.fExtent[2] * absC[2][0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
//	r0 = m_CollsionBox.fExtent[0] * absC[1][2] + m_CollsionBox.fExtent[1] * absC[0][2];
//	r1 = targetBox.fExtent[0] * absC[2][1] + targetBox.fExtent[1] * absC[2][0];
//	if (r > r0 + r1)
//	{
//		obbCollide = false;
//		return;
//	}
//
//
//	obbCollide = true;
//	return;
//}
//


D3DXVECTOR3 AHeroObj::GetPlayerPos()
{
	return m_vCenter;
}
INT AHeroObj::GetPlayerPosX()
{
	return m_vCenter.x;
}
INT AHeroObj::GetPlayerPosY()
{
	return m_vCenter.y;
}
INT AHeroObj::GetPlayerPosZ()
{
	return m_vCenter.z;
}


bool AHeroObj::SetPlayerCharacter(const TCHAR* fileName, float x , float y, float z)
{
	m_pChar = I_CHARMGR.GetPtr(fileName);
	if (m_pChar == nullptr) {
		return false;
	}

	m_pBoneObject = m_pChar->m_pBoneObject;
	SetActionFrame(
		m_pBoneObject->m_Scene.iFirstFrame,
		m_pBoneObject->m_Scene.iLastFrame);


	m_matWorld._41 += x;
	m_matWorld._42 += y;
	m_matWorld._43 += z;
	SetCollisionData(m_matWorld);

	return true;
}



bool AHeroObj::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC Desc;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.ByteWidth = sizeof(CBConstBoneWorld);
	hr = g_pd3dDevice->CreateBuffer(&Desc, NULL, m_pCBConstBoneWorld.GetAddressOf());


	return true;
}

void AHeroObj::SetActionFrame(int iStart, int iEnd, int iCharacterMatrixIndex, int iModelMatrixIndex)
{
	m_iStartFrame = iStart;
	m_iLastFrame = iEnd;
	m_iCurrentFrame = (float)iStart;
	m_fFrame = m_iStartFrame;

	if (iCharacterMatrixIndex >= 0)
	{
		m_iMatrixIndex = iCharacterMatrixIndex;
	}
}

bool		AHeroObj::SetANIM(const TCHAR* szName)
{
	SetActionFrame(0, 0);

	m_pBoneObject = (ABoneObj*)I_OBJMGR.GetPtr(szName);
	if (m_pBoneObject == nullptr) { 
		return false; }
	SetActionFrame(
		m_pBoneObject->m_Scene.iFirstFrame,
		m_pBoneObject->m_Scene.iLastFrame);
	return true;
}

void AHeroObj::SetBoneMatrices(vector<D3DXMATRIX>* pList)
{
	for (UINT i = 0; i < pList->size(); i++)
	{
		D3DXMatrixTranspose(&m_cbBoneData.g_mConstBoneWorld[i], &((*pList)[i] * m_pMatrix[i]));
	}
}

bool		AHeroObj::Frame()
{
	//m_fLerpTime *= 0.1f;
	m_fFrame += (g_fSPF * m_fSpeed * m_pBoneObject->m_Scene.iFrameSpeed);
	m_iCurrentFrame = (int)m_fFrame;

	// 1프레임 사이 간격
	m_fLerpTime = m_fFrame - m_iCurrentFrame;
	if (m_pBoneObject->AniFrame(m_fFrame,
		m_fLerpTime,
		m_iStartFrame,
		m_iLastFrame,
		m_pMatrix))
	{
		m_iCurrentFrame = m_iStartFrame;
		m_fFrame = (float)m_iStartFrame + m_fLerpTime;
	}



	return true;
}

bool	AHeroObj::RenderInstancing(ID3D11DeviceContext*    pContext)
{
	// 메쉬 랜더링
	for (int iObj = 0; iObj < m_pChar->m_pModelList.size(); iObj++)
	{
		ASkinObj* pModel = (ASkinObj*)m_pChar->m_pModelList[iObj]->m_pModel;
		_ASSERT(pModel);
		//pModel->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pModel->SetMatrix(&m_pInstance[iObj].matWorld, &m_matView, &m_matProj);	//중요함


		if (m_bConstantBufferType)	//바인드포즈가 아닐경우?
		{
			SetBoneMatrices(pModel->GetMatrix());
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			pContext->Map(m_pCBConstBoneWorld.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
			memcpy(MappedResource.pData, &m_cbBoneData, sizeof(CBConstBoneWorld));
			pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);
			pContext->VSSetConstantBuffers(1, 1, m_pCBConstBoneWorld.GetAddressOf());
		}
		else
		{
			m_pBoneObject->SetBoneMatrices(pContext, m_pMatrix, pModel->GetMatrix());
			ID3D11ShaderResourceView* aRViews[1] = { m_pBoneObject->m_pBoneBufferRV };
			pContext->VSSetShaderResources(1, 1, aRViews);
		}
		pModel->RenderInstancing(pContext);

	}
	return true;
}


bool	AHeroObj::Render(ID3D11DeviceContext*    pContext)
{
	// 메쉬 랜더링
	for (int iObj = 0; iObj < m_pChar->m_pModelList.size(); iObj++)
	{
		ASkinObj* pModel = (ASkinObj*)m_pChar->m_pModelList[iObj]->m_pModel;
		_ASSERT(pModel);
		pModel->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		
		if (m_bConstantBufferType)	//바인드포즈가 아닐경우?
		{
			SetBoneMatrices(pModel->GetMatrix());
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			pContext->Map(m_pCBConstBoneWorld.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
			memcpy(MappedResource.pData, &m_cbBoneData, sizeof(CBConstBoneWorld));
			pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);
			pContext->VSSetConstantBuffers(1, 1, m_pCBConstBoneWorld.GetAddressOf());
		}
		else
		{
			//if (m_pChar->m_Scene.iBindPose > 0)
			m_pBoneObject->SetBoneMatrices(pContext, m_pMatrix, pModel->GetMatrix());	//pModel.m_matBipedList
			//else
			//	m_pBoneObject->SetBoneMatrices(pContext, m_pMatrix);
			ID3D11ShaderResourceView* aRViews[1] = { m_pBoneObject->m_pBoneBufferRV };
			pContext->VSSetShaderResources(1, 1, aRViews);
		}

		//ASkinObj* pModel = (ASkinObj*)m_pChar->m_pModelList[iObj]->m_pModel;
		//D3DXMATRIX* pMatrices;
		//HRESULT hr = S_OK;
		//D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		//if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
		//{
		//	pMatrices = (D3DXMATRIX*)MappedFaceDest.pData;
		//	for (DWORD dwObject = 0; dwObject < m_Scene.iNumMesh; dwObject++)	//씬갯수가 아닌  매쉬 갯수
		//	{
		//		pMatrices[dwObject] = (*pList)[dwObject] * pMatrix[dwObject];
		//		//pMatrices[dwObject] = (*pModel.m_matBipedList)[dwObject] * m_pMatrix[dwObject];
		//	}
		//	pContext->Unmap(m_pBoneBuffer, 0);
		//}

		pModel->Render(pContext);
	}
	// 본 오브젝트 랜더링
	if (m_bBoneRender==true)
	{
		memcpy(m_pBoneObject->m_pMatrix, m_pMatrix, m_pBoneObject->m_Scene.iNumMesh * sizeof(D3DXMATRIX));
		m_pBoneObject->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		m_pBoneObject->Render(pContext);
	}
	return true;
}

bool	AHeroObj::Release()
{
	//SAFE_DEL(m_PlayerCamera);
	SAFE_DEL(m_pBoneObject);
	SAFE_DEL(m_pChar);

	return true;
}


AHeroObj::AHeroObj()
{
	m_fSpeed = 1.0f;
	m_fFrame = 0.0f;
	m_fLerpTime = 0.0f;
	m_iStartFrame = 0;
	m_iLastFrame = 0;
	m_bConstantBufferType = false;
	m_bBoneRender = false;
}


AHeroObj::~AHeroObj()
{
}


