#include "ADxShape.h"

bool ALineShape::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxObj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice.Get(), m_dxObj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxObj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));

	return true;
}

bool ALineShape::CreateVertexData()
{
	m_LineVertexList.resize(2);
	m_LineVertexList[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[1].p = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_LineVertexList[1].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	m_dxObj.m_iVertexSize = sizeof(PC_VERTEX);
	m_dxObj.m_iNumVertex = m_LineVertexList.size();
	return true;
}
bool ALineShape::CreateIndexData()
{
	m_IndexList.resize(2);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;

	m_dxObj.m_iNumIndex = m_IndexList.size();
	m_dxObj.m_iIndexSize = sizeof(DWORD);
	return true;
}
bool ALineShape::CreateVertexBuffer()
{
	if (m_dxObj.m_iNumVertex <= 0) return S_OK;

	m_dxObj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice.Get(),
		&m_LineVertexList.at(0),		m_dxObj.m_iNumVertex,		m_dxObj.m_iVertexSize));

	return true;
}

bool ALineShape::Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor)
{
	PC_VERTEX vertices[2];
	vertices[0].p = vStart;
	vertices[0].c = vColor;
	vertices[1].p = vEnd;
	vertices[1].c = vColor;
	// 동적 리소스 갱신 방법 1 : D3D11_USAGE_DEFAULT 사용
	pContext->UpdateSubresource(m_dxObj.g_pVertexBuffer.Get(), 0, NULL, &vertices, 0, 0);
	return Render(pContext);
}
bool ALineShape::CreatePrimitiveType()
{
	m_dxObj.m_iPrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	return true;
}
ALineShape::ALineShape(void)
{
}
ALineShape::~ALineShape(void)
{
}


bool ADirectionLineShape::SetInputLayout()
{


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	 m_dxObj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice.Get(),
		m_dxObj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxObj.g_pVSBlob.Get()->GetBufferPointer(), 
		layout, numElements));

	return true;
}
bool ADirectionLineShape::CreateVertexData()
{
	m_LineVertexList.resize(6);
	m_LineVertexList[0] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	m_LineVertexList[1] = PC_VERTEX(D3DXVECTOR3(5000.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	m_LineVertexList[2] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_LineVertexList[3] = PC_VERTEX(D3DXVECTOR3(0.0f, 5000.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_LineVertexList[4] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
	m_LineVertexList[5] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 5000.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));

	m_dxObj.m_iVertexSize = sizeof(PC_VERTEX);
	m_dxObj.m_iNumVertex = m_LineVertexList.size();
	return true;
}
bool ADirectionLineShape::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 3;	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5;

	m_dxObj.m_iNumIndex = m_IndexList.size();
	m_dxObj.m_iIndexSize = sizeof(DWORD);
	return true;
}
bool ADirectionLineShape::CreateVertexBuffer()
{
	if (m_dxObj.m_iNumVertex <= 0) return false;
	m_dxObj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice.Get(), &m_LineVertexList.at(0), m_dxObj.m_iNumVertex,
		m_dxObj.m_iVertexSize));
	return true;
}
bool ADirectionLineShape::CreatePrimitiveType()
{
	m_dxObj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	return true;
}
ADirectionLineShape::ADirectionLineShape(void)
{
}
ADirectionLineShape::~ADirectionLineShape(void)
{
}










