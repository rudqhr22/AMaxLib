#include "AFrustum.h"

bool		AFrustum::Init()
{
	ZeroMemory(m_vFrustumPos, sizeof(m_vFrustumPos[0]) * 8);
	ZeroMemory(m_FrustumPlane, sizeof(m_FrustumPlane[0]) * 6);


	return true;
}

void	AFrustum::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_mFrustumWorld = *pWorld;
	}
	else
	{
		D3DXMatrixIdentity(&m_mFrustumWorld);
	}

	if (pView != NULL)
	{
		m_mFrustumView = *pView;
	}
	if (pProj != NULL)
	{
		m_mFrustumProj = *pProj;
	}
}


bool	AFrustum::CreateFrustum()
{
	// 윗면
	// 5    6
	// 1    2
	// 아랫면
	// 4    7
	// 0    3  

	//0123 앞
	//4567 뒤

	//중심 0 기준으로 
	m_vFrustumPos[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vFrustumPos[1] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vFrustumPos[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vFrustumPos[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vFrustumPos[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_vFrustumPos[5] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vFrustumPos[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vFrustumPos[7] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	//한번에 곱해서 역행렬로 사용 
	D3DXMatrixMultiply(&m_mFstummatViewProj, &m_mFrustumView, &m_mFrustumProj);
	D3DXMatrixInverse(&m_mFstummatViewProj,NULL,&m_mFstummatViewProj);


	//벡터 pV (x, y, z, 1)를 행렬 pM 에 의해 변환 해, 그 결과를 w=1 에 투영 한다.
	//w 1로 두고 행렬을 곱한다.
	for (int i= 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_vFrustumPos[i], &m_vFrustumPos[i], &m_mFstummatViewProj);
	}


	//그걸로 평면을 만듬
	CreateFrustumPlane(&m_FrustumPlane[0], 5, 0, 1);
	CreateFrustumPlane(&m_FrustumPlane[1], 3, 6, 2);
	CreateFrustumPlane(&m_FrustumPlane[2], 5, 2, 6);
	CreateFrustumPlane(&m_FrustumPlane[3], 0, 7, 3);
	CreateFrustumPlane(&m_FrustumPlane[4], 0, 2, 1);
	CreateFrustumPlane(&m_FrustumPlane[5], 6, 4, 5);


	return true;
}


void	AFrustum::CreateFrustumPlane(D3DXPLANE* pPlane,int a, int b , int c)
{
	
	D3DXVECTOR3 vEdge0 = m_vFrustumPos[b] - m_vFrustumPos[a];
	D3DXVECTOR3 vEdge1 = m_vFrustumPos[c] - m_vFrustumPos[a];
	D3DXVECTOR3 vNormal;

	D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);					//외적해서 노말 \을 만든다
	D3DXVec3Normalize(&vNormal, &vNormal);

	//평면의 방정식 ax+by+cz+d=0;
	pPlane->a = vNormal.x;
	pPlane->b = vNormal.y;
	pPlane->c = vNormal.z;
	pPlane->d = -(pPlane->a *  m_vFrustumPos[a].x +		//노말x * 
				  pPlane->b *  m_vFrustumPos[a].y +		//노말y * 
				  pPlane->c *  m_vFrustumPos[a].z);		//노말z    모두 더하면 거리 인데 - 를 해서 안쪽으로 방향

	//D3DXPlaneFromPointNormal();	점1, 평면노말1로 => 평면
	//D3DXPlaneFromPoints();		점3개			 => 평면생성
	//D3DXPlaneDot()				평면과 4벡터 내적

	//D3DXPlaneDotCoord()			
	//D3DXPlaneDotNormal()			점1- 평면 -> 내적


	//D3DXPlaneIntersectLine()		평면-직선 교점의 계산
	//D3DXPlaneTransform()			행렬을 평면으로 반환
	
}


BOOL AFrustum::CheckOBBInPlane(A_Box*  pBox)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	D3DXVECTOR3 vDir;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);

		fPlaneToCenter = m_FrustumPlane[iPlane].a * pBox->vCenter.x + m_FrustumPlane[iPlane].b*pBox->vCenter.y +
			m_FrustumPlane[iPlane].c * pBox->vCenter.z + m_FrustumPlane[iPlane].d;

		if (fPlaneToCenter <= -fDistance)
		{
			return FALSE;
		}
	}
	return TRUE;
}


