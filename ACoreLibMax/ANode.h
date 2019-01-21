#pragma once
#include "ACamera.h"
#include "ADxObjStd.h"
#include "ABaseObj.h"
class ANode
{
public:
	DWORD				m_dwDepth;
	BOOL				m_bLeaf;
	BOOL				m_isTile;
	DWORD				m_dwLodLevel;
	DWORD				m_dwLodType;
	// �ٿ�� �ڽ�
	A_Box				m_Box;
	vector<DWORD>		m_CornerIndex;
	// ����� �ڳ� ���� �ε��� �� ��ġ
	vector<D3DXVECTOR3>	m_CornerList;
	vector<ANode*>		m_ChildList;
	vector<ANode*>		m_NeighborList;
	vector<ABaseObj*>	m_ObjectList;
	// �߰�
	//DWORD				m_dwPositionIndex[2];// X,Y	

	vector <PNCT_VERTEX>	m_PNCT_VertexList;
	vector<DWORD>			m_IndexList;

	//D3D11_BOX			m_BoxVB;
	//D3D11_BOX			m_BoxIB;
	//UINT				m_iBeginVB;
	//UINT				m_iBeginIB;


	ComPtr<ID3D11Buffer>	m_pVB;
	ComPtr<ID3D11Buffer>	m_pIB;

public:
	ANode();
	~ANode();
};

