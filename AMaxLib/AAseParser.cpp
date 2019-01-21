#include "AAseParser.h"
#include "ATexture.h"


bool			AAseParser::Release() 
{

	//m_ListTokens.clear();
	//SAFE_DELETE_ARRAY(m_pwcStaticMeshData);
	//SAFE_DELETE_ARRAY(m_pmbStaticMeshData);
	//CloseHandle(m_hHandle);
	
	//m_ListTokens.clear();
	//
	//delete[] m_pwcStaticMeshData;
	//m_pwcStaticMeshData = NULL;

	//delete[] m_pmbStaticMeshData;
	//m_pmbStaticMeshData = NULL;

	//CloseHandle(m_hHandle);



	return true; 
};

bool			AAseParser::LoadHelperObject(vector<AGeomMesh*>& GeomList)
{
	INT iObjectType = -1;
	AGeomMesh* pObject = new AGeomMesh;

	GeomList.push_back(pObject);

	TCHAR szNodeName[256] = _T("");
	TCHAR szParentName[256] = _T("");

	GetData(_T("NODE_NAME"), &szNodeName, STRING_DATA);
	pObject->m_strNodeName = szNodeName;


	int iNumTypes = sizeof(AseHelperObjectTokens) / sizeof(AseHelperObjectTokens[0]);
	while ((iObjectType = SearchTokenArray(iNumTypes, AseHelperObjectTokens, m_TokenMaxSize)) >= 0)
	{
		switch (iObjectType)
		{
		case HELPER_NODE_PARENT:
		{
			GetData(&szParentName, STRING_DATA);
			pObject->m_strParentName = szParentName;
		}break;
		case HELPER_CLASS:
		{
			TCHAR szHelperClassName[256] = _T("");
			GetData(&szHelperClassName, STRING_DATA);
			if (_tcsicmp(szHelperClassName, _T("Bone")) == 0)
			{
				pObject->m_ClassType = CLASS_BONE;
			}
			else if (_tcsicmp(szHelperClassName, _T("Dummy")) == 0)
			{
				pObject->m_ClassType = CLASS_DUMMY;
			}
		}break;
		case HELPER_MODE_TM:
		{
			LoadTM(pObject);
		}break;
		case BOUNDINGBOX_MIN:
		{
			//m_TokenIndex--;
			//GetData(_T("BOUNDINGBOX_MIN"), &pObject->m_BBox.vMin, VECTOR_DATA);
			//GetData(_T("BOUNDINGBOX_MAX"), &pObject->m_BBox.vMax, VECTOR_DATA);
		}break;
		case HELPER_TM_ANIMATION:
		{
			LoadAnimation(pObject);
		}break;
		//case WIREFRAME_COLOR:
		//	{
		//		pObject->m_iMtrlRef = -1;
		//	}break;
		default:
		{
			m_TokenIndex--;
			return true;
		}break;
		}
	}


	return true; 
};



bool AAseParser::Load(const TCHAR* strFileName)
{
	if (strFileName == NULL) return false;
	LoadBuffer(strFileName);
	return true;
}



bool AAseParser::LoadScene(AScene& Scene)
{
	GetData(_T("SCENE_FIRSTFRAME"), &Scene.iFirstFrame, INT_DATA);
	GetData(_T("SCENE_LASTFRAME"), &Scene.iLastFrame, INT_DATA);
	GetData(_T("SCENE_FRAMESPEED"), &Scene.iFrameSpeed, INT_DATA);
	GetData(_T("SCENE_TICKSPERFRAME"), &Scene.iTickPerFrame, INT_DATA);
	return true;
}