//6평면을 돌면서 점하고 비교 q
BOOL	AFrustum::CheackFrustuminPoint(D3DXVECTOR3* pv)
{
	float fPoint;
	for (int i = 0; i < 6; i++)		
	{
		//fPoint = D3DXPlaneDotNormal(&m_FrustumPlane[i], pv); //평면, 점1개 내적 ->	안됨X  == 0만 나온닿
		fPoint = m_FrustumPlane[i].a * pv->x +
				 m_FrustumPlane[i].b * pv->y +
				 m_FrustumPlane[i].c * pv->z +
				 m_FrustumPlane[i].d;

		//6평면 중 한번이라도 걸리면 포함 X
		if (fPoint < 0.0f)
		{
			//점이 평면 안쪽 노말에 없닿
			return false;
		}
	}

	return true;
}




///OBB 노말 직교축으로 부터 직교축 거리를 모두 더한다.
BOOL	AFrustum::CheackFrustuminOBB(A_Box*  pBox)
{
	float		fPlaneCenter = 0.0;
	float		fDistance = 0.0f;
	D3DXVECTOR3 vDir;

	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];	//1직교축 * 거리
		fDistance = fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		
		vDir = pBox->vAxis[1] * pBox->fExtent[1];	//2직교축 * 거리
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		
		vDir = pBox->vAxis[2] * pBox->fExtent[2];	//3직교축 * 거리
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);


		fPlaneCenter =  m_FrustumPlane[iPlane].a * pBox->vCenter.x + 
						m_FrustumPlane[iPlane].b * pBox->vCenter.y +
						m_FrustumPlane[iPlane].c * pBox->vCenter.z + 
						m_FrustumPlane[iPlane].d;
	
		if (fPlaneCenter <= -fDistance)
		{
			return false;
		}

	}
	return true;

}
A_POSITION AFrustum::CheckPoitionOBBInPlane(A_Box*  pBox)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	D3DXVECTOR3 vDir;
	A_POSITION  t_Position;

	t_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);

		fPlaneToCenter = m_FrustumPlane[iPlane].a * pBox->vCenter.x + m_FrustumPlane[iPlane].b*pBox->vCenter.y +
			m_FrustumPlane[iPlane].c * pBox->vCenter.z + m_FrustumPlane[iPlane].d;

		if (fPlaneToCenter <= fDistance)
		{
			t_Position = P_SPANNING;
		}

		if (fPlaneToCenter + 1.0f < -fDistance)
		{
			return P_BACK;
		}
	}
	return t_Position;
}


