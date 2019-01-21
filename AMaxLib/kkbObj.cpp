#include "kkbObj.h"

bool kkbObj::Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread)
{
	m_dxObj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxObj.m_iIndexSize = sizeof(DWORD);


	m_Parser.Load(szLoadName);

	if (LoadScene(szLoadName))
	{
		if (!LoadMaterial()) return false;
		if (!LoadObject()) return false;


		if (!Convert(pd3dDevice))
		{
			return false;
		}

		m_dxObj.m_iNumVertex = m_iMaxVertex;
		m_dxObj.m_iNumIndex = m_iMaxIndex;

		if (Set(pd3dDevice, pLoadShaderFile))
		{
		//	m_Parser.CloseStream();
		//	return false;
		}

		m_Parser.Release();// .CloseStream();

		return true;
	}
	return false;
}





bool kkbObj::LoadScene(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(strFileName, Drive, Dir, FName, Ext);
	m_szDirName = Drive;
	m_szDirName += Dir;
	m_szName = FName;
	m_szName += Ext;

	m_Parser.GetSearchString(_T("SCENE"),true);
	m_Parser.GetNextTokenString();
	_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d%d%d%d%d%d%d%d"),
		&m_Scene.iVersion,
		&m_Scene.iFirstFrame,
		&m_Scene.iLastFrame,
		&m_Scene.iFrameSpeed,
		&m_Scene.iTickPerFrame,
		&m_Scene.iNumMesh,
		&m_Scene.iMaxWeight,
		&m_Scene.iBindPose);

	return true;
}

bool		kkbObj::LoadTexture(ID3D11Device* device, const TCHAR* fileName)
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

bool kkbObj::LoadMaterial()
{
	int iNumRootMtl = 0;
	// Material		
	TCHAR temp[256] = _T("");
	TCHAR temp2[256] = _T("");

	if (m_Parser.GetSearchString(_T("ROOT_MATERIAL")))
	{
		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%s%d"), &temp, &iNumRootMtl);
	}

	m_Material.resize(iNumRootMtl);

	for (DWORD dwMtl = 0; dwMtl < iNumRootMtl; dwMtl++)
	{
		if (m_Parser.GetSearchString(_T("MATERIAL")))
		{
			//_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%s%d"), &temp, &iNumRootMtl);
		}

		LoadMaterialHeader(&m_Material[dwMtl]);

		if (m_Material[dwMtl].m_dwSubCount > 0)
		{
			m_Material[dwMtl].m_SubMtrl.resize(m_Material[dwMtl].m_dwSubCount);
			for (DWORD dwSub = 0; dwSub < m_Material[dwMtl].m_dwSubCount; dwSub++)
			{
				// SubMaterial Index
				if (m_Parser.GetSearchString(_T("SUBMATERIAL")))
				{	//	if (!m_Parser.GetDataFromFileLoop(_T("#SUBMATERIAL"), &m_Material[dwMtl].m_SubMaterial[dwSub].m_dwIndex, INT_DATA))
					_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%s %d"), &temp, &m_Material[dwMtl].m_SubMtrl[dwSub].m_dwIndex);//m_Material[dwMtl].m_SubMtrl[dwSub].m_dwIndex);
				}
				LoadMaterialHeader(&m_Material[dwMtl].m_SubMtrl[dwSub]);
				LoadTexMap(&m_Material[dwMtl].m_SubMtrl[dwSub], m_szDirName);
			}
		}
		else
		{
			LoadTexMap(&m_Material[dwMtl], m_szDirName);
		}
	}

	return true;
}

