#pragma once
//#include "AMesh.h"
#include "AAseParser.h"
#include "AMesh.h"
//#include "AModel.h"



class AAseObj : public AModel
{
private:
	const TCHAR*					m_FileName;
public:
	float					m_fTickSpeed;
	float					m_fElapseTime;
	float					fFrameTick=5;
	int						m_iMaxVertex = 0;
	int						m_iMaxIndex = 0;


public:
	AScene				m_Scene;
	AAseParser			m_Parser;

	vector<AMtrl>		m_MtrlList;			//텍스처 정보가 들어 있다.
	vector<AGeomMesh*>	m_GeomObjList;		//지오브젝트가 들어 갈 구조체가 필요
	
											
	vector<AMesh*>		m_MeshList;		//모든 정보를 여기에 담게 된다.

public:
	bool	Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	bool	Convert(ID3D11Device* pd3dDevice);

	bool	CreateVertexBuffer();
	bool	CreateIndexBuffer();
	//bool	CreateConstantBuffer();


	bool	LoadTexture(ID3D11Device* device, const TCHAR* fileName);

	bool	Frame();
	bool	Render(ID3D11DeviceContext* context);
	
public:	
	//bool	SetMeshFaceList(AGeomMesh* GObj, vector<AMesh*>& MeshList, AMesh* pMesh);
	bool	SetMeshFaceList(AGeomMesh* GObj, AMesh* pMesh);
	bool	SetMeshSubFaceList(AGeomMesh* GObj, vector<AMesh*>& MeshList);

	bool	ConvertAnim(AGeomMesh* GObj, AMesh* pMesh);
	void	AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick);
	bool	GetAnimationTrack(float fFrameTick,vector<AAnimTrack*> pTrackList, AAnimTrack** ppStartTrack, AAnimTrack** ppEndTrack);
	AAnimTrack* SetLinkedTrack(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev);


	float PlayTrackTick(float fFrameTick, float fStartTick,float fEndTick);
	AMesh* SearchToCollects(T_STR	m_strParentName);

	//bool SetInputLayout();
	bool AseRender(ID3D11DeviceContext*    pContext, AModel* pParent);
	int GetMapID(AMtrl* pMtrl, int iTexMapType);


	void	SetTriangleBuffer(AGeomMesh* GObjList, AMesh* pMesh);

public:
	AAseObj();
	virtual ~AAseObj();
};






class AAnimation
{
public:
	D3DXMATRIX	Interpolate(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick, AScene tScene);
	D3DXMATRIX	Update(float fElapseTime, AMesh* pMesh, D3DXMATRIX &matWorld, AScene &tScene);
	bool		GetAnimationTrack(float fFrame, vector<AAnimTrack*>& pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack);
public:
	AAnimation(void);
	virtual ~AAnimation(void);
};





//int iCount = 0;
//for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
//{
//	AMesh* pSubMesh = new AMesh;
//	pSubMesh->m_matWorld = pMesh->m_matWorld;
//	pMesh->m_pSubMesh.push_back(pSubMesh);
//
//}
//
//SetMeshSubFaceList(m_GeomObjList[A], pMesh->m_pSubMesh);
//
//for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
//{
//	AMesh* pSubMesh = pMesh->m_pSubMesh[iSub];
//	pSubMesh->MakeBuffer();
//	pSubMesh->m_iDiffuseTex = -1;
//
//
//	if (m_GeomObjList[A]->m_iMtrlRef >= 0)
//	{
//		AMtrl* pSubMtrl = &pMtrl->m_SubMtrl[iSub];
//		if (pSubMtrl->m_TexMaps.size() == 0) continue;
//		pSubMesh->m_iDiffuseTex = pSubMtrl->m_TexMaps[0].m_dwIndex;
//	}
//}

//else
	//{
	//	int iCount = 0;
	//	for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
	//	{
	//		AMesh* pSubMesh = new AMesh;
	//		pSubMesh->m_matWorld = pMesh->m_matWorld;
	//		pMesh->m_pSubMesh.push_back(pSubMesh);
	//	}
	//	SetMeshSubFaceList(m_GeomObjList[A], pMesh->m_pSubMesh);
	//	for (int iSub = 0; iSub < pMtrl->m_SubMtrl.size(); iSub++)
	//	{
	//		AMesh* pSubMesh = pMesh->m_pSubMesh[iSub];
	//		pSubMesh->MakeBuffer();
	//		//pMesh->m_strNodeName = m_GeomObjList[A]->m_strNodeName;
	//		//pMesh->m_matWorld = m_GeomObjList[A]->m_matWorld;				
	//		pSubMesh->m_iDiffuseTex = -1;
	//		if (m_GeomObjList[A]->m_iMtrlRef >= 0)
	//		{
	//			AMtrl* pSubMtrl = &pMtrl->m_SubMtrl[iSub];
	//			if (pSubMtrl->m_TexMaps.size() == 0) continue;
	//			pSubMesh->m_iDiffuseTex = pSubMtrl->m_TexMaps[0].m_dwIndex;
	//		}
	//	}
	//}