bool APlaneShape::CreateVertexData()
{
	if (m_PNCTList.size() <= 0)	{
	
		m_PNCTList.resize(4);
		m_PNCTList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		m_PNCTList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1, 0.0f));
		m_PNCTList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1, 1));
		m_PNCTList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1));
	
	}
	else
	{
		m_PNCTList[0] = PNCT_VERTEX(m_PNCTList[0].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		m_PNCTList[1] = PNCT_VERTEX(m_PNCTList[1].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
		m_PNCTList[2] = PNCT_VERTEX(m_PNCTList[2].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
		m_PNCTList[3] = PNCT_VERTEX(m_PNCTList[3].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	}

	m_dxObj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxObj.m_iNumVertex = m_PNCTList.size();
	return true;
}


bool APlaneShape::CreateIndexData()
{
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	
	m_IndexList[3] = 0;
	m_IndexList[4] = 2;
	m_IndexList[5] = 3;

	m_dxObj.m_iNumIndex = m_IndexList.size();
	m_dxObj.m_iIndexSize = sizeof(DWORD);
	return true;
}


//bool APlaneShape::SetScreenVertex(float x,	float y,float w,float h,D3DXVECTOR2 vScreen)
//{
//	m_PNCTList.resize(4);
//
//	D3DXVECTOR2 vPoint;
//	// 0 ~ 1
//	vPoint.x = x / vScreen.x;
//	vPoint.y = y / vScreen.y;
//	// 0~1 => -1 ~ +1
//	vPoint.x = vPoint.x *  2.0f - 1.0f;
//	vPoint.y = -(vPoint.y *  2.0f - 1.0f);
//
//	D3DXVECTOR2 vOffset;
//	// 0 ~ 1
//	vOffset.x = (x + w) / vScreen.x;
//	vOffset.y = (y + h) / vScreen.y;
//	// 0~1 => -1 ~ +1
//	vOffset.x = (vOffset.x *  2.0f - 1.0f) - vPoint.x;
//	vOffset.y = vPoint.y + (vOffset.y *  2.0f - 1.0f);
//
//	m_PNCTList[0].p.x = vPoint.x;
//	m_PNCTList[0].p.y = vPoint.y;
//	m_PNCTList[0].p.z = 0.5f;
//	m_PNCTList[1].p.x = vPoint.x + vOffset.x;
//	m_PNCTList[1].p.y = vPoint.y;
//	m_PNCTList[1].p.z = 0.5f;
//	m_PNCTList[2].p.x = vPoint.x + vOffset.x;
//	m_PNCTList[2].p.y = vPoint.y - vOffset.y;
//	m_PNCTList[2].p.z = 0.5f;
//	m_PNCTList[3].p.x = vPoint.x;
//	m_PNCTList[3].p.y = vPoint.y - vOffset.y;
//	m_PNCTList[3].p.z = 0.5f;
//	return true;
//}

bool APlaneShape::CreatePrimitiveType()
{
	m_dxObj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return true;
}
APlaneShape::APlaneShape(void)
{

}
APlaneShape::~APlaneShape(void)
{
}


//
//bool		APlane::SetInputLayout()
//{
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//
//	UINT numElements = sizeof(layout) / sizeof(layout[0]);
//	m_dxObj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice.Get(), m_dxObj.g_pVSBlob.Get()->GetBufferSize(),
//		m_dxObj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
//
//	return true;
//}
//
//bool			APlane::CreateVertexData()
//{
//	m_PCTList.resize(4);
//	m_PCTList[0] = { D3DXVECTOR3(-1,  1, 0.5f) ,D3DXVECTOR2(0.0f, 0.0f) };
//	m_PCTList[1] = { D3DXVECTOR3(1,  1, 0.5f) ,D3DXVECTOR2(1.0f, 0.0f) };
//	m_PCTList[2] = { D3DXVECTOR3(1, -1, 0.5f) ,D3DXVECTOR2(1.0f, 1.0f) };
//	m_PCTList[3] = { D3DXVECTOR3(-1, -1, 0.5f) ,D3DXVECTOR2(0.0f, 1.0f) };
//	return true;
//}
//bool			APlane::CreateIndexData()
//{
//	m_IndexList.resize(6);
//	m_IndexList = { 0,1,3,1,2,3 };
//	return true;
//}
//
//bool			APlane::CreatePrimitiveType()
//{
//	m_dxObj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	return true;
//}
//
//APlane::APlane(void)
//{
//}
//APlane::~APlane(void)
//{
//}
////
//
//

bool ABoxShape::CreateVertexData()
{
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_PNCTList.resize(24);
	m_PNCTList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_PNCTList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_PNCTList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_PNCTList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	// 뒷면
	m_PNCTList[4] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_PNCTList[5] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_PNCTList[6] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_PNCTList[7] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f),  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 오른쪽
	m_PNCTList[8] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f) ,  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_PNCTList[9] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),    D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_PNCTList[10] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_PNCTList[11] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 왼쪽
	m_PNCTList[12] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_PNCTList[13] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_PNCTList[14] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_PNCTList[15] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 윗면
	m_PNCTList[16] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_PNCTList[17] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),    D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_PNCTList[18] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_PNCTList[19] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 아랫면
	m_PNCTList[20] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_PNCTList[21] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_PNCTList[22] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),    D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_PNCTList[23] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	m_dxObj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxObj.m_iNumVertex = m_PNCTList.size();
	return true;
}
bool ABoxShape::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	m_dxObj.m_iNumIndex = m_IndexList.size();
	m_dxObj.m_iIndexSize = sizeof(DWORD);
	return true;
}