void kkbObj::LoadMaterialHeader(AMtrl* pMtrl)
{
	TCHAR strName[256] = _T("");
	TCHAR strClassName[256] = _T("");
	m_Parser.GetNextTokenString();
	_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%s %s %d %d"),
		&strName,
		&strClassName,
		&pMtrl->m_dwSubCount,
		&pMtrl->m_dwTexMapCount);

	pMtrl->m_strName = strName;
	pMtrl->m_strClassName = strClassName;

}
void kkbObj::LoadTexMap(AMtrl* pMtrl, T_STR szDirName)
{
	TCHAR strName[256] = _T("");
	for (DWORD dwTex = 0; dwTex < pMtrl->m_dwTexMapCount; dwTex++)
	{
		m_Parser.GetNextTokenString();
		ATextexMap TexMap;

		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d%s"), &TexMap.m_dwType, &strName );


		TexMap.m_strTextureName = strName;
		// 텍스쳐 매니져 등록 및 배열 저장.
		TexMap.m_dwIndex = I_TEXTURE.Add(g_pd3dDevice.Get(), TexMap.m_strTextureName.c_str(), szDirName.c_str());

		pMtrl->m_TexMaps.push_back(TexMap);
	}
}
bool kkbObj::LoadObject()
{
	// Object	
	DWORD dwCount;
	TCHAR temp[256] = _T(""); 
	TCHAR strName[256] = _T("");
	TCHAR strParentName[256] = _T("");

	for (DWORD dwMesh = 0; dwMesh < m_Scene.iNumMesh; dwMesh++)
	{
		AMesh* pMesh = new AMesh();
		pMesh->m_iIndex = dwMesh;
		if (m_Parser.GetSearchString(_T("OBJECTS")))
		{
			_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%s%d"), &temp, &dwCount);
		}
	
		
		m_Parser.GetNextTokenString();
		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T(" %s %s"), strName, strParentName);

		pMesh->m_strNodeName = strName;
		pMesh->m_strParentName = strParentName;

		m_Parser.GetNextTokenString();
		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d%d%d%d%d%d %d"),
			&pMesh->m_ClassType,
			&pMesh->m_iNumFace,
			&pMesh->m_iNumTrack[0],
			&pMesh->m_iNumTrack[1],
			&pMesh->m_iNumTrack[2],
			&pMesh->m_iNumTrack[3],
			&pMesh->m_iMtrlRef);

		m_Parser.GetNextTokenString();

		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%f%f%f%f%f%f"),
			&pMesh->m_Box.vMax.x, 
			&pMesh->m_Box.vMax.y, &pMesh->m_Box.vMax.z,
			&pMesh->m_Box.vMin.x, &pMesh->m_Box.vMin.y, &pMesh->m_Box.vMin.z);

		if (m_Parser.GetSearchString(_T("WORLD_MATRIX")))
		{
			m_Parser.GetNextTokenString();
			_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%f%f%f%f"), &pMesh->m_matWorld._11, &pMesh->m_matWorld._12, &pMesh->m_matWorld._13, &pMesh->m_matWorld._14);
			m_Parser.GetNextTokenString();
			_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%f%f%f%f"), &pMesh->m_matWorld._21, &pMesh->m_matWorld._22, &pMesh->m_matWorld._23, &pMesh->m_matWorld._24);
			m_Parser.GetNextTokenString();
			_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%f%f%f%f"), &pMesh->m_matWorld._31, &pMesh->m_matWorld._32, &pMesh->m_matWorld._33, &pMesh->m_matWorld._34);
			m_Parser.GetNextTokenString();
			_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%f%f%f%f"), &pMesh->m_matWorld._41, &pMesh->m_matWorld._42, &pMesh->m_matWorld._43, &pMesh->m_matWorld._44);
		}

		if (!m_Parser.GetSearchString(_T("TRAINGLES")))
		{
			
		}

		D3DXMatrixInverse(&pMesh->m_matInverse, NULL, &pMesh->m_matWorld);
		// 서브메쉬에 페이스가 존재하면
		if (pMesh->m_iMtrlRef >= 0 && m_Material[pMesh->m_iMtrlRef].m_dwSubCount > 0)
		{
			for (int iMtrl = 0; iMtrl < m_Material[pMesh->m_iMtrlRef].m_dwSubCount; iMtrl++)
			{
				AMesh* pSubMesh = new AMesh();

				pSubMesh->m_iNumFace = LoadVertexIndex(pSubMesh);
				pSubMesh->m_iMtrlRef = iMtrl;

				if (pSubMesh->m_iNumFace <= 0)
				{
					continue;
				}
				// 실제데이터가 들어있는 서브 오브젝트 갯수( 사용처가 없는 서브매터리얼은 제외된다. )				
				pMesh->m_pSubMesh.push_back(pSubMesh);
			}
		}
		else
		{
			// 정점 및 인덱스 데이터 로드
			LoadVertexIndex(pMesh);
		}

		// 에니메이션 트랙 로드
		LoadAnimation(pMesh);

		m_MeshList.push_back(pMesh);
	}
	return true;
}

