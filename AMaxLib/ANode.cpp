#include "ANode.h"



ANode::ANode()
{
	m_dwLodType = 0;
	m_dwDepth = 0;
	m_bLeaf = FALSE;
	m_Box.vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Box.vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Box.vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_Box.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Box.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Box.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Box.fExtent[0] = 1;
	m_Box.fExtent[1] = 1;
	m_Box.fExtent[2] = 1;


	
}


ANode::~ANode()
{
	if (!m_CornerList.empty()) m_CornerList.clear();
	if (!m_CornerIndex.empty()) m_CornerIndex.clear();	// Ãß°¡

	for (int i = 0; i < m_ChildList.size(); i++)
	{
		delete m_ChildList[i];
		m_ChildList[i] = NULL;
		
	}


	if (!m_PNCT_VertexList.empty()) m_PNCT_VertexList.clear();
	if (!m_IndexList.empty()) m_IndexList.clear();

}