bool ABoxShape::CreatePrimitiveType()
{
	m_dxObj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return true;
}
ABoxShape::ABoxShape(void)
{
}
ABoxShape::~ABoxShape(void)
{
}



bool ASphereShape::CreateVertexData()
{
	CreateSphere(40);

	m_dxObj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxObj.m_iNumVertex = m_PNCTList.size();
	return true;
}
bool ASphereShape::CreateIndexData()
{
	return true;
}

void ASphereShape::MakeSpherePoint(std::vector<D3DXVECTOR3> &spherePoints,
	UINT Slices,
	double fRadius, D3DXVECTOR3 center)
{

	spherePoints.clear();
	spherePoints.resize(Slices* Slices);

	for (DWORD j = 0; j < Slices; j++)
	{
		FLOAT theta = (D3DX_PI*j) / (Slices);
		for (DWORD i = 0; i< Slices; i++)
		{
			int  iPos = j * Slices + i;
			FLOAT phi = (2 * D3DX_PI*i) / (Slices);
			spherePoints[iPos].x = fRadius * (float)(sin(theta)*cos(phi)) + center.x;
			spherePoints[iPos].z = fRadius * (float)(sin(theta)*sin(phi)) + center.y;
			spherePoints[iPos].y = fRadius * (float)(cos(theta)) + center.z;
		}
	}
}