void kkbObj::LoadAnimation(AMesh* pMesh)
{
	if (pMesh->m_iNumTrack[0] && SUCCEEDED(m_Parser.GetSearchString(_T("POSITION_TRACK"))))
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[0], pMesh->m_PosTrack);
	}
	if (pMesh->m_iNumTrack[1] && m_Parser.GetSearchString(_T("ROTATION_TRACK")))
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[1], pMesh->m_RotTrack);
	}
	if (pMesh->m_iNumTrack[2] && m_Parser.GetSearchString(_T("SCALE_TRACK")))
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[2], pMesh->m_SclTrack);
	}
	if (pMesh->m_iNumTrack[3] && m_Parser.GetSearchString(_T("VISIVILITY_TRACK")))
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[3], pMesh->m_VisTrack);
	}
}

int kkbObj::LoadVertexIndex(AMesh* pData)
{
	int iMtrlIndex = 0;
	int iNumSubFaces = 0;
	int iVertexCounter = 0;

	m_Parser.GetNextTokenString();
	_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d %d %d"), &iMtrlIndex, &iNumSubFaces, &iVertexCounter);
	if (iVertexCounter <= 0) return 0;

	if (iNumSubFaces > 0)
	{
		pData->m_TriList.resize(iNumSubFaces);
	}
	else
	{
		pData->m_TriList.resize(pData->m_iNumFace);
	}
	pData->m_AseIndexList.resize(iNumSubFaces * 3);
	pData->m_AseVertexList.resize(iVertexCounter);

	for (int iCnt = 0; iCnt < iVertexCounter; iCnt++)
	{
		m_Parser.GetNextTokenString();

		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%f %f %f  %f %f %f  %f %f %f %f  %f %f"),
			&pData->m_AseVertexList[iCnt].p.x, &pData->m_AseVertexList[iCnt].p.y, &pData->m_AseVertexList[iCnt].p.z,
			&pData->m_AseVertexList[iCnt].n.x, &pData->m_AseVertexList[iCnt].n.y, &pData->m_AseVertexList[iCnt].n.z,
			&pData->m_AseVertexList[iCnt].c.x, &pData->m_AseVertexList[iCnt].c.y, &pData->m_AseVertexList[iCnt].c.z, &pData->m_AseVertexList[iCnt].c.w,
			&pData->m_AseVertexList[iCnt].t.x, &pData->m_AseVertexList[iCnt].t.y);
	}

	for (int iIndex = 0; iIndex < iNumSubFaces; iIndex++)
	{
		m_Parser.GetNextTokenString();
		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d %d %d"), &pData->m_AseIndexList[iIndex * 3 + 0],
			&pData->m_AseIndexList[iIndex * 3 + 1],
			&pData->m_AseIndexList[iIndex * 3 + 2]);
	}

	m_iMaxVertex += pData->m_AseVertexList.size();
	m_iMaxIndex += pData->m_AseIndexList.size();

	// 트라이앵글 리스트 생성
	for (DWORD dwFace = 0; dwFace < iNumSubFaces; dwFace++)
	{
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			pData->m_TriList[dwFace].vVertex[iCnt] = pData->m_AseVertexList[pData->m_AseIndexList[dwFace * 3 + iCnt]];
		}
	}
	return iNumSubFaces;
}
bool kkbObj::LoadAnimationTrack(int iNumTrack, vector<AAnimTrack*>& pTrackList)
{
	AAnimTrack* pPrevTrack = NULL;
	for (int iTrack = 0; iTrack < iNumTrack; iTrack++)
	{
		AAnimTrack* pTrack = new AAnimTrack();
		m_Parser.GetNextTokenString();

		//_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d %d %d"), &iMtrlIndex, &iNumSubFaces, &iVertexCounter);

		_stscanf(m_Parser.m_pwcTokenData.c_str(), _T("%d%f%f%f%f%f%f%f"),
			&pTrack->iTick,
			&pTrack->vVector.x,
			&pTrack->vVector.y,
			&pTrack->vVector.z,
			&pTrack->qRotate.x,
			&pTrack->qRotate.y,
			&pTrack->qRotate.z,
			&pTrack->qRotate.w);

		pPrevTrack = SetDoublyLinkedList(pTrack, pPrevTrack);
		pTrackList.push_back(pTrack);
	}
	return true;
}