bool AAseParser::LoadMaterial(AMtrl*	pMaterial)
{
	if (!pMaterial) return false;
	int iObjectType = -1;
	int iNumTypes = sizeof(AseMaterialTokens) / sizeof(AseMaterialTokens[0]);

	TCHAR szTexName[256] = _T("");
	AMtrl*		pCurrentMtrl = NULL;
	ATextexMap*	pCurrentTexMap = NULL;

	GetData(_T("MATERIAL_NAME"), &szTexName, STRING_DATA);
	pMaterial->m_strName = szTexName;

	GetData(_T("MATERIAL_CLASS"), &szTexName, STRING_DATA);
	pMaterial->m_strClassName = szTexName;

	while ((iObjectType = SearchTokenArray(iNumTypes, AseMaterialTokens)) >= 0)
	{
		switch (iObjectType)
		{
		case NUM_SUBMTRLS:
		{
			int		iSubMaterialCount = 0;
			GetData(&iSubMaterialCount, INT_DATA);

			for (int i = 0; i < iSubMaterialCount; i++)
			{
				AMtrl SubMaterial;
				if (LoadMaterial(&SubMaterial))					
				{
					pMaterial->m_SubMtrl.push_back(SubMaterial);
				}
			}
			return true;
		}break;

		case MAP_NAME:							//*MAP_DIFFUSE  ~ 지오브젝트 전까지
		{
			if (LoadTexture(pMaterial))			//맵 네임부터 ~지오브젝트 전까지[맵 네임 다음순서는 여기서 부터 처리한다.]
			{
				return true;
			}
			return false;
		}break;
		default:
		{
			m_TokenIndex--;
			return true;
		}break;
		}
	}
	return true;
}



//맵 네임 클래스
bool AAseParser::LoadTexture(AMtrl*	pMaterial)
{
	int iObjectType = -1;
	int iNumTypes = sizeof(AseTextureTokens) / sizeof(AseTextureTokens[0]);
	ATextexMap* pTexMap = 0;
	TCHAR szTexName[256] = _T("");
	GetData(szTexName, STRING_DATA);

	while ((iObjectType = SearchTokenArray(iNumTypes, AseTextureTokens)) >= 0)
	{
		switch (iObjectType)
		{
		case MAP_SUBNO: // BITMAP은 typedef struct tagBITMAP 정의되어 있음. 
		{
			ATextexMap TexMap;
			GetData(&TexMap.m_dwType, INT_DATA);
			pMaterial->m_TexMaps.push_back(TexMap);
			pTexMap = &pMaterial->m_TexMaps[pMaterial->m_TexMaps.size() - 1];
		}break;

		case BITMAP_FILE: // BITMAP은 typedef struct tagBITMAP 정의되어 있음. 
		{
			GetData(&szTexName, STRING_DATA);
			pTexMap->m_strTextureName = szTexName;
			pTexMap->m_dwIndex = I_TEXTURE.Add(g_pd3dDevice.Get(), szTexName, m_szDirName.c_str());
		}break;

		//추가봄
		//case MAP_CLASS: // BITMAP은 typedef struct tagBITMAP 정의되어 있음. 
		//{
		//	GetData(&szTexName, STRING_DATA);
		//	pTexMap->m_strTextureName = szTexName;
		//	pTexMap->m_dwIndex = I_TEXTURE.Add(
		//		g_pd3dDevice.Get(), szTexName, m_szDirName.c_str());
		//}break;

		default:
		{
			m_TokenIndex--;
			return true;
		}break;
		}
	}
	return false;
}

bool AAseParser::LoadMaterialList(vector<AMtrl>&	MtrlList)
{
	int		iMtrlCnt = 0;
	GetData(_T("MATERIAL_COUNT"), &iMtrlCnt, INT_DATA);

	for (int i = 0; i < iMtrlCnt; i++)
	{
		AMtrl Material;
		if (LoadMaterial(&Material))
		{
			MtrlList.push_back(Material);
		}
	}
	return true;
}


//bool AAseParser::LoadGeomObject()
//{
//	INT iObjectType = -1;
//	auto pAseObject = make_shared<AAseMesh>();
//	m_pAseMesh.push_back(pAseObject);
//
//	AAseMesh*	pObject = pAseObject.get();
//
//	TCHAR szNodeName[256] = _T("");
//	TCHAR szParentName[256] = _T("");
//
//	GetData(_T("NODE_NAME"), &szNodeName, STRING_DATA);
//	pObject->m_strNodeName = szNodeName;
//
//
//	int iNumTypes = sizeof(AseGeomObjectTokens) / sizeof(AseGeomObjectTokens[0]);
//
//	while ((iObjectType = SearchTokenArray(iNumTypes, AseGeomObjectTokens, m_TokenMaxSize)) >= 0)
//	{
//		switch (iObjectType)
//		{
//		case NODE_PARENT:
//		{
//			GetData(&szParentName, STRING_DATA);
//			pObject->m_strParentName = szParentName;
//		}break;
//		case MODE_TM:
//		{
//			LoadTM(pObject);
//		}break;
//		case MESH:
//		{
//			LoadMesh(pObject);
//		}break;
//		case TM_ANIMATION:
//		{
//			//LoadAnimation(pObject);
//		}break;
//		case TM_VISIBILITY:
//		{
//			//LoadVisAnimation(pObject);
//		}break;
//		case MATERIAL_REF://_T("MATERIAL_REF)"
//		{
//			GetData(&pObject->m_iMtrlRef, INT_DATA);
//		}break;
//		default:
//		{
//			m_TokenIndex--;
//			return true;
//		}break;
//		}
//	}
//	return true;
//}


bool AAseParser::LoadGeomObject(vector<AGeomMesh*>& GeomList)
{
	INT iObjectType = -1;
	AGeomMesh* pAseObject = new AGeomMesh;// make_shared<G>();

	GeomList.push_back(pAseObject);

	AGeomMesh*	pObject = pAseObject;

	TCHAR szNodeName[256] = _T("");
	TCHAR szParentName[256] = _T("");

	GetData(_T("NODE_NAME"), &szNodeName, STRING_DATA);
	pObject->m_strNodeName = szNodeName;


	int iNumTypes = sizeof(AseGeomObjectTokens) / sizeof(AseGeomObjectTokens[0]);

	while ((iObjectType = SearchTokenArray(iNumTypes, AseGeomObjectTokens, m_TokenMaxSize)) >= 0)
	{
		switch (iObjectType)
		{
		case NODE_PARENT:
		{
			GetData(&szParentName, STRING_DATA);
			pObject->m_strParentName = szParentName;
		}break;
		case MODE_TM:
		{
			LoadTM(pObject);
		}break;
		case MESH:
		{
			LoadMesh(pObject);
		}break;
		case TM_ANIMATION:
		{
			LoadAnimation(pObject);
		}break;
		//case TM_VISIBILITY:
		//{
		//	//LoadVisAnimation(pObject);
		//}break;
		case MATERIAL_REF://_T("MATERIAL_REF)"
		{
			GetData(&pObject->m_iMtrlRef, INT_DATA);
		}break;
		default:
		{
			m_TokenIndex--;
			return true;
		}break;
		}
	}
	return true;
}


void AAseParser::LoadMesh(AGeomMesh* pObject)
{
	GetData(_T("MESH_NUMVERTEX"), &pObject->m_PosVertexList.dwNumVertex, INT_DATA);
	pObject->m_iNumFace = 0;
	if (pObject->m_PosVertexList.dwNumVertex > 0)
	{
		GetData(_T("MESH_NUMFACES"), &pObject->m_PosVertexList.dwNumFace, INT_DATA);
		pObject->m_iNumFace = pObject->m_PosVertexList.dwNumFace;

		GetVertexListFromString(_T("MESH_VERTEX_LIST"), pObject->m_PosVertexList.dwNumVertex, pObject->m_PosVertexList.pVertexList);
		GetPositionFaceListFromString(_T("MESH_FACE_LIST"), pObject->m_PosVertexList.dwNumFace, pObject->m_PosVertexList);
	}


	GetData(_T("MESH_NUMTVERTEX"), &pObject->m_TexVertexList.dwNumVertex, INT_DATA);
	if (pObject->m_TexVertexList.dwNumVertex > 0)
	{
		GetVertexListFromString(_T("MESH_TVERTLIST"), pObject->m_TexVertexList.dwNumVertex, pObject->m_TexVertexList.pVertexList);
		GetData(_T("MESH_NUMTVFACES"), &pObject->m_TexVertexList.dwNumFace, INT_DATA);
		GetFaceListFromString(_T("MESH_TFACELIST"), pObject->m_TexVertexList.dwNumFace, pObject->m_TexVertexList);
	}


	GetData(_T("MESH_NUMCVERTEX"), &pObject->m_ColVertexList.dwNumVertex, INT_DATA, false);
	if (pObject->m_ColVertexList.dwNumVertex > 0)
	{
		GetVertexListFromString(_T("MESH_CVERTLIST"), pObject->m_ColVertexList.dwNumVertex, pObject->m_ColVertexList.pVertexList);
		GetData(_T("MESH_NUMCVFACES"), &pObject->m_ColVertexList.dwNumFace, INT_DATA);
		GetFaceListFromString(_T("MESH_CFACELIST"), pObject->m_ColVertexList.dwNumFace, pObject->m_ColVertexList);
	}

	if (pObject->m_iNumFace > 0)
	{
		GetNormalListFromString(_T("MESH_NORMALS"), pObject->m_iNumFace, pObject);
	}

}



void AAseParser::LoadTM(AGeomMesh* pObject)
{

	D3DXMatrixIdentity(&pObject->m_matWorld);
	GetData(_T("TM_ROW0"), &pObject->m_matWorld.m[0], VECTOR_DATA);
	GetData(_T("TM_ROW1"), &pObject->m_matWorld.m[2], VECTOR_DATA);
	GetData(_T("TM_ROW2"), &pObject->m_matWorld.m[1], VECTOR_DATA);
	GetData(_T("TM_ROW3"), &pObject->m_matWorld.m[3], VECTOR_DATA);

	//추후 분해 사용?
	//D3DXMATRIX m_matInverse;
	//D3DXQUATERNION qR;
	//D3DXVECTOR3 vTrans, vScale;
	//m_matInverse = m_pMesh[dwObject]->m_matWorld * pParenANode->m_matInverse;
	//D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_matInverse);
	//D3DXMatrixScaling(&m_pMesh[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
	//D3DXMatrixTranslation(&m_pMesh[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
	//D3DXMatrixRotationQuaternion(&m_pMesh[dwObject]->m_matWorldRotate, &qR);



	D3DXVECTOR3 v0, v1, v2, v3;
	//v0 = pObject->m_matWorld.m[0];
	//v1 = pObject->m_matWorld.m[1];
	//v2 = pObject->m_matWorld.m[2];

	//D3DXVec3Cross(&v3, &v1, &v2);
	//if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	//{
	//	D3DXMATRIX matW;
	//	D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
	//	D3DXMatrixMultiply(&pObject->m_matWorld, &pObject->m_matWorld, &matW);
	//}

	D3DXVECTOR3		vVector;
	float			fAngle=0;
	D3DXQUATERNION	qRotation;

	GetData(_T("TM_POS"), &vVector, VECTOR_DATA);
	//오브젝트 월드이동행렬 곱해서 넣어둠
	D3DXMatrixTranslation(&pObject->m_matWorldTrans, vVector.x, vVector.y, vVector.z);

	GetData(_T("TM_ROTAXIS"), &vVector, VECTOR_DATA);
	GetData(_T("TM_ROTANGLE"), &fAngle, FLOAT_DATA);
	
	D3DXQuaternionRotationAxis(&qRotation, &vVector, fAngle);					//사원수 = 임의의 축  * 앵글
	D3DXMatrixRotationQuaternion(&pObject->m_matWorldRotate, &qRotation);		//사원수를 회전 행렬로 변환
	
	D3DXMATRIX matScl;
	GetData(_T("TM_SCALE"), &vVector, VECTOR_DATA);
	D3DXMatrixScaling(&pObject->m_matWorldScale, vVector.x, vVector.y, vVector.z);
	
	D3DXVECTOR3 vAxis;
	GetData(_T("TM_SCALEAXIS"), &vAxis, VECTOR_DATA);		//스케일 축
	GetData(_T("TM_SCALEAXISANG"), &fAngle, FLOAT_DATA);	//스케일 
	

	   
	// 스케일축의 행렬과 그 역행렬을 구한다.
	D3DXMATRIX matRotation, matRotationInv;
	D3DXMatrixRotationAxis(&matRotation, &vAxis, fAngle);		//회전을 만듬
	D3DXMatrixInverse(&matRotationInv, NULL, &matRotation);		//임의의 축 회전을 역행렬 만듬 
	
	 pObject->m_matWorldScale = matRotationInv * pObject->m_matWorldScale  * matRotation;	//임의의 축에 스케일이 들어감
}



//*TM_ANIMATION{
//*NODE_NAME "Box01"
//*CONTROL_ROT_TRACK{
//*CONTROL_ROT_TRACK{
//*CONTROL_SCALE_TRACK{
//*CONTROL_SCALE_TRACK{

void AAseParser::LoadAnimation(AGeomMesh* pObject)
{
	GetSearchString(_T("NODE_NAME"), false);
	if (GetSearchString(_T("CONTROL_POS_TRACK"), false))
	{
		GetTrackListFromString(pObject->m_PosTrack, POS_SAMLE_TRACK);
	}

	if (GetSearchString(_T("CONTROL_ROT_TRACK"), false))
	{
		GetTrackListFromString(pObject->m_RotTrack, ROT_SAMLE_TRACK);
	}

	if (GetSearchString(_T("CONTROL_SCALE_TRACK"), false))
	{
		GetTrackListFromString(pObject->m_SclTrack, SCL_SAMLE_TRACK);
	}

}


//if (GetSearchString(_T("CONTROL_POS_TRACK"), false) || GetSearchString(_T("CONTROL_POS_TCB"), false) || GetSearchString(_T("CONTROL_POS_BEZIER"), false))
//{
//	GetTrackListFromString(pObject->m_PosTrack, POS_SAMLE_TRACK);
//}
//if (GetSearchString(_T("CONTROL_ROT_TRACK"), false) || GetSearchString(_T("CONTROL_ROT_TCB"), false) || GetSearchString(_T("CONTROL_ROT_BEZIER"), false))
//{
//	GetTrackListFromString(pObject->m_RotTrack, ROT_SAMLE_TRACK);
//}
//if (GetSearchString(_T("CONTROL_SCALE_TRACK"), false) || GetSearchString(_T("CONTROL_SCALE_TCB"), false) || GetSearchString(_T("CONTROL_SCALE_BEZIER"), false))
//{
//	GetTrackListFromString(pObject->m_SclTrack, SCL_SAMLE_TRACK);
//}




AAseParser::AAseParser()
{

}

AAseParser::~AAseParser()
{
}

bool AAseParser::GetData(const TCHAR* pString, VOID* pData, INT DataType, bool bLoop)
{
	if (GetSearchString(pString, bLoop) != NULL)
	{
		if (DataType == NULL_DATA) {
			return true;
		}

		return GetData(pData, DataType);
	}
	return false;
}

