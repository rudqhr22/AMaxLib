#include "AMesh.h"



int	AMesh::IsEqulVertex(vector<PNCT_VERTEX>&  VertexArray, PNCT_VERTEX& Vertex)
{
	for (int iVertex = 0; iVertex < VertexArray.size(); iVertex++)
	{
		if (VertexArray[iVertex]== Vertex)
		{
			return iVertex;
		}
	}
	//if (VertexArray[iVertex].p == Vertex.p &&
	//	VertexArray[iVertex].n == Vertex.n &&
	//	VertexArray[iVertex].c == Vertex.c &&
	//	VertexArray[iVertex].t == Vertex.t)

	return -1;
}

int AMesh::IsEqulVertexList(vector<PNCT_VERTEX>&  VertexArray, PNCT_VERTEX& Vertex)
{
	for (int iVertex = 0; iVertex < m_AseVertexList.size(); iVertex++)
	{
		if (m_AseVertexList[iVertex] == Vertex)
		{
			return iVertex;
		}
	}
	return -1;
}


//void		 AMesh::MakeBuffer()
//{
//	int iNumFaces = m_AseFaceList.size() / 3;
//	m_AseVertexList.reserve(iNumFaces * 3);
//	m_AseIndexList.reserve(iNumFaces * 3);
//
//
//	for (int iCnt = 0; iCnt < m_AseFaceList.size(); iCnt++)
//	{
//		int iPosReturn = IsEqulVertex(m_AseVertexList, m_AseFaceList[iCnt]);
//		if (iPosReturn < 0)
//		{
//			m_AseVertexList.push_back(m_AseFaceList[iCnt]);
//			iPosReturn = m_AseVertexList.size() - 1;
//		}
//		m_AseIndexList.push_back(iPosReturn);
//	}
//
//}
//
//int		 AMesh::MakeSubBuffer(vector<PNCT_VERTEX>& vVertex, int isub, int addcnt)
//{
//
//	int iNumFaces = vVertex.size()/3;
//	m_AseVertexList.reserve(iNumFaces);
//	m_AseIndexList.reserve(iNumFaces);
//
//	iNumFaces = 0;
//	PNCT_VERTEX  triSame;
//	triSame.iSubIndex = isub;
//
//	if (isub >= 0)
//	{
//		//IsTriangleSame(isub);
//
//		iNumFaces = count_if(vVertex.begin(), vVertex.end(), IsiSubSame(triSame));
//
//		/*	for (auto A = vVertex.begin(); A < vVertex.end(); A++)
//			{
//				if (A->iSubIndex == isub)
//				{
//					++iNumFaces;
//				}
//			}*/
//	}
//
//	if (addcnt > 0 && iNumFaces>0)
//	{
//		int a = 666;
//	}
//
//	for (int iCnt = 0; iCnt < iNumFaces; iCnt++)
//	{
//		int iPosReturn = IsEqulVertex(m_AseVertexList, vVertex[addcnt+iCnt]);
//		if (iPosReturn < 0)
//		{
//			m_AseVertexList.push_back(vVertex[addcnt+iCnt]);
//			iPosReturn = m_AseVertexList.size() - 1;
//		}
//		m_AseIndexList.push_back(iPosReturn);
//
//	}
//	return iNumFaces;
//}
//
//

int AMesh::SetUniqueBuffer(vector <ATri>& TriList, int iMtrl, int iStartTri)
{
	int iNumFaces = TriList.size();
	m_AseVertexList.reserve(iNumFaces * 3);
	m_AseIndexList.reserve(iNumFaces * 3);

	ATri  triSame(iMtrl);
	if (iMtrl >= 0)
	{
		iNumFaces = count_if(TriList.begin(), TriList.end(), IsTriangleSame(triSame));
	}

	for (int iFace = 0; iFace < iNumFaces; iFace++)
	{
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			int iPosReturn = IsEqulVertexList(m_AseVertexList, TriList[iStartTri + iFace].vVertex[iCnt]);
			if (iPosReturn < 0)
			{
				m_AseVertexList.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
				iPosReturn = m_AseVertexList.size() - 1;
			}
			m_AseIndexList.push_back(iPosReturn);
		}
	}

	return iNumFaces;
}


AMesh::AMesh()
{
	m_pParent = NULL;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matWorldTrans);
	D3DXMatrixIdentity(&m_matWorldRotate);
	D3DXMatrixIdentity(&m_matWorldScale);
	D3DXMatrixIdentity(&m_matCalculation);
	m_bUnAnimObj = false;
	m_bSelect = false;
}


AMesh::~AMesh()
{
	SAFE_DEL(m_pParent);

	m_pSubMesh.clear();
	m_pChildMesh.clear();



}