AAnimTrack* kkbObj::SetDoublyLinkedList(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev)
{
	if (pPrev)
	{
		pPrev->pNext = pCurrentTrack;
		pCurrentTrack->pPrev = pPrev;
	}
	return pCurrentTrack;
}


float  kkbObj::PlayTrackTick(float fFrameTick, float fStartTick, float fEndTick)
{
	return  (fFrameTick - fStartTick) / (fEndTick - fStartTick);
}



D3DXMATRIX kkbObj::AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick)
{

	D3DXQUATERNION qR, qS;	//사원수 회전, 사원수 스케일
	D3DXMatrixIdentity(&pMesh->m_matCalculation);		//최종은 칼코리행렬에 저장된다.
	D3DXMATRIX matAnim, matPos, matRotate, matScale;	//애님행렬, T, R ,S

	matScale = pMesh->m_matWorldScale;					//사원수 보간해야함
	matRotate = pMesh->m_matWorldRotate;				//사원수 보간해야함
	matPos = pMesh->m_matWorldTrans;					//벡터보간

	D3DXQuaternionRotationMatrix(&qR, &matRotate);		//행렬로분터 사원수 생성
	D3DXQuaternionRotationMatrix(&qS, &matScale);		//행렬로분터 사원수 생성

	float fStartTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	float fEndTick = 0.0f;

	//구간 마나다 NULL을 안해주면 치명적인 현상 발생?
	//순서 R P S 순으로 안하면 애니메이션이 꺠진다.
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
			//사원수 선형 보간
			D3DXQuaternionSlerp(&qR,
				&qR,
				&pEndTrack->qRotate,
				(fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qR);
	}


	pStartTrack = NULL;
	pEndTrack = NULL;
	D3DXVECTOR3 Trans(matPos._41, matPos._42, matPos._43);
	if (pMesh->m_PosTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_PosTrack, &pStartTrack, &pEndTrack))
		{
			Trans = pStartTrack->vVector;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			//벡터 선형 보간
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
		if (GetAnimationTrack(fFrameTick, pMesh->m_SclTrack, &pStartTrack, &pEndTrack))
		{
			vScale = pStartTrack->vVector;
			qS = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			//벡터보간
			D3DXVec3Lerp(&vScale,
				&vScale,
				&pEndTrack->vVector,
				PlayTrackTick(fFrameTick, fStartTick, fEndTick));


			//사원수보간
			D3DXQuaternionSlerp(&qS,
				&qS,
				&pEndTrack->qRotate,
				PlayTrackTick(fFrameTick, fStartTick, fEndTick));

		}

		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);	//스케일 행렬을 만듬
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);			//사원수회전행렬을 만든다.
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);		//스케일회전의 역행렬을 만듬 
		matScale = matInvScaleRot * matScale * matScaleRot;			//최종 스케일 행렬 || (로컬 이나 자식으로) 돌아가서 스케일 곱셈처리를 한다.
	}

	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;
	// 최종 에미메이션 행렬을 완성한다.	
	D3DXMatrixMultiply(&pMesh->m_matCalculation, &matAnim, matParents);

	// 인버스 매트릭스 확인 코드.
	D3DXVECTOR3 v0, v1, v2, v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		D3DXMATRIX matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&pMesh->m_matCalculation, &pMesh->m_matCalculation, &matW);
	}
	return pMesh->m_matCalculation;
}

