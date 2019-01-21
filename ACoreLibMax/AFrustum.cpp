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
	// ����
	// 5    6
	// 1    2
	// �Ʒ���
	// 4    7
	// 0    3  

	//0123 ��
	//4567 ��

	//�߽� 0 �������� 
	m_vFrustumPos[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vFrustumPos[1] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vFrustumPos[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vFrustumPos[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vFrustumPos[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_vFrustumPos[5] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vFrustumPos[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vFrustumPos[7] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	//�ѹ��� ���ؼ� ����ķ� ��� 
	D3DXMatrixMultiply(&m_mFstummatViewProj, &m_mFrustumView, &m_mFrustumProj);
	D3DXMatrixInverse(&m_mFstummatViewProj,NULL,&m_mFstummatViewProj);


	//���� pV (x, y, z, 1)�� ��� pM �� ���� ��ȯ ��, �� ����� w=1 �� ���� �Ѵ�.
	//w 1�� �ΰ� ����� ���Ѵ�.
	for (int i= 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_vFrustumPos[i], &m_vFrustumPos[i], &m_mFstummatViewProj);
	}


	//�װɷ� ����� ����
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

	D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);					//�����ؼ� �븻 \�� �����
	D3DXVec3Normalize(&vNormal, &vNormal);

	//����� ������ ax+by+cz+d=0;
	pPlane->a = vNormal.x;
	pPlane->b = vNormal.y;
	pPlane->c = vNormal.z;
	pPlane->d = -(pPlane->a *  m_vFrustumPos[a].x +		//�븻x * 
				  pPlane->b *  m_vFrustumPos[a].y +		//�븻y * 
				  pPlane->c *  m_vFrustumPos[a].z);		//�븻z    ��� ���ϸ� �Ÿ� �ε� - �� �ؼ� �������� ����

	//D3DXPlaneFromPointNormal();	��1, ���븻1�� => ���
	//D3DXPlaneFromPoints();		��3��			 => ������
	//D3DXPlaneDot()				���� 4���� ����

	//D3DXPlaneDotCoord()			
	//D3DXPlaneDotNormal()			��1- ��� -> ����


	//D3DXPlaneIntersectLine()		���-���� ������ ���
	//D3DXPlaneTransform()			����� ������� ��ȯ
	
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


//6����� ���鼭 ���ϰ� �� q
BOOL	AFrustum::CheackFrustuminPoint(D3DXVECTOR3* pv)
{
	float fPoint;
	for (int i = 0; i < 6; i++)		
	{
		//fPoint = D3DXPlaneDotNormal(&m_FrustumPlane[i], pv); //���, ��1�� ���� ->	�ȵ�X  == 0�� ���´�
		fPoint = m_FrustumPlane[i].a * pv->x +
				 m_FrustumPlane[i].b * pv->y +
				 m_FrustumPlane[i].c * pv->z +
				 m_FrustumPlane[i].d;

		//6��� �� �ѹ��̶� �ɸ��� ���� X
		if (fPoint < 0.0f)
		{
			//���� ��� ���� �븻�� ����
			return false;
		}
	}

	return true;
}




///OBB �븻 ���������� ���� ������ �Ÿ��� ��� ���Ѵ�.
BOOL	AFrustum::CheackFrustuminOBB(A_Box*  pBox)
{
	float		fPlaneCenter = 0.0;
	float		fDistance = 0.0f;
	D3DXVECTOR3 vDir;

	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];	//1������ * �Ÿ�
		fDistance = fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		
		vDir = pBox->vAxis[1] * pBox->fExtent[1];	//2������ * �Ÿ�
		fDistance += fabs(m_FrustumPlane[iPlane].a * vDir.x + m_FrustumPlane[iPlane].b*vDir.y + m_FrustumPlane[iPlane].c * vDir.z);
		
		vDir = pBox->vAxis[2] * pBox->fExtent[2];	//3������ * �Ÿ�
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


//�ǽð� ���ؽ� ����Ʈ�� ���� ������ �Ѵ�. ���� �����ֱ⵵ �ؾ���.
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



//�� �������

//������� ���� VB IB
//1.��� ������ ��ġ�� �ߺ� �� ���� �ִ�
//2.���������� �ҋ�, ���������� �ƴ϶� �� ���� �ȿö� ����. (�������ҷ� �ȵ������� �� �� �ø���.)
//3.Ÿ�Ϸ��������� ���ϴ� -> �������� ������ �� �� �־ ����
//4.LOD �Ҷ� ���ϴ�.


//������� �� VB - IB ����
//1.���������� ���ϴ�(IB�� �� ���Ƽ�)
//2.Ÿ�Ϸ����� �����(��ġ�� ������, UV�� �ٸ���)

//������� �� IB - VB ����
//��������� ����Ƽ(��������� ���̰����� �����)
//�ڳʰ����� �ε��� ���۷� ���������. �ε��� �ڳʰ����� ���������� LOD ����
//�׷��� ������ ��ŵ �Ǳ� ������  (ũ����)������ �ؾ� �Ѵ�

//2������ ����
//1. �ؽ�ó��ǥ 2���� ������ ������ �ȴ�.		//��ü�� �Ҷ���, ���� �Ҷ�
//2. ��Ƽ��Ʈ��, PNCT�� �ɰ� 4���� ����� �ȴ�. -> PNCT�� Ǯ���� ���


//���������� ������ Ÿ�Ϸ����� LOD�� ���ϴ�
//���ؽ� ��Ʈ���� �����.
//1�� PNCT�� -> ����  4���� �� �ɰ�?
//�۷ι� VB�� �ɰ���.