void ASphereShape::CreateSphere(UINT Slices, float fRadius)
{
	const double PI = 3.141592653589793238462643383279502884197;
	int iPos = 0;
	vector<D3DXVECTOR3> VertexArray;
	MakeSpherePoint(VertexArray, Slices, fRadius, D3DXVECTOR3(0, 0, 0));

	//m_VertexList.resize(VertexArray.size());

	int iNext = 0;
	for (DWORD j = 0; j < Slices; j++)
	{
		for (DWORD i = 0; i<Slices; i++)
		{
			if (i == Slices - 1)         iNext = 0;
			else iNext = i + 1;
			PNCT_VERTEX v0, v1, v2, v3, v4, v5;
			iPos = (j*Slices * 6) + (i * 6);

			v0.p = VertexArray[j*Slices + i];
			v1.p = VertexArray[j*Slices + iNext];

			if (j != Slices - 1) v2.p = VertexArray[((j + 1)*Slices) + i];
			else                v2.p = D3DXVECTOR3(0, -1, 0);

			v3.p = v2.p;
			v4.p = v1.p;

			if (j != Slices - 1) v5.p = VertexArray[((j + 1)*Slices) + iNext];
			else                 v5.p = D3DXVECTOR3(0, -1, 0);

			D3DXVec3Normalize(&v0.n, &v0.p);
			D3DXVec3Normalize(&v1.n, &v1.p);
			D3DXVec3Normalize(&v2.n, &v2.p);
			D3DXVec3Normalize(&v3.n, &v3.p);
			D3DXVec3Normalize(&v4.n, &v4.p);
			D3DXVec3Normalize(&v5.n, &v5.p);

			v0.c = D3DXVECTOR4(v0.n.x, v0.n.y, v0.n.z, 1.0f);
			v1.c = D3DXVECTOR4(v1.n.x, v1.n.y, v1.n.z, 1.0f);
			v2.c = D3DXVECTOR4(v2.n.x, v2.n.y, v2.n.z, 1.0f);
			v3.c = D3DXVECTOR4(v3.n.x, v3.n.y, v3.n.z, 1.0f);
			v4.c = D3DXVECTOR4(v4.n.x, v4.n.y, v4.n.z, 1.0f);
			v5.c = D3DXVECTOR4(v5.n.x, v5.n.y, v5.n.z, 1.0f);

			// 구 텍스처 좌표
			v0.t = D3DXVECTOR2(atan2(v0.n.z, v0.n.x) / (PI*2.0f) + 0.5f, asinf(-v0.n.y) / PI + 0.5f);
			v1.t = D3DXVECTOR2(atan2(v1.n.z, v1.n.x) / (PI*2.0f) + 0.5f, asinf(-v1.n.y) / PI + 0.5f);
			v2.t = D3DXVECTOR2(atan2(v2.n.z, v2.n.x) / (PI*2.0f) + 0.5f, asinf(-v2.n.y) / PI + 0.5f);
			v3.t = D3DXVECTOR2(atan2(v3.n.z, v3.n.x) / (PI*2.0f) + 0.5f, asinf(-v3.n.y) / PI + 0.5f);
			v4.t = D3DXVECTOR2(atan2(v4.n.z, v4.n.x) / (PI*2.0f) + 0.5f, asinf(-v4.n.y) / PI + 0.5f);
			v5.t = D3DXVECTOR2(atan2(v5.n.z, v5.n.x) / (PI*2.0f) + 0.5f, asinf(-v5.n.y) / PI + 0.5f);

			// 구형 환경 맵( 전방만 정상적임 )
			/*	v0.t = D3DXVECTOR2( (v0.n.x + 1) / 2.0f,  (-v0.n.y+1) / 2.0f );
			v1.t = D3DXVECTOR2( (v1.n.x + 1) / 2.0f,  (-v1.n.y+1) / 2.0f);
			v2.t = D3DXVECTOR2( (v2.n.x + 1) / 2.0f,  (-v2.n.y+1) / 2.0f);
			v3.t = D3DXVECTOR2( (v3.n.x + 1) / 2.0f,  (-v3.n.y+1) / 2.0f);
			v4.t = D3DXVECTOR2( (v4.n.x + 1) / 2.0f,  (-v4.n.y+1) / 2.0f);
			v5.t = D3DXVECTOR2( (v5.n.x + 1) / 2.0f,  (-v5.n.y+1) / 2.0f);
			*/
			//////////////////////////////////////
			//   V0.t.x( 0.94f) ---- V1.t.x(0.0f) 일 경우에 
			//   v1.t.x= 1.0f으로 조정한다.
			//////////////////////////////////////
			if (fabs(v1.t.x - v0.t.x) > 0.9f)
			{
				if (fabs(v1.t.x) <= 0.01f)
				{
					v1.t.x = 1.0f;
				}
				if (fabs(v0.t.x) <= 0.01f)
				{
					v0.t.x = 1.0f;
				}
			}
			if (fabs(v2.t.x - v0.t.x) > 0.9f)
			{
				if (fabs(v2.t.x) <= 0.01f)
				{
					v2.t.x = 1.0f;
				}
				if (fabs(v0.t.x) <= 0.01f)
				{
					v0.t.x = 1.0f;
				}
			}
			if (fabs(v4.t.x - v3.t.x) > 0.9f)
			{
				if (fabs(v4.t.x) <= 0.01f)
				{
					v4.t.x = 1.0f;
				}
				if (fabs(v3.t.x) <= 0.01f)
				{
					v3.t.x = 1.0f;
				}
			}
			if (fabs(v5.t.x - v3.t.x) > 0.9f)
			{
				if (fabs(v5.t.x) <= 0.01f)
				{
					v5.t.x = 1.0f;
				}
				if (fabs(v3.t.x) <= 0.01f)
				{
					v3.t.x = 1.0f;
				}
			}
			m_PNCTList.push_back(v0);
			m_PNCTList.push_back(v1);
			m_PNCTList.push_back(v2);
			m_PNCTList.push_back(v3);
			m_PNCTList.push_back(v4);
			m_PNCTList.push_back(v5);
		}
	}
	VertexArray.clear();
}

bool ASphereShape::Render(ID3D11DeviceContext*	pContext)
{
	PreRender(pContext);
	pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
	pContext->Draw(m_dxObj.m_iNumVertex, 0);
	return true;
}
//--------------------------------------------------------------------------------------
 
//--------------------------------------------------------------------------------------
HRESULT ASphereShape::CreateDxResource()
{
	m_dxObj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
ASphereShape::ASphereShape(void)
{
}

ASphereShape::~ASphereShape(void)
{
}

