#include "AAseObj.h"
#include "ATexture.h"
#include "ATimer.h"

bool		AAseObj::Load(ID3D11Device* pd3dDevice,	const TCHAR* szLoadName,	const TCHAR* pLoadShaderFile, bool bThread)
{
	m_dxObj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxObj.m_iIndexSize = sizeof(DWORD);

	m_Parser.Load(szLoadName);
	m_FileName = szLoadName;
	int iObjectType = -1;
	int iNumTypes = sizeof(AseSections) / sizeof(AseSections[0]);


	while ((iObjectType = m_Parser.SearchTokenArray(iNumTypes, AseSections, m_Parser.m_TokenMaxSize)) >= 0)
	{
		switch (iObjectType)
		{
		case SCENE:
		{
			if (!m_Parser.LoadScene(m_Scene))
				return false;

		}break;
		case MATERIALLIST:
		{
			if (!m_Parser.LoadMaterialList(m_MtrlList))
				return false;

		}break;
		case GEOMOBJECT:		
		{
			if (!m_Parser.LoadGeomObject(m_GeomObjList))
				return false;
		}break;
		case HELPEROBJECT:
		{
			if (!m_Parser.LoadHelperObject(m_GeomObjList))		return false;
		}break;
		default:
			return false;
		}
	}
	
	if (!Convert(pd3dDevice))
	{
		return false;
	}
	m_dxObj.m_iNumVertex = m_iMaxVertex;
	m_dxObj.m_iNumIndex = m_iMaxIndex;

	if (!Set(pd3dDevice, pLoadShaderFile, 0))
	{
		m_Parser.Release();
		return false;
	}
	
	//////////////////////////
	m_Parser.Release();
	m_MtrlList.clear();
	m_GeomObjList.clear();
	//////////////////////////



	return true;
}


//bool		AAseObj::Convert(ID3D11Device* pd3dDevice)
//{
//
//	PNCT_VERTEX vVertex;
//	for (DWORD A = 0; A < m_GeomObjList.size(); A++)
//	{
//		AMesh* pMesh = new AMesh;
//
//		pMesh->m_strNodeName = m_GeomObjList[A]->m_strNodeName;
//		pMesh->m_strParentName = m_GeomObjList[A]->m_strParentName;
//		pMesh->m_iNumFace = m_GeomObjList[A]->m_iNumFace;
//
//
//		pMesh->m_matWorld = m_GeomObjList[A]->m_matWorld;
//		pMesh->m_matCalculation = m_GeomObjList[A]->m_matWorld;
//		pMesh->m_matWorldTrans = m_GeomObjList[A]->m_matWorldTrans;
//		pMesh->m_matWorldRotate = m_GeomObjList[A]->m_matWorldRotate;
//		pMesh->m_matWorldScale = m_GeomObjList[A]->m_matWorldScale;
//
//		D3DXMatrixInverse(&pMesh->m_matInverse, NULL, &pMesh->m_matWorld);	//������� �־���?
//
//
//
//		if (pMesh->m_iNumFace > 0)
//		{
//			AMtrl* pMtrl = NULL;
//			int iRootMtrRef = m_GeomObjList[A]->m_iMtrlRef;
//			pMtrl = &m_MtrlList[iRootMtrRef];
//
//			SetMeshFaceList(m_GeomObjList[A], pMesh);
//
//
//			int iSubMtrl = m_MtrlList[m_GeomObjList[A]->m_iMtrlRef].m_SubMtrl.size();
//			if (iSubMtrl <= 0)	//���긶�׸��� ������
//			{
//				pMesh->MakeBuffer();
//
//				pMtrl = &m_MtrlList[m_GeomObjList[A]->m_iMtrlRef];
//				pMesh->m_iDiffuseTex = -1;
//
//				for (int iTex = 0; iTex < pMtrl->m_TexMaps.size(); iTex++)	//�ؽ�ó �ε��� ã��
//				{
//					AMtrl* pMtrl = &m_MtrlList[m_GeomObjList[A]->m_iMtrlRef];
//					pMesh->m_iDiffuseTex = GetMapID(pMtrl, ID_BASIS_DI);
//
//				}
//
//			}
//			else
//			{
//				for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
//				{
//					int iCount = 0;
//					AMesh* pSubMesh = new AMesh;
//
//					iCount += pSubMesh->MakeSubBuffer(pMesh->m_AseFaceList, iSub);
//
//					pSubMesh->m_iDiffuseTex = -1;
//
//					AMtrl* pSubMtrl = &pMtrl->m_SubMtrl[iSub];
//					//if (pSubMtrl->m_TexMaps.size() == 0) continue;
//
//					if (pSubMtrl)
//					{
//						pSubMesh->m_iDiffuseTex = GetMapID(pSubMtrl, ID_BASIS_DI);
//					}
//
//					if (pSubMesh->m_AseVertexList.size() > 0)
//					{
//						pMesh->m_pSubMesh.push_back(pSubMesh);
//					}
//
//				}
//			}
//
//		}
//
//
//		m_MeshList.push_back(pMesh);
//		ConvertAnim(m_GeomObjList[A], pMesh);
//
//	}
//
//	
//
//	D3DXMATRIX m_matInverse;
//	D3DXQUATERNION qR;
//	D3DXVECTOR3 vTrans, vScale;
//
//	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
//	{
//		if (!m_MeshList[dwObject]->m_strParentName.empty())
//		{
//			AMesh* pParentNode = SearchToCollects(m_MeshList[dwObject]->m_strParentName);
//			if (pParentNode)
//			{
//				m_MeshList[dwObject]->m_pParent = pParentNode;
//
//				m_matInverse = m_MeshList[dwObject]->m_matWorld * pParentNode->m_matInverse;
//				D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_matInverse);
//				D3DXMatrixScaling(&m_MeshList[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
//				D3DXMatrixTranslation(&m_MeshList[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
//				D3DXMatrixRotationQuaternion(&m_MeshList[dwObject]->m_matWorldRotate, &qR);
//				pParentNode->m_pChildMesh.push_back(m_MeshList[dwObject]);
//			}
//		}
//	}
//
//	//for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
//	//{
//	//	if (m_MeshList[dwObject]->m_pChildMesh.size() <= 0 &&
//	//		(m_MeshList[dwObject]->m_ClassType == CLASS_BONE || m_MeshList[dwObject]->m_ClassType == CLASS_DUMMY))
//	//	{
//	//		m_MeshList[dwObject]->m_bUsed = false;
//	//	}
//	//}
//
//
//	m_GeomObjList.clear();
//	m_MtrlList.clear();
//
//	return true;
//}