//실시간 버텍스 리스트를 만들어서 갱신을 한다. 물론 지워주기도 해야함.
bool AFrustum::PreRender(ID3D11DeviceContext*    pContext)
{
	m_vFrustumVertexList[0]  = PNCT_VERTEX(m_vFrustumPos[1], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_vFrustumVertexList[1]  = PNCT_VERTEX(m_vFrustumPos[2], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_vFrustumVertexList[2]  = PNCT_VERTEX(m_vFrustumPos[3], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	m_vFrustumVertexList[3]  = PNCT_VERTEX(m_vFrustumPos[0], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_vFrustumVertexList[4]  = PNCT_VERTEX(m_vFrustumPos[6], D3DXVECTOR3(0.0f, 0.0f, 1.0f),  D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_vFrustumVertexList[5]  = PNCT_VERTEX(m_vFrustumPos[5], D3DXVECTOR3(0.0f, 0.0f, 1.0f),  D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_vFrustumVertexList[6]  = PNCT_VERTEX(m_vFrustumPos[4], D3DXVECTOR3(0.0f, 0.0f, 1.0f),  D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	m_vFrustumVertexList[7]  = PNCT_VERTEX(m_vFrustumPos[7], D3DXVECTOR3(0.0f, 0.0f, 1.0f),  D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_vFrustumVertexList[8]  = PNCT_VERTEX(m_vFrustumPos[5], D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_vFrustumVertexList[9]  = PNCT_VERTEX(m_vFrustumPos[1], D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_vFrustumVertexList[10] = PNCT_VERTEX(m_vFrustumPos[0], D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	m_vFrustumVertexList[11] = PNCT_VERTEX(m_vFrustumPos[4], D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_vFrustumVertexList[12] = PNCT_VERTEX(m_vFrustumPos[2], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_vFrustumVertexList[13] = PNCT_VERTEX(m_vFrustumPos[6], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_vFrustumVertexList[14] = PNCT_VERTEX(m_vFrustumPos[7], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	m_vFrustumVertexList[15] = PNCT_VERTEX(m_vFrustumPos[3], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_vFrustumVertexList[16] = PNCT_VERTEX(m_vFrustumPos[5], D3DXVECTOR3(0.0f, 1.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_vFrustumVertexList[17] = PNCT_VERTEX(m_vFrustumPos[6], D3DXVECTOR3(0.0f, 1.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_vFrustumVertexList[18] = PNCT_VERTEX(m_vFrustumPos[2], D3DXVECTOR3(0.0f, 1.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	m_vFrustumVertexList[19] = PNCT_VERTEX(m_vFrustumPos[1], D3DXVECTOR3(0.0f, 1.0f, 0.0f),  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_vFrustumVertexList[20] = PNCT_VERTEX(m_vFrustumPos[0], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_vFrustumVertexList[21] = PNCT_VERTEX(m_vFrustumPos[3], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_vFrustumVertexList[22] = PNCT_VERTEX(m_vFrustumPos[7], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	m_vFrustumVertexList[23] = PNCT_VERTEX(m_vFrustumPos[4], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));

	pContext->UpdateSubresource(m_pBoxShape->m_dxObj.g_pVertexBuffer.Get(), 0, NULL, &m_vFrustumVertexList.at(0), 0, 0);
	m_pBoxShape->SetMatrix(NULL, &m_mFrustumView, &m_mFrustumProj);
	m_pBoxShape->m_dxObj.PreRender(pContext);
	return true;
}



bool AFrustum::PostRender(ID3D11DeviceContext*    pContext)
{
//	m_pBoxShape->cbData.vColor = D3DXVECTOR4(0.8,0.8,0.8,0.5);
	pContext->UpdateSubresource(m_pBoxShape->m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &m_pBoxShape->cbData, 0, 0);
	m_pBoxShape->m_dxObj.PostRender(pContext);
	return true;
}

AFrustum::AFrustum()
{
	Init();

}


AFrustum::~AFrustum()
{
	int a = 999;
	delete m_pBoxShape;
}



//맵 구조잡기

//리프노드 단위 VB IB
//1.모든 정점의 위치가 중복 될 수가 있다
//2.지형편집을 할떄, 공유정점이 아니라서 다 같이 안올라 간다. (공간분할로 안들어왔을때 도 못 올린다.)
//3.타일랜더링에는 편하다 -> 노드단위당 랜더링 할 수 있어서 편함
//4.LOD 할때 편하다.


//리프노드 당 VB - IB 공용
//1.지형편집이 편하다(IB는 다 같아서)
//2.타일랜더가 힘들다(위치는 같지만, UV가 다르다)

//리프노드 당 IB - VB 공용
//리프노드의 퀼리티(루프노드의 사이값을이 어려움)
//코너값으로 인덱스 버퍼로 만들어진다. 인덱스 코너값으로 랜더링으로 LOD 가능
//그러나 정점이 스킵 되기 때문에  (크랙을)떔빵을 해야 한다

//2가지의 장점
//1. 텍스처좌표 2개씩 가지고 있으면 된다.		//전체로 할때와, 따로 할때
//2. 멀티스트림, PNCT를 쪼갯 4개를 만들면 된다. -> PNCT를 풀러서 사용


//리프단위로 랜더를 타일랜더와 LOD가 편하다
//버텍스 컨트롤이 힘들다.
//1개 PNCT를 -> 각각  4개씩 더 쪼갬?
//글로벌 VB를 쪼갠다.