bool AAseParser::GetData(VOID* pData, INT DataType)
{
	if (pData != NULL)
	{
		switch (DataType)
		{
		case INT_DATA:
			_stscanf(m_pwcTokenData.c_str(), _T("%s%d"), m_pString, pData);
			return true;

		case FLOAT_DATA:
			_stscanf(m_pwcTokenData.c_str(), _T("%s%f"), m_pString, pData);
			return true;

		case STRING_DATA:
		{
			T_STR szFind;
			T_STR::size_type idx, idxEnd;
			idx = m_pwcTokenData.find_first_of(_T("\""));
			szFind = m_pwcTokenData.substr(idx + 1);
			idxEnd = szFind.find_first_of(_T("\""));
			_tcscpy((TCHAR*)pData, szFind.substr(0, idxEnd).c_str());
		}
		return true;

		case VERTEX_DATA:
		{
			_stscanf(m_pwcTokenData.c_str(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&((D3DXVECTOR3*)pData)->x,
				&((D3DXVECTOR3*)pData)->z,
				&((D3DXVECTOR3*)pData)->y);
		}
		return true;

		case VECTOR_DATA:
		{
			_stscanf(m_pwcTokenData.c_str(), _T("%s%f%f%f"), m_pString,
				&((D3DXVECTOR3*)pData)->x,
				&((D3DXVECTOR3*)pData)->z,
				&((D3DXVECTOR3*)pData)->y);
		}
		return true;

		case NULL_DATA:
			return true;
		}
	}
	return false;
}


bool AAseParser::GetVertexFromIndexData(const TCHAR* strToken, DWORD dwSearchIndex, VOID* pData, INT DataType)
{
	GetData(strToken);
	TCHAR szBuffer[256] = _T("");
	_stprintf_s(szBuffer, _T("%d"), dwSearchIndex);
	GetData(szBuffer, pData, DataType);
	return true;
}

bool AAseParser::GetVertexListFromString(const TCHAR* strVertexList, DWORD dwNumVertex, vector<D3DXVECTOR3>& VertexList, const TCHAR* strToken)
{
	if (dwNumVertex > 0 && GetData(strVertexList))
	{
		VertexList.resize(dwNumVertex);
		for (DWORD dwVertexCount = 0; dwVertexCount < dwNumVertex; dwVertexCount++)
		{
			D3DXVECTOR3 vVertex;
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			VertexList[dwVertexCount] = vVertex;
		}
	}
	return true;
}


bool AAseParser::GetFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, AVertexList& VertexList, const TCHAR* strToken)
{
	if (dwNumFace > 0 && GetData(strFaceList))
	{
		VertexList.pFaceList.resize(dwNumFace);

		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			AFaceList vFaceList;
			_stscanf(GetNextTokenString(), _T("%s%d %d%d%d"), m_pString, &m_iData,
				&vFaceList._0,
				&vFaceList._2,
				&vFaceList._1);
			VertexList.pFaceList[dwFaceCount] = vFaceList;
		}
	}
	return true;
}
bool AAseParser::GetPositionFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, AVertexList& VertexList, const TCHAR* strToken)
{
	if (dwNumFace > 0 && GetData(strFaceList))
	{
		VertexList.pFaceList.resize(dwNumFace);

		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			AFaceList vFaceList;
			// *MESH_FACE
			
			_stscanf(GetNextTokenString(), _T("%s %s%s %d %s %d %s %d"),
				m_pString, m_pString, m_pString,
				&vFaceList._0, m_pString,	//x축
				&vFaceList._2, m_pString,	//z축
				&vFaceList._1);				//y축
			//x z y 

			// *MESH_SMOOTHING
			GetNextTokenString();
			
			// *MESH_MTLID
			_stscanf(GetNextTokenString(), _T("%s %d"), m_pString, &vFaceList.dwMtrl);

			VertexList.pFaceList[dwFaceCount] = vFaceList;
		}
	}
	return true;
}
bool AAseParser::GetNormalListFromString(const TCHAR* strFaceList, DWORD dwNumFace, AGeomMesh*	pGeomObj, const TCHAR* strToken)
{
	if (GetData(strFaceList))
	{
		pGeomObj->m_NorVertexList.pVertexList.resize(dwNumFace * 4);

		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			// *MESH_FACENORMAL
			D3DXVECTOR3 vVertex;
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 0] = vVertex;

			// *MESH_VERTEXNORMAL				
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 1] = vVertex;

			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 3] = vVertex;

			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 2] = vVertex;
		}
	}
	return true;
}


bool AAseParser::GetTrackListFromString(vector<AAnimTrack>& vTrack, AseTrackType TrackType)
{
	for (;;)
	{
		AAnimTrack Track;

		if (TrackType == POS_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &Track.iTick,
				&Track.vVector.x,
				&Track.vVector.z,
				&Track.vVector.y);
		}
		else if (TrackType == ROT_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f%f"), m_pString, &Track.iTick,
				&Track.qRotate.x, &Track.qRotate.z, &Track.qRotate.y, &Track.qRotate.w);
		}
		else if (TrackType == SCL_SAMLE_TRACK)		//7개 벡터성분 || 회전성분
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f %f%f%f%f"), m_pString, &Track.iTick,
				&Track.vVector.x, &Track.vVector.z, &Track.vVector.y,
				&Track.qRotate.x, &Track.qRotate.z, &Track.qRotate.y, &Track.qRotate.w);
		}
		else if (TrackType == VIS_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f"),
				m_pString, &Track.iTick,
				&Track.vVector.x);
		}
		vTrack.push_back(Track);

		if (_tcsstr(m_pwcTokenData.c_str(), _T("}")) != NULL)
		{
			break;
		}
	}
	return true;
}