//
//bool		AAseObj::Convert(ID3D11Device* pd3dDevice)
//{
//
//	PNCT_VERTEX vVertex;
//	for (DWORD A = 0; A < m_GeomObjList.size(); A++)
//	{
//		AMesh* pMesh = new AMesh;
//
//		pMesh->m_strNodeName = m_GeomObjList[A]->m_strNodeName;
//		pMesh->m_strParentName = m_GeomObjList[A]->m_strParentName;
//		pMesh->m_iNumFace = m_GeomObjList[A]->m_iNumFace;
//		pMesh->m_iDiffuseTex = -1;
//
//		pMesh->m_matWorld = m_GeomObjList[A]->m_matWorld;
//		pMesh->m_matCalculation = m_GeomObjList[A]->m_matWorld;
//		pMesh->m_matWorldTrans = m_GeomObjList[A]->m_matWorldTrans;
//		pMesh->m_matWorldRotate = m_GeomObjList[A]->m_matWorldRotate;
//		pMesh->m_matWorldScale = m_GeomObjList[A]->m_matWorldScale;
//
//		D3DXMatrixInverse(&pMesh->m_matInverse, NULL, &pMesh->m_matWorld);	//������� �־���?
//
//
//
//
//		if (pMesh->m_iNumFace > 0)
//		{
//			AMtrl* pMtrl = NULL;
//			int iRootMtrRef = m_GeomObjList[A]->m_iMtrlRef;
//			pMtrl = &m_MtrlList[iRootMtrRef];
//
//			SetMeshFaceList(m_GeomObjList[A], pMesh);
//			//SetTriangleBuffer(m_GeomObjList[A], pMesh);
//
//			int iSubMtrl = m_MtrlList[m_GeomObjList[A]->m_iMtrlRef].m_SubMtrl.size();
//
//			if (iSubMtrl <= 0)	//���긶�׸��� ������
//			{
//				pMesh->m_iDiffuseTex = -1;
//				pMesh->MakeBuffer();
//
//				for (int iTex = 0; iTex < pMtrl->m_TexMaps.size(); iTex++)	//�ؽ�ó �ε��� ã��
//				{
//					AMtrl* pMtrl = &m_MtrlList[m_GeomObjList[A]->m_iMtrlRef];
//					if (pMtrl->m_TexMaps[iTex].m_dwType == ID_BASIS_DI)
//					{
//						pMesh->m_iDiffuseTex = pMtrl->m_TexMaps[iTex].m_dwIndex;
//					}
//				}
//				m_iMaxVertex += pMesh->m_AseVertexList.size();
//				m_iMaxIndex += pMesh->m_AseIndexList.size();
//			}
//			else
//			{
//				int iCount = 0;
//				for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
//				{
//
//					AMesh* pSubMesh = new AMesh;
//
//					iCount += pSubMesh->MakeSubBuffer(pMesh->m_AseFaceList, iSub, iCount);
//
//					pSubMesh->m_iDiffuseTex = -1;
//
//					AMtrl* pSubMtrl = &pMtrl->m_SubMtrl[iSub];
//
//					pSubMesh->m_iNumFace = pSubMesh->m_AseIndexList.size() / 3;
//
//					if (pSubMtrl)
//					{
//						pSubMesh->m_iDiffuseTex = GetMapID(pSubMtrl, ID_BASIS_DI);
//					}
//
//					if (pSubMesh->m_AseVertexList.size() > 0)
//					{
//						pMesh->m_pSubMesh.push_back(pSubMesh);
//					}
//					m_iMaxVertex += pSubMesh->m_AseVertexList.size();
//					m_iMaxIndex += pSubMesh->m_AseIndexList.size();
//				}
//
//
//			}
//		}
//
//		m_MeshList.push_back(pMesh);
//		ConvertAnim(m_GeomObjList[A], pMesh);
//
//	}
//
//
//	D3DXMATRIX m_matInverse;
//	D3DXQUATERNION qR;
//	D3DXVECTOR3 vTrans, vScale;
//
//	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
//	{
//		if (!m_MeshList[dwObject]->m_strParentName.empty())
//		{
//			AMesh* pParentNode = SearchToCollects(m_MeshList[dwObject]->m_strParentName);
//			if (pParentNode)
//			{
//				m_MeshList[dwObject]->m_pParent = pParentNode;
//
//				m_matInverse = m_MeshList[dwObject]->m_matWorld * pParentNode->m_matInverse;
//				D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_matInverse);
//				D3DXMatrixScaling(&m_MeshList[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
//				D3DXMatrixTranslation(&m_MeshList[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
//				D3DXMatrixRotationQuaternion(&m_MeshList[dwObject]->m_matWorldRotate, &qR);
//				pParentNode->m_pChildMesh.push_back(m_MeshList[dwObject]);
//			}
//		}
//	}
//
//
//	m_GeomObjList.clear();
//	m_MtrlList.clear();
//
//	return true;
//}