//// pEndTrack 트랙이 없으면 flase 리턴( 보간할 대상이 없을 때 )
bool kkbObj::GetAnimationTrack(float fFrame, vector<AAnimTrack*> pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack)
{
	for (DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++)
	{
		AAnimTrack *pTrack = pTrackList[dwTrack];
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// 현프레임보다 큰값이 없다면. 가장 시간경과가 오래된 것을 사용한다.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;
}


bool kkbObj::Frame()
{
	m_fElapseTime += g_fSPF * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame /* * m_fTickSpeed*/;

	if (m_fElapseTime >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)	//라스트 프레임이면
	{
		m_fElapseTime = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;	//처음프레임으로간다
	}


	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);	//부모 없을 경우에 대한 단위 행렬


	for (DWORD A = 0; A < m_MeshList.size(); A++)
	{
		AMesh* pMesh = m_MeshList[A];
		if (pMesh->m_bUnAnimObj == true)
		{
			continue;
		}

		if (pMesh->m_pParent == NULL)
		{
			AnimPlay(pMesh, &matIdent, m_fElapseTime);
		}
		else
		{
			AnimPlay(pMesh, &pMesh->m_pParent->m_matCalculation, m_fElapseTime);
		}

		/*for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
		{
			AMesh* pMesh = m_pMesh[dwObject].get();
			pMesh->m_matCalculation = pMesh->m_matCalculation * m_matWorld;
		}*/
	}
	return true;
}

bool kkbObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
//bool kkbObj::Release()
//{
//	return true;
//}

bool kkbObj::Convert(ID3D11Device* pd3dDevice)
{
	if (!SetMaterial())
	{
		return false;
	}
	if (!InheriteCollect())
	{
		return false;
	}
	return true;
}




int	kkbObj::GetMapID(AMtrl* pMtrl, int iTexMapType)
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