//
//bool		AAseObj::SetMeshSubFaceList(AGeomMesh* GObjList, vector<AMesh*>& MeshList)
//{
//	D3DXMATRIX matInverse;
//	PNCT_VERTEX vVertex;
//	D3DXMatrixInverse(&matInverse, NULL, &GObjList->m_matWorld);
//
//
//	for (DWORD dwFace = 0; dwFace < GObjList->m_iNumFace; dwFace++)					//12
//	{
//
//		DWORD dwMtrl = GObjList->m_PosVertexList.pFaceList[dwFace].dwMtrl;
//
//		for (DWORD dwVertex = 0; dwVertex < 3; dwVertex++)							//*3 =36 트라이앵글을 안만들기 떄문
//		{
//			if (GObjList->m_PosVertexList.dwNumVertex > 0 && GObjList->m_PosVertexList.dwNumFace > 0)
//			{
//				DWORD dwIndex = GObjList->m_PosVertexList.pFaceList[dwFace].v[dwVertex];
//				vVertex.p = GObjList->m_PosVertexList.pVertexList[dwIndex];
//			}
//
//			if (GObjList->m_NorVertexList.pVertexList.size() > 0)
//			{
//				vVertex.n = GObjList->m_NorVertexList.pVertexList[dwFace * 4 + dwVertex + 1];	//줄 번거 띄기위해 *4
//																								//pMesh->m_AseVertexList[dwFace].n = GObjList[A]->m_NorVertexList.pVertexList[dwFace * 4 + dwVertex + 1];
//			}
//
//			if (GObjList->m_ColVertexList.dwNumVertex && GObjList->m_ColVertexList.dwNumFace)
//			{
//				DWORD dwIndex = GObjList->m_ColVertexList.pFaceList[dwFace].v[dwVertex];
//
//				vVertex.c.x = GObjList->m_ColVertexList.pVertexList[dwIndex].x;
//				vVertex.c.y = GObjList->m_ColVertexList.pVertexList[dwIndex].z;
//				vVertex.c.z = GObjList->m_ColVertexList.pVertexList[dwIndex].y;
//				vVertex.c.w = 1.0f;
//			}
//			else
//			{
//				vVertex.c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
//			}
//
//			if (GObjList->m_TexVertexList.dwNumVertex && GObjList->m_TexVertexList.dwNumFace)
//			{
//				DWORD dwIndex = GObjList->m_TexVertexList.pFaceList[dwFace].v[dwVertex];
//
//				vVertex.t.x = GObjList->m_TexVertexList.pVertexList[dwIndex].x;
//				vVertex.t.y = 1.0f - GObjList->m_TexVertexList.pVertexList[dwIndex].z;
//			}
//			// 로칼 정점 = 월드 정점 * 월드행렬의 역행렬
//			D3DXVec3TransformCoord(&vVertex.p, &vVertex.p, &matInverse);	//역행렬을 곱해서 로컬로 만듬
//			MeshList[dwMtrl]->m_AseFaceList.push_back(vVertex);

//			MeshList[dwMtrl]->m_pSubMesh[0].m_AseFaceList.push_back(vVertex);
//		}
//	}
//	return true;
//}

//template <class T >
//int SetUniqueBuffer(vector <TTri<T>>& TriList, int iMtrl, int iStartTri)
//{
//	int iNumFaces = TriList.size();
//	m_VertexArray.reserve(iNumFaces * 3);
//	m_IndexArray.reserve(iNumFaces * 3);
//
//	TTri<T>  triSame(iMtrl);
//	if (iMtrl >= 0)
//	{
//		iNumFaces = count_if(TriList.begin(), TriList.end(), IsTriangleSame<T>(triSame));
//		iNumFaces = count_if(TriList.begin(), TriList.end(), IsTriangleSame<T>(triSame));
//
//	}
//
//	for (int iFace = 0; iFace < iNumFaces; iFace++)
//	{
//		for (int iCnt = 0; iCnt < 3; iCnt++)
//		{
//			int iPosReturn = IsEqulVertex(m_VertexArray, TriList[iStartTri + iFace].vVertex[iCnt]);
//			if (iPosReturn < 0)
//			{
//				m_VertexArray.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
//				iPosReturn = m_VertexArray.size() - 1;
//			}
//			m_IndexArray.push_back(iPosReturn);
//		}
//	}
//
//	return iNumFaces;
//}



//int SetUniqueBuffer(vector <AMesh>& TriList, int iMtrl, int iStartTri)
//{
//	int iNumFaces = TriList.size();
//	m_VertexArray.reserve(iNumFaces * 3);
//	m_IndexArray.reserve(iNumFaces * 3);
//
//	//TTri<T>  triSame(iMtrl);
//	if (iMtrl >= 0)
//	{
//		iNumFaces = count_if(TriList.begin(), TriList.end(), PNCT_VERTEX());
//		//iNumFaces = count_if(TriList.begin(), TriList.end(), IsTriangleSame<T>(triSame));
//
//	}
//	for (int iFace = 0; iFace < iNumFaces; iFace++)
//	{
//		for (int iCnt = 0; iCnt < 3; iCnt++)
//		{
//			int iPosReturn = IsEqulVertex(m_VertexArray, TriList[iStartTri + iFace].vVertex[iCnt]);
//			if (iPosReturn < 0)
//			{
//				m_VertexArray.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
//				iPosReturn = m_VertexArray.size() - 1;
//			}
//			m_IndexArray.push_back(iPosReturn);
//		}
//	}
//
//	return iNumFaces;
//}


// struct IsTriangleSame
//{
//	PNCT_VERTEX			m_Tri;
//	IsTriangleSame(PNCT_VERTEX data) : m_Tri(data) {}
//	bool operator()(PNCT_VERTEX& value)
//	{
//		return value == m_Tri;
//	}
//};

//template< class T > struct IsTriangleSame
//{
//	ATri<T>			m_Tri;
//	IsTriangleSame(TTri<T> data) : m_Tri(data) {}
//	bool operator()(TTri<T>& value)
//	{
//		return value.iSubIndex == m_Tri.iSubIndex;
//	}
//};