bool		AAseObj::Convert(ID3D11Device* pd3dDevice)
{

	PNCT_VERTEX vVertex;
	for (DWORD A = 0; A < m_GeomObjList.size(); A++)
	{
		AMesh* pMesh = new AMesh;

		pMesh->m_strNodeName = m_GeomObjList[A]->m_strNodeName;
		pMesh->m_strParentName = m_GeomObjList[A]->m_strParentName;
		pMesh->m_iNumFace = m_GeomObjList[A]->m_iNumFace;
		pMesh->m_iDiffuseTex = -1;

		pMesh->m_matWorld = m_GeomObjList[A]->m_matWorld;
		pMesh->m_matCalculation = m_GeomObjList[A]->m_matWorld;
		pMesh->m_matWorldTrans = m_GeomObjList[A]->m_matWorldTrans;
		pMesh->m_matWorldRotate = m_GeomObjList[A]->m_matWorldRotate;
		pMesh->m_matWorldScale = m_GeomObjList[A]->m_matWorldScale;

		pMesh->m_ClassType = m_GeomObjList[A]->m_ClassType;

		D3DXMatrixInverse(&pMesh->m_matInverse, NULL, &pMesh->m_matWorld);	//������� �־���?


	

		if (pMesh->m_iNumFace > 0)
		{
			AMtrl* pMtrl = NULL;
			int iRootMtrRef = m_GeomObjList[A]->m_iMtrlRef;
			pMtrl = &m_MtrlList[iRootMtrRef];

			SetTriangleBuffer(m_GeomObjList[A], pMesh);

			int iSubMtrl = m_MtrlList[m_GeomObjList[A]->m_iMtrlRef].m_SubMtrl.size();

			if (iSubMtrl <= 0)	//���긶�׸��� ������
			{
				pMesh->m_iDiffuseTex = -1;
				pMesh->SetUniqueBuffer(pMesh->m_TriList, -1, 0);

				for (int iTex = 0; iTex < pMtrl->m_TexMaps.size(); iTex++)	//�ؽ�ó �ε��� ã��
				{
					AMtrl* pMtrl = &m_MtrlList[m_GeomObjList[A]->m_iMtrlRef];
					if (pMtrl->m_TexMaps[iTex].m_dwType == ID_BASIS_DI)
					{
						pMesh->m_iDiffuseTex = pMtrl->m_TexMaps[iTex].m_dwIndex;
					}
				}
				m_iMaxVertex += pMesh->m_AseVertexList.size();
				m_iMaxIndex  += pMesh->m_AseIndexList.size();
			}
			else
			{
				int iCount = 0;
				for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
				{
					AMesh* pSubMesh = new AMesh;

					iCount += pSubMesh->SetUniqueBuffer(pMesh->m_TriList, iSub, iCount);

					pSubMesh->m_iDiffuseTex = -1;

					AMtrl* pSubMtrl = &pMtrl->m_SubMtrl[iSub];

					pSubMesh->m_iNumFace = pSubMesh->m_AseIndexList.size()/3;

					if (pSubMtrl)
					{
						pSubMesh->m_iDiffuseTex = GetMapID(pSubMtrl, ID_BASIS_DI);
					}

					if (pSubMesh->m_AseVertexList.size() > 0)
					{
						pMesh->m_pSubMesh.push_back(pSubMesh);
					}
					m_iMaxVertex += pSubMesh->m_AseVertexList.size();
					m_iMaxIndex += pSubMesh->m_AseIndexList.size();
				}
			}
		}
		else
		{
		
			pMesh->m_Box.vMin = m_GeomObjList[A]->m_Box.vMin;
			pMesh->m_Box.vMax = m_GeomObjList[A]->m_Box.vMax;
		}

		m_MeshList.push_back(pMesh);
		ConvertAnim(m_GeomObjList[A], pMesh);

	}

	//��Ӱ��� ���߱�	inheritance
	//
	D3DXMATRIX m_matInverse;
	D3DXQUATERNION qR;
	D3DXVECTOR3 vTrans, vScale;

	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		if (!m_MeshList[dwObject]->m_strParentName.empty())
		{
			AMesh* pParentNode = SearchToCollects(m_MeshList[dwObject]->m_strParentName);
			if (pParentNode)
			{
				m_MeshList[dwObject]->m_pParent = pParentNode;

				m_matInverse = m_MeshList[dwObject]->m_matWorld * pParentNode->m_matInverse;				//�ڽ��� ������� = �ڽĿ������ * �θ��� �����

				
				D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_matInverse);									//�ڽ��� ���������  ����


				//�ڽ��� SRT�� SRT�� ����
				D3DXMatrixScaling(&m_MeshList[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
				D3DXMatrixTranslation(&m_MeshList[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
				D3DXMatrixRotationQuaternion(&m_MeshList[dwObject]->m_matWorldRotate, &qR);

				pParentNode->m_pChildMesh.push_back(m_MeshList[dwObject]);
			
			}
		}
	}

	// ������ ���� ������Ʈ�� false ���� �ְ� �ִϸ��̼��� ������ �ʴ´�.
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		if (m_MeshList[dwObject]->m_pChildMesh.size() <= 0 && 
			(m_MeshList[dwObject]->m_ClassType == CLASS_BONE || m_MeshList[dwObject]->m_ClassType == CLASS_DUMMY))
		{
			m_MeshList[dwObject]->m_bUnAnimObj = true;
		}
	}

	m_GeomObjList.clear();
	m_MtrlList.clear();

	return true;
}


bool		AAseObj::ConvertAnim(AGeomMesh* GObj, AMesh* pMesh)
{
	AAnimTrack* pPrevTrack = NULL;
	for (int iTrack = 0; iTrack < GObj->m_PosTrack.size(); iTrack++)
	{
		AAnimTrack* pTrack = new AAnimTrack;

		pTrack->iTick = GObj->m_PosTrack[iTrack].iTick;
		pTrack->vVector = GObj->m_PosTrack[iTrack].vVector;

		


		pPrevTrack = SetLinkedTrack(pTrack, pPrevTrack);
		pMesh->m_PosTrack.push_back(pTrack);
	}

	pPrevTrack = NULL;
	for (int iTrack = 0; iTrack < GObj->m_RotTrack.size(); iTrack++)
	{
		AAnimTrack* pTrack = new AAnimTrack;

		pTrack->iTick = GObj->m_RotTrack[iTrack].iTick;
		//ȸ���� ���Ͱ� �ȵ�
	//	pTrack->vVector = GObj->m_RotTrack[iTrack].vVector;

		//ȸ������, ����3,�ޱ�		
		//������ ���� �̿��ؿ� ������� ��ȯ�Ͽ� ����
		D3DXQuaternionRotationAxis(&pTrack->qRotate,		//������ ���� ȸõ�����μ� ���ϴϿ��� ȸ����Ų��
			&D3DXVECTOR3(GObj->m_RotTrack[iTrack].qRotate.x,
						GObj->m_RotTrack[iTrack].qRotate.y,
						GObj->m_RotTrack[iTrack].qRotate.z),
						GObj->m_RotTrack[iTrack].qRotate.w);


		if (pPrevTrack != NULL)		
		{	//���� ȸ���� �ִٸ� q1 ,q2 �� ���Ѵ�
			D3DXQuaternionMultiply(&pTrack->qRotate, &pPrevTrack->qRotate, &pTrack->qRotate);
		}

	pPrevTrack = SetLinkedTrack(pTrack, pPrevTrack);
		pMesh->m_RotTrack.push_back(pTrack);
	}

	pPrevTrack = NULL;
	for (int iTrack = 0; iTrack < GObj->m_SclTrack.size(); iTrack++)
	{
		AAnimTrack* pTrack = new AAnimTrack;

		pTrack->iTick = GObj->m_SclTrack[iTrack].iTick;
		pTrack->vVector = GObj->m_SclTrack[iTrack].vVector;

		//ȸ������, ����3,�ޱ�
		D3DXQuaternionRotationAxis(&pTrack->qRotate,	//������ ���� ȸõ�����μ� ���ϴϿ��� ȸ����Ų��
			&D3DXVECTOR3(GObj->m_SclTrack[iTrack].qRotate.x,
				GObj->m_SclTrack[iTrack].qRotate.y,
				GObj->m_SclTrack[iTrack].qRotate.z),
			GObj->m_SclTrack[iTrack].qRotate.w);	//�ޱ۰�



		pPrevTrack = SetLinkedTrack(pTrack, pPrevTrack);
		pMesh->m_SclTrack.push_back(pTrack);
	}
	





	return true;
}


void		AAseObj::SetTriangleBuffer(AGeomMesh* GObjList, AMesh* pData)
{
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &GObjList->m_matWorld);

	pData->m_TriList.clear();
	pData->m_TriList.reserve(GObjList->m_iNumFace);
	pData->m_TriList.resize(GObjList->m_iNumFace);
	pData->m_iNumFace = GObjList->m_iNumFace;

	D3DXVECTOR3 vVertex;
	pData->m_Box.vMin = D3DXVECTOR3(999999.0f, 999999.0f, 999999.0f);
	pData->m_Box.vMax = D3DXVECTOR3(-999999.0f, -999999.0f, -999999.0f);

	int Index = 0;
	for (DWORD dwFace = 0; dwFace < GObjList->m_iNumFace; dwFace++)
	{
		DWORD dwMtrl = GObjList->m_PosVertexList.pFaceList[dwFace].dwMtrl;
		pData->m_TriList[dwFace].iSubIndex = dwMtrl;

		if (GObjList->m_NorVertexList.pVertexList.size() > 0)
		{
			pData->m_TriList[dwFace].vNormal = GObjList->m_NorVertexList.pVertexList[dwFace * 4];
		}

		for (DWORD dwVertex = 0; dwVertex < 3; dwVertex++)
		{
			if (GObjList->m_PosVertexList.dwNumVertex >0 && GObjList->m_PosVertexList.dwNumFace >0)
			{
				pData->m_TriList[dwFace].vVertex[dwVertex].p =
					GObjList->m_PosVertexList.pVertexList[GObjList->m_PosVertexList.pFaceList[dwFace].v[dwVertex]];
			}

			if (GObjList->m_NorVertexList.pVertexList.size() > 0)
			{
				pData->m_TriList[dwFace].vVertex[dwVertex].n = GObjList->m_NorVertexList.pVertexList[dwFace * 4 + dwVertex + 1];
			}

			if (GObjList->m_ColVertexList.dwNumVertex && GObjList->m_ColVertexList.dwNumFace)
			{
				pData->m_TriList[dwFace].vVertex[dwVertex].c.x = GObjList->m_ColVertexList.pVertexList[GObjList->m_ColVertexList.pFaceList[dwFace].v[dwVertex]].x;
				pData->m_TriList[dwFace].vVertex[dwVertex].c.y = GObjList->m_ColVertexList.pVertexList[GObjList->m_ColVertexList.pFaceList[dwFace].v[dwVertex]].z;
				pData->m_TriList[dwFace].vVertex[dwVertex].c.z = GObjList->m_ColVertexList.pVertexList[GObjList->m_ColVertexList.pFaceList[dwFace].v[dwVertex]].y;
				pData->m_TriList[dwFace].vVertex[dwVertex].c.w = 1.0f;
			}
			else
			{
				pData->m_TriList[dwFace].vVertex[dwVertex].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (GObjList->m_TexVertexList.dwNumVertex && GObjList->m_TexVertexList.dwNumFace)
			{
				float fV = GObjList->m_TexVertexList.pVertexList[GObjList->m_TexVertexList.pFaceList[dwFace].v[dwVertex]].x;

				pData->m_TriList[dwFace].vVertex[dwVertex].t.x = fV;
				pData->m_TriList[dwFace].vVertex[dwVertex].t.y = 1.0f - GObjList->m_TexVertexList.pVertexList[GObjList->m_TexVertexList.pFaceList[dwFace].v[dwVertex]].z;
			}

			D3DXVec3TransformCoord(&pData->m_TriList[dwFace].vVertex[dwVertex].p, &pData->m_TriList[dwFace].vVertex[dwVertex].p, &matInverse);
			vVertex = pData->m_TriList[dwFace].vVertex[dwVertex].p;
			if (pData->m_Box.vMin.x > vVertex.x) pData->m_Box.vMin.x = vVertex.x;
			if (pData->m_Box.vMin.y > vVertex.y) pData->m_Box.vMin.y = vVertex.y;
			if (pData->m_Box.vMin.z > vVertex.z) pData->m_Box.vMin.z = vVertex.z;
			if (pData->m_Box.vMax.x < vVertex.x) pData->m_Box.vMax.x = vVertex.x;
			if (pData->m_Box.vMax.y < vVertex.y) pData->m_Box.vMax.y = vVertex.y;
			if (pData->m_Box.vMax.z < vVertex.z) pData->m_Box.vMax.z = vVertex.z;
		}
	}
	std::sort(pData->m_TriList.begin(), pData->m_TriList.end(), DescendingTriSort());
}



bool		AAseObj::CreateVertexBuffer()
{
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				AMesh* pSubMesh = pMesh->m_pSubMesh[dwSub];
				if (pSubMesh->m_AseVertexList.size() < 3) continue;

				pSubMesh->m_pVertexBuffer.Attach(DX::CreateVertexBuffer(g_pd3dDevice.Get(),
					&pSubMesh->m_AseVertexList.at(0),
					pSubMesh->m_AseVertexList.size(),
					sizeof(PNCT_VERTEX)));

			}
		}
		else 
		{
			if (pMesh->m_AseVertexList.size() < 3) continue;
			pMesh->m_pVertexBuffer.Attach(DX::CreateVertexBuffer(g_pd3dDevice.Get(),
				&pMesh->m_AseVertexList.at(0),
				pMesh->m_AseVertexList.size(),
				sizeof(PNCT_VERTEX)));

		}
	}

	return true;
};