bool kkbObj::InheriteCollect()
{
	D3DXMATRIX m_matInverse;
	D3DXQUATERNION qR;
	D3DXVECTOR3 vTrans, vScale;
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		if (!m_MeshList[dwObject]->m_strParentName.empty()
			&& (m_MeshList[dwObject]->m_strParentName != _T("(null)")))
		{
			AMesh* pParenANode = SearchToCollects(m_MeshList[dwObject]->m_strParentName);
			if (pParenANode)
			{
				m_MeshList[dwObject]->m_pParent = pParenANode;
				D3DXMATRIX matInverse = m_MeshList[dwObject]->m_matWorld * pParenANode->m_matInverse;
				D3DXMatrixDecompose(&vScale, &qR, &vTrans, &matInverse);
				D3DXMatrixScaling(&m_MeshList[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
				D3DXMatrixTranslation(&m_MeshList[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
				D3DXMatrixRotationQuaternion(&m_MeshList[dwObject]->m_matWorldRotate, &qR);

				pParenANode->m_pChildMesh.push_back(m_MeshList[dwObject]);
			}
		}
		else
		{
			D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_MeshList[dwObject]->m_matWorld);
			D3DXMatrixScaling(&m_MeshList[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
			D3DXMatrixTranslation(&m_MeshList[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
			D3DXMatrixRotationQuaternion(&m_MeshList[dwObject]->m_matWorldRotate, &qR);
		}
	}
	// 사용되지 않은 오브젝트 삭제
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		if (m_MeshList[dwObject]->m_pChildMesh.size() <= 0 &&
			(m_MeshList[dwObject]->m_ClassType == CLASS_BONE || m_MeshList[dwObject]->m_ClassType == CLASS_DUMMY))
		{
			m_MeshList[dwObject]->m_bUnAnimObj = true;
		}
	}
	return true;
}
AMesh* kkbObj::SearchToCollects(T_STR	m_strParentName)
{
	for (DWORD dwGroup = 0; dwGroup < m_MeshList.size(); dwGroup++)
	{
		if (m_MeshList[dwGroup]->m_strNodeName == m_strParentName)
		{
			return m_MeshList[dwGroup];
		}
	}
	return NULL;
}
bool kkbObj::SetMaterial()
{
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		if (pMesh->m_iNumFace <= 0 || pMesh->m_iMtrlRef < 0) continue;

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < pMesh->m_pSubMesh.size(); iSubMesh++)
			{
				AMesh* pSubMesh = pMesh->m_pSubMesh[iSubMesh];
				if (pSubMesh)
				{
					pSubMesh->m_iDiffuseTex = GetMapID(&m_Material[pMesh->m_iMtrlRef].m_SubMtrl[pSubMesh->m_iMtrlRef], ID_BASIS_DI);
				//	pSubMesh->m_iDiffuseTex = I_TEXTURE.GetPtr(pMesh->m_pSubMesh[iSubMesh]->m_iDiffuseTex)->m_TextureSRV;
				}
			}
		}
		else
		{
			pMesh->m_iDiffuseTex = GetMapID(&m_Material[pMesh->m_iMtrlRef], ID_BASIS_DI); //I_TEXTURE.GetPtr(pMesh->m_iDiffuseTex)->m_TextureSRV; //GetMapID(&m_Material[pMesh->m_iMtrlRef], ID_BASIS_DI);
		}
	}
	return true;
}
//bool kkbObj::UpdateBuffer()
//{
//	if (!CombineBuffer(m_dxObj.g_pVertexBuffer.Get(), m_dxObj.g_pIndexBuffer.Get()))
//	{
//		return false;
//	}
//	return true;
//}
//bool kkbObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
//{
//	size_t dstOffset = 0;
//	size_t vbOffset = 0;
//	size_t dstibOffset = 0;
//	size_t ibOffset = 0;
//	{
//		for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
//		{
//			auto pData = m_pData[dwObject].get();
//			auto ptMesh = m_pMesh[dwObject].get();
//			if (ptMesh->m_pSubMesh.size() > 0)
//			{
//				for (DWORD dwSub = 0; dwSub < ptMesh->m_pSubMesh.size(); dwSub++)
//				{
//					auto pSubData = pData->m_pSubMesh[dwSub].get();
//					auto ptSubMesh = ptMesh->m_pSubMesh[dwSub].get();
//					if (pSubData->m_VertexArray.size() < 3) continue;
//
//					ptSubMesh->m_dxObj.m_iNumVertex = pSubData->m_VertexArray.size();
//					ptSubMesh->m_dxObj.m_iVertexSize = m_dxObj.m_iVertexSize;
//
//					ptSubMesh->m_dxObj.m_BoxVB.left = dstOffset;
//					ptSubMesh->m_dxObj.m_BoxVB.right = dstOffset + ptSubMesh->m_dxObj.m_iNumVertex*m_dxObj.m_iVertexSize;
//					ptSubMesh->m_dxObj.m_BoxVB.top = 0; ptSubMesh->m_dxObj.m_BoxVB.bottom = 1;
//					ptSubMesh->m_dxObj.m_BoxVB.front = 0; ptSubMesh->m_dxObj.m_BoxVB.back = 1;
//
//					g_pImmediateContext->UpdateSubresource(
//						pVB, 0,
//						&ptSubMesh->m_dxObj.m_BoxVB,
//						(uint8_t*)&pSubData->m_VertexArray.at(0),
//						0, 0);
//
//
//					/*g_pImmediateContext->CopySubresourceRegion(
//					m_dxObj.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
//					(void*)&pSubMesh->m_VertexArray.at(0),
//					0, &pSubMesh->m_dxObj.m_BoxVB);*/
//
//					ptSubMesh->m_dxObj.m_iBeginVB = vbOffset;
//					vbOffset += ptSubMesh->m_dxObj.m_iNumVertex;
//					dstOffset = ptSubMesh->m_dxObj.m_BoxVB.right;
//
//					ptSubMesh->m_dxObj.m_iNumIndex = pSubData->m_IndexArray.size();
//					ptSubMesh->m_dxObj.m_BoxIB.left = dstibOffset;
//					ptSubMesh->m_dxObj.m_BoxIB.right = dstibOffset + ptSubMesh->m_dxObj.m_iNumIndex * sizeof(DWORD);
//					ptSubMesh->m_dxObj.m_BoxIB.top = 0;	ptSubMesh->m_dxObj.m_BoxIB.bottom = 1;
//					ptSubMesh->m_dxObj.m_BoxIB.front = 0;	ptSubMesh->m_dxObj.m_BoxIB.back = 1;
//
//					g_pImmediateContext->UpdateSubresource(pIB, 0,
//						&ptSubMesh->m_dxObj.m_BoxIB, (void*)&pSubData->m_IndexArray.at(0), 0, 0);
//
//					ptSubMesh->m_dxObj.m_iBeginIB = ibOffset;
//					ibOffset += ptSubMesh->m_dxObj.m_iNumIndex;
//					dstibOffset = ptSubMesh->m_dxObj.m_BoxIB.right;
//
//					//texture
//					if (ptSubMesh->m_iDiffuseTex < 0) continue;
//					ptSubMesh->m_dxObj.g_pTextureSRV = I_Texture.GetPtr(ptSubMesh->m_iDiffuseTex)->m_pTextureRV;
//				}
//			}
//			else
//			{
//				if (pData->m_VertexArray.size() < 3) continue;
//				ptMesh->m_dxObj.m_iNumVertex = pData->m_VertexArray.size();
//				ptMesh->m_dxObj.m_iVertexSize = m_dxObj.m_iVertexSize;
//
//				ptMesh->m_dxObj.m_BoxVB.left = dstOffset;
//				ptMesh->m_dxObj.m_BoxVB.right = dstOffset + ptMesh->m_dxObj.m_iNumVertex*m_dxObj.m_iVertexSize;
//				ptMesh->m_dxObj.m_BoxVB.top = 0; ptMesh->m_dxObj.m_BoxVB.bottom = 1;
//				ptMesh->m_dxObj.m_BoxVB.front = 0; ptMesh->m_dxObj.m_BoxVB.back = 1;
//
//				g_pImmediateContext->UpdateSubresource(pVB, 0, &ptMesh->m_dxObj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);
//
//				ptMesh->m_dxObj.m_iBeginVB = vbOffset;
//				vbOffset += ptMesh->m_dxObj.m_iNumVertex;
//				dstOffset = ptMesh->m_dxObj.m_BoxVB.right;
//
//				ptMesh->m_dxObj.m_iNumIndex = pData->m_IndexArray.size();
//				ptMesh->m_dxObj.m_BoxIB.left = dstibOffset;
//				ptMesh->m_dxObj.m_BoxIB.right = dstibOffset + ptMesh->m_dxObj.m_iNumIndex * sizeof(DWORD);
//				ptMesh->m_dxObj.m_BoxIB.top = 0; ptMesh->m_dxObj.m_BoxIB.bottom = 1;
//				ptMesh->m_dxObj.m_BoxIB.front = 0; ptMesh->m_dxObj.m_BoxIB.back = 1;
//
//				g_pImmediateContext->UpdateSubresource(pIB, 0, &ptMesh->m_dxObj.m_BoxIB, (void*)&pData->m_IndexArray.at(0), 0, 0);
//				ptMesh->m_dxObj.m_iBeginIB = ibOffset;
//				ibOffset += ptMesh->m_dxObj.m_iNumIndex;
//				dstibOffset = ptMesh->m_dxObj.m_BoxIB.right;
//
//				//texture
//				if (ptMesh->m_iDiffuseTex < 0) continue;
//				ptMesh->m_dxObj.g_pTextureSRV = I_Texture.GetPtr(ptMesh->m_iDiffuseTex)->m_pTextureRV;
//			}
//		}
//	}
//	return true;
//}
//
//

bool kkbObj::Draw(ID3D11DeviceContext*    pContext, AModel* pParent)
{
	for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	{
		AMesh* pMesh = m_MeshList[dwObject];
		UINT stride = sizeof(PNCT_VERTEX);
		UINT offset = 0;
		D3DXMATRIX matWorld = pMesh->m_matCalculation * pParent->m_matWorld;
		D3DXMatrixTranspose(&cbData.matWorld, &matWorld);

		//pMesh->m_matCalculation = pMesh->m_matCalculation;// *m_matControlWorld;		//추후에 컨트롤 행렬을 곱할 것이다.
		this->SetMatrix(&pMesh->m_matCalculation, &this->m_matView, &this->m_matProj);

		pParent->SetMatrix(&pMesh->m_matCalculation, &pParent->m_matView, &pParent->m_matProj);

		UpdateConstantBuffer(pContext, pParent);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				AMesh* pSubMesh = pMesh->m_pSubMesh[dwSub];

				SetMatrix(&pSubMesh->m_matWorld, &m_matView, &m_matProj);
				//pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
				pContext->IASetVertexBuffers(0, 1, pSubMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
				pContext->IASetIndexBuffer(pSubMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_pTextureSRV.GetAddressOf());

				pContext->DrawIndexed(pSubMesh->m_AseIndexList.size(), 0, 0);
			}
		}
		else
		{
	//		pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
			pContext->IASetVertexBuffers(0, 1, pMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
			pContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 1, pMesh->m_pTextureSRV.GetAddressOf());

			pContext->DrawIndexed(pMesh->m_AseIndexList.size(), 0, 0);
		}
	}


	//for (DWORD dwObject = 0; dwObject < m_MeshList.size(); dwObject++)
	//{
	//	auto pMesh = m_MeshList[dwObject];
	//	D3DXMATRIX matWorld = pMesh->m_matCalculation * pParent->m_matWorld;
	//	D3DXMatrixTranspose(&cbData.matWorld, &matWorld);
	//	UpdateConstantBuffer(pContext, pParent);
	//	if (pMesh->m_pSubMesh.size() > 0)
	//	{
	//		for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
	//		{
	//			auto pSubMesh = pMesh->m_pSubMesh[dwSub];
	//			if (pSubMesh->m_iNumFace < 1) continue;
	//			pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
	//			pContext->DrawIndexed(pSubMesh->m_dxObj.m_iNumIndex,
	//				pSubMesh->m_dxObj.m_iBeginIB,
	//				pSubMesh->m_dxObj.m_iBeginVB);
	//		}
	//	}
	//	else
	//	{
	//		if (pMesh->m_iNumFace < 1) continue;
	//		pContext->PSSetShaderResources(0, 1, pMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
	//		pContext->DrawIndexed(pMesh->m_dxObj.m_iNumIndex,
	//			pMesh->m_dxObj.m_iBeginIB,
	//			pMesh->m_dxObj.m_iBeginVB);
	//	}
	//}
	return true;
}

bool		kkbObj::CreateVertexBuffer()
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


bool		kkbObj::CreateIndexBuffer()
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


kkbObj::kkbObj()
{
}


kkbObj::~kkbObj()
{
}
