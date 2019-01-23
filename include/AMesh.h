#pragma once
#include "AModel.h"
#include "AAseObjStd.h"

struct TriListSort
{
	bool operator()(ATri& rpStart, ATri& rpEnd)
	{
		return rpStart.iSubIndex < rpEnd.iSubIndex;
	}
};

struct TriListSame
{
	ATri					 m_Tri;
	TriListSame(ATri data) : m_Tri(data) {}

	bool operator()(ATri& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};

//struct DescendingFaceSort
//{
//	bool operator()(PNCT_VERTEX& rpStart, PNCT_VERTEX& rpEnd)
//	{
//		return rpStart.iSubIndex < rpEnd.iSubIndex;
//	}
//};
//
//struct DescendingPNCTSort
//{
//	bool operator()(PNCT_VERTEX& rpStart, PNCT_VERTEX& rpEnd)
//	{
//		return rpStart.iSubIndex < rpEnd.iSubIndex;
//	}
//};
//
//struct IsiSubSame
//{
//	PNCT_VERTEX			m_Tri;
//	IsiSubSame(PNCT_VERTEX data) : m_Tri(data) {}
//	bool operator()(PNCT_VERTEX& value)
//	{
//		return value.iSubIndex == m_Tri.iSubIndex;
//	}
//};

struct DescendingTriSort
{
	bool operator()(ATri& rpStart, ATri& rpEnd)
	{
		return rpStart.iSubIndex < rpEnd.iSubIndex;
	}
};

struct IsTriangleSame
{
	ATri			m_Tri;
	IsTriangleSame(ATri data) : m_Tri(data) {}
	bool operator()(ATri& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};

struct IsTriangleSame5
{
	ATri_PNCT5			m_Tri;
	IsTriangleSame5(ATri_PNCT5 data) : m_Tri(data) {}
	bool operator()(ATri_PNCT5& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};


class AMesh : public AModel
{
public:
	//int						m_iIndex;

	T_STR								m_strNodeName;
	T_STR								m_strParentName;
	
	AMesh*								m_pParent;
	vector<AMesh*>						m_pChildMesh;

	int iSubIndex;

	int   m_iNumTrack[4];
	int   m_iMtrlRef;


	int									m_iNumFace;
	int									m_iDiffuseTex;

	vector<PNCT_VERTEX>					m_AseFaceList;
	vector<PNCT_VERTEX>					m_AseVertexList;
	vector<DWORD>						m_AseIndexList;

	ComPtr<ID3D11Buffer>				m_pVertexBuffer;
	ComPtr<ID3D11Buffer>				m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView>	m_pTextureSRV;

	vector<AMesh*>				m_pSubMesh;




	D3DXMATRIX				m_matInverse;		//�����

	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matWorldScale;	//�������
	D3DXMATRIX				m_matWorldRotate;	//ȸ�����
	D3DXMATRIX				m_matWorldTrans;	//������


	D3DXMATRIX				m_matCalculation;	//�������

	vector<AAnimTrack*>		m_PosTrack;		// �̵� Ʈ�� 
	vector<AAnimTrack*>		m_RotTrack;		// ȸ�� Ʈ��
	vector<AAnimTrack*>		m_SclTrack;		// ���� Ʈ��												
	vector<AAnimTrack*>		m_VisTrack;		// ���� Ʈ��

	OBJECTCLASS_TYPE			m_ClassType;
	bool						m_bUnAnimObj;

	vector<ATri>			m_TriList;



	A_Box					m_Box;
	//bool					m_bSelect;
	int						m_iNumMtrl;
	int						m_iNumVertexs;
	int						m_iNumTex;
	int						m_iTexType[13];
	TCHAR					m_szTexName[13][30];
	//ComPtr<ID3D11CommandList>      m_pd3dCommandList;
public:

	int IsEqulVertex(vector<PNCT_VERTEX>&  VertexArray, PNCT_VERTEX& Vertex);
	int IsEqulVertexList(vector<PNCT_VERTEX>&  VertexArray, PNCT_VERTEX& Vertex);

	//	void MakeBuffer();
	//	int MakeSubBuffer(vector<PNCT_VERTEX>& vVertex,int isub,int addcnt=0);
	int SetUniqueBuffer(vector <ATri>& TriList, int iMtrl, int iStartTri);

	AMesh();
	virtual ~AMesh();
};