bool		AAseObj::CreateIndexBuffer()
{
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				AMesh* pSubMesh = pMesh->m_pSubMesh[dwSub];
				if (pSubMesh->m_AseVertexList.size() < 3) continue;
				pSubMesh->m_pIndexBuffer.Attach(DX::CreateIndexBuffer(g_pd3dDevice.Get(),
					&pSubMesh->m_AseIndexList.at(0),
					pSubMesh->m_AseIndexList.size(),
					sizeof(DWORD)));
			}
		}
		else
		{
			if (pMesh->m_AseVertexList.size() < 3) continue;
			pMesh->m_pIndexBuffer.Attach(DX::CreateIndexBuffer(g_pd3dDevice.Get(),
				&pMesh->m_AseIndexList.at(0),
				pMesh->m_AseIndexList.size(),
				sizeof(DWORD)));
		}
	}
	return true;
};


bool		AAseObj::LoadTexture(ID3D11Device* device, const TCHAR* fileName)
{

	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				if (pMesh->m_pSubMesh[dwSub]->m_iDiffuseTex < 0) continue;
				pMesh->m_pSubMesh[dwSub]->m_pTextureSRV = I_TEXTURE.GetPtr(pMesh->m_pSubMesh[dwSub]->m_iDiffuseTex)->m_TextureSRV;
			}
		}
		else
		{
			if (pMesh->m_iDiffuseTex < 0) continue;
			pMesh->m_pTextureSRV = I_TEXTURE.GetPtr(pMesh->m_iDiffuseTex)->m_TextureSRV;
		}
	}
	
	////m_GeomObjList[asd]->m_iMtrlRef;
	//m_MtrlList;
	//m_MtrlList[0].m_TexMaps[0].m_strTextureName;
	//m_dxObj.g_pTextureSRV = I_TEXTURE.GetPtr(m_MtrlList[0].m_TexMaps[0].m_dwIndex)->m_TextureSRV;

	return true;
}


bool		AAseObj::Frame()
{
	m_fElapseTime += g_fSPF *  m_Scene.iFrameSpeed * m_Scene.iTickPerFrame /* * m_fTickSpeed*/;

	if (m_fElapseTime >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)	//��Ʈ �������̸�
	{
		m_fElapseTime = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;	//ó�����������ΰ���
	}
	

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);	//�θ� ���� ��쿡 ���� ���� ���
	
	
	for (DWORD A = 0; A < m_MeshList.size(); A++)
	{
		AMesh* pMesh = m_MeshList[A];
		if (pMesh->m_bUnAnimObj == true)
		{
			continue;
		}
	
		if (pMesh->m_pParent==NULL)
		{
			AnimPlay(pMesh, &matIdent, m_fElapseTime);
		}
		else
		{
			AnimPlay(pMesh, &pMesh->m_pParent->m_matCalculation, m_fElapseTime);
		}


		//if (pMesh->m_pParent == NULL)
		//{
		//	AnimPlay(pMesh, &matIdent, m_fElapseTime);
		//}
		//else
		//{
		//	if (pMesh->m_pParent)
		//	{
		//		AnimPlay(pMesh, &pMesh->m_pParent->m_matCalculation, m_fElapseTime);
		//	}
		//	else
		//	{
		//		AnimPlay(pMesh, &matIdent, m_fElapseTime);
		//	}
		//}
	

	}

	return true;
}


void		AAseObj::AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents ,float fFrameTick)
{
	D3DXQUATERNION qR, qS;	//����� ȸ��, ����� ������
	D3DXMatrixIdentity(&pMesh->m_matCalculation);		//������ Į�ڸ���Ŀ� ����ȴ�.
	D3DXMATRIX matAnim, matPos, matRotate, matScale;	//�ִ����, T, R ,S

	matScale = pMesh->m_matWorldScale;					//����� �����ؾ���
	matRotate = pMesh->m_matWorldRotate;				//����� �����ؾ���
	matPos = pMesh->m_matWorldTrans;					//���ͺ���

	D3DXQuaternionRotationMatrix(&qR, &matRotate);		//��ķκ��� ����� ����
	D3DXQuaternionRotationMatrix(&qS, &matScale);		//��ķκ��� ����� ����

	float fStartTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	float fEndTick = 0.0f;

	//���� ������ NULL�� �����ָ� ġ������ ���� �߻�?
	//���� R P S ������ ���ϸ� �ִϸ��̼��� ������.
	AAnimTrack* pStartTrack = NULL;
	AAnimTrack* pEndTrack = NULL;
	
	if (pMesh->m_RotTrack.size())
	{
		if (GetAnimationTrack(fFrameTick, pMesh->m_RotTrack, &pStartTrack, &pEndTrack))
		{
			qR = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			//����� ���� ����
			D3DXQuaternionSlerp(&qR,
				&qR,
				&pEndTrack->qRotate,
				PlayTrackTick(fFrameTick, fStartTick, fEndTick));
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qR);
	}


	pStartTrack = NULL;
	pEndTrack = NULL;
	D3DXVECTOR3 Trans(matPos._41, matPos._42, matPos._43);
	if (pMesh->m_PosTrack.size())
	{
		// pStartTrack�� ã���� ������
		if (GetAnimationTrack(fFrameTick, pMesh->m_PosTrack, &pStartTrack, &pEndTrack))
		{
			Trans = pStartTrack->vVector;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;	
			//���� ���� ����
			D3DXVec3Lerp(&Trans,
				&Trans, 
				&pEndTrack->vVector, 
				PlayTrackTick(fFrameTick, fStartTick, fEndTick));
		}

		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);
	}



	pStartTrack = NULL;
	pEndTrack = NULL;

	D3DXMATRIX matScaleRot, matInvScaleRot;
	D3DXVECTOR3 vScale(matScale._11, matScale._22, matScale._33);

	if (pMesh->m_SclTrack.size())
	{
		if (GetAnimationTrack(fFrameTick,pMesh->m_SclTrack, &pStartTrack, &pEndTrack))
		{
			vScale = pStartTrack->vVector;
			qS = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			//���ͺ���
			D3DXVec3Lerp(&vScale,
				&vScale,
				&pEndTrack->vVector, 
				PlayTrackTick(fFrameTick, fStartTick, fEndTick));


			//���������
			D3DXQuaternionSlerp(&qS,
				&qS,
				&pEndTrack->qRotate,
				PlayTrackTick(fFrameTick, fStartTick, fEndTick));

		}

		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);	//������ ����� ����
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);			//�����ȸ������� �����.
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);		//������ȸ���� ������� ���� 
		matScale = matInvScaleRot * matScale * matScaleRot;			//���� ������ ��� || (���� �̳� �ڽ�����) ���ư��� ������ ����ó���� �Ѵ�.
	}


	//������� �۾�
	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);		
	matAnim._41 =  matPos._41;	
	matAnim._42 =  matPos._42;	
	matAnim._43 =  matPos._43;	


	// ���� ���̸��̼� ����� �ϼ��Ѵ�.	(�θ���ĸ� ���ؼ�)
	D3DXMatrixMultiply(&pMesh->m_matCalculation, &matAnim, matParents);

}




bool		AAseObj::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	//AseRender(pContext,this);
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		UINT stride = sizeof(PNCT_VERTEX);
		UINT offset = 0;

		//pMesh->m_matCalculation = pMesh->m_matCalculation;// *m_matControlWorld;		//���Ŀ� ��Ʈ�� ����� ���� ���̴�.
		//this->SetMatrix(&pMesh->m_matCalculation, &this->m_matView, &this->m_matProj);
		
		SetMatrix(&pMesh->m_matCalculation, &this->m_matView, &this->m_matProj);
	
		UpdateConstantBuffer(pContext);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				AMesh* pSubMesh = pMesh->m_pSubMesh[dwSub];
			
				//SetMatrix(&pSubMesh->m_matWorld, &m_matView, &m_matProj);
				pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
				pContext->IASetVertexBuffers(0, 1, pSubMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
				pContext->IASetIndexBuffer(pSubMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_pTextureSRV.GetAddressOf());

				pContext->DrawIndexed(pSubMesh->m_AseIndexList.size(), 0, 0);
			}
		}
		else
		{
			pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
			pContext->IASetVertexBuffers(0, 1, pMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
			pContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 1, pMesh->m_pTextureSRV.GetAddressOf());

			pContext->DrawIndexed(pMesh->m_AseIndexList.size(), 0, 0);
		}
	}

	return true;
}




bool AAseObj::AseRender(ID3D11DeviceContext*    pContext, AModel* pParent)
{
	//PreRender(pContext);
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		UINT stride = sizeof(PNCT_VERTEX);
		UINT offset = 0;

		//pMesh->m_matCalculation = pMesh->m_matCalculation;// *m_matControlWorld;		//���Ŀ� ��Ʈ�� ����� ���� ���̴�.
		//this->SetMatrix(&pMesh->m_matCalculation, &this->m_matView, &this->m_matProj);

		pParent->SetMatrix(&pMesh->m_matCalculation, &pParent->m_matView, &pParent->m_matProj);

		UpdateConstantBuffer(pContext, pParent);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				AMesh* pSubMesh = pMesh->m_pSubMesh[dwSub];

				SetMatrix(&pSubMesh->m_matWorld, &m_matView, &m_matProj);
				pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
				pContext->IASetVertexBuffers(0, 1, pSubMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
				pContext->IASetIndexBuffer(pSubMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_pTextureSRV.GetAddressOf());

				pContext->DrawIndexed(pSubMesh->m_AseIndexList.size(), 0, 0);
			}
		}
		else
		{
			pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
			pContext->IASetVertexBuffers(0, 1, pMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
			pContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 1, pMesh->m_pTextureSRV.GetAddressOf());

			pContext->DrawIndexed(pMesh->m_AseIndexList.size(), 0, 0);
		}
	}

	return true;
}






//Ʈ�� ������ ã�´�.
bool		 AAseObj::GetAnimationTrack(float fFrame,vector<AAnimTrack*> pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack)
{

	for (DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++)
	{
		AAnimTrack *pTrack = pTrackList[dwTrack];
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// �������Ӻ��� ū���� ���ٸ�. ���� �ð������ ������ ���� ����Ѵ�.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;

	return true;
}



float  AAseObj::PlayTrackTick(float fFrameTick, float fStartTick, float fEndTick)
{
	return  (fFrameTick - fStartTick) / (fEndTick - fStartTick);
}


AMesh* AAseObj::SearchToCollects(T_STR	m_strParentName)
{
	for (DWORD dwGroup = 0; dwGroup < m_GeomObjList.size(); dwGroup++)
	{
		if (m_MeshList[dwGroup]->m_strNodeName == m_strParentName)
		{
			return m_MeshList[dwGroup];
		}
	}
	return NULL;
}



int	AAseObj::GetMapID(AMtrl* pMtrl, int iTexMapType)
{
	_ASSERT(pMtrl);
	if (pMtrl->m_TexMaps.size() <= 0) return -1;

	for (int iTexmap = 0; iTexmap < pMtrl->m_TexMaps.size(); iTexmap++)
	{
		if (pMtrl->m_TexMaps[iTexmap].m_dwType == iTexMapType)
		{
			return pMtrl->m_TexMaps[iTexmap].m_dwIndex;
		}
	}
	return -1;
}

AAnimTrack* AAseObj::SetLinkedTrack(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev)
{
	if (pPrev)
	{
		pPrev->pNext = pCurrentTrack;
		pCurrentTrack->pPrev = pPrev;
	}
	return pCurrentTrack;
}


AAseObj::AAseObj()
{
	m_fTickSpeed = 1;
}


AAseObj::~AAseObj()
{



}




bool AAnimation::GetAnimationTrack(float fFrame, vector<AAnimTrack*>& pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack)
{
	for (DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++)
	{
		AAnimTrack *pTrack = pTrackList[dwTrack];
		_ASSERT(pTrack);
		// fFrame ���� ū Tick Ʈ���� �ִٸ� ���� Ʈ���� �Ѱ� �־�� �ϱ� ������ break�Ѵ�.
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// �������Ӻ��� ū���� ���ٸ�. ���� �ð������ ������ ���� ����Ѵ�.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;
}

D3DXMATRIX AAnimation::Interpolate(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick, AScene tScene)
{

	// TM		= AnimMat * ParentTM;
	// AaniMat	= TM * Inverse(ParentTM)
	D3DXQUATERNION qR, qS;
	D3DXMATRIX matAnim, matPos, matRotate, matScale, matCalculation;

	D3DXMatrixIdentity(&matCalculation);

	matRotate = pMesh->m_matWorldRotate;
	matPos = pMesh->m_matWorldTrans;
	matScale = pMesh->m_matWorldScale;

	D3DXQuaternionRotationMatrix(&qR, &matRotate);
	D3DXQuaternionRotationMatrix(&qS, &matScale);

	// fFrameTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame + CurFame;
	float fStartTick = tScene.iFirstFrame * tScene.iTickPerFrame;
	float fEndTick = 0.0f;

	AAnimTrack* pStartTrack = NULL;
	AAnimTrack* pEndTrack = NULL;
	if (pMesh->m_RotTrack.size())
	{
		// pStartTrack�� ã���� ������
		if (GetAnimationTrack(fFrameTick, pMesh->m_RotTrack, &pStartTrack, &pEndTrack))
		{
			qR = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXQuaternionSlerp(&qR, &qR, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qR);
	}

	pStartTrack = NULL;
	pEndTrack = NULL;

	D3DXVECTOR3 Trans(matPos._41, matPos._42, matPos._43);
	if (pMesh->m_PosTrack.size())
	{
		// pStartTrack�� ã���� ������
		if (GetAnimationTrack(fFrameTick, pMesh->m_PosTrack, &pStartTrack, &pEndTrack))
		{
			Trans = pStartTrack->vVector;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp(&Trans, &Trans, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}

		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);
	}


	pStartTrack = NULL;
	pEndTrack = NULL;

	D3DXMATRIX matScaleRot, matInvScaleRot;
	D3DXVECTOR3 vScale(matScale._11, matScale._22, matScale._33);
	if (pMesh->m_SclTrack.size())
	{
		// pStartTrack�� ã���� ������
		if (GetAnimationTrack(fFrameTick, pMesh->m_SclTrack, &pStartTrack, &pEndTrack))
		{
			vScale = pStartTrack->vVector;
			qS = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp(&vScale, &vScale, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);
		matScale = matInvScaleRot * matScale * matScaleRot;
	}

	pStartTrack = NULL;
	pEndTrack = NULL;

	float fCurAlpha, fNextAlpha, fOffSet;
	fCurAlpha = 0.0f;
	fNextAlpha = 0.0f;
	//if (pMesh->m_VisTrack.size())
	//{
	//	// pStartTrack�� ã���� ������
	//	if (GetAnimationTrack(fFrameTick, pMesh->m_VisTrack, &pStartTrack, &pEndTrack))
	//	{
	//		fCurAlpha = pStartTrack->vVector.x;
	//		fStartTick = pStartTrack->iTick;
	//	}
	//	if (pEndTrack)
	//	{
	//		fNextAlpha = pEndTrack->vVector.x;
	//		fEndTick = pEndTrack->iTick;

	//		fOffSet = (fFrameTick - fStartTick) / (fEndTick - fStartTick);
	//		fNextAlpha = (fNextAlpha - fCurAlpha)*fOffSet;
	//	}
	//	pMesh->m_VisTrack = (fCurAlpha + fNextAlpha);
	//}
	//else
	//{
	//	pMesh->m_fVisibility = 1.0f;
	//}

	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;
	// ���� ���̸��̼� ����� �ϼ��Ѵ�.	
	D3DXMatrixMultiply(&matCalculation, &matAnim, matParents);

	// �ι��� ��Ʈ���� Ȯ�� �ڵ�.
	D3DXVECTOR3 v0, v1, v2, v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		D3DXMATRIX matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&matCalculation, &pMesh->m_matCalculation, &matW);
	}
	return matCalculation;
}

D3DXMATRIX AAnimation::Update(float fElapseTime, AMesh* pMesh, D3DXMATRIX &matParent, AScene &tScene)
{
	D3DXMATRIX mat, matReturn;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixIdentity(&matReturn);

	if (pMesh->m_bUnAnimObj != false)
	{
		if (pMesh->m_pParent)
			return Interpolate(pMesh, &matParent, fElapseTime, tScene);
		else
			return Interpolate(pMesh, &mat, fElapseTime, tScene);
	}

	return mat;
}

AAnimation::AAnimation(void)
{
}


AAnimation::~AAnimation(void)
{
}

