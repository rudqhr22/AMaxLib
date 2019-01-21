#pragma once
#include "ABaseObj.h"



//쿼드트리에 테스트에 사용할 박스 오브젝트

class A_BoxObj : public ABaseObj
{
public:
	D3DXMATRIX	m_matWorld;
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR4 m_vColor;

public:
	A_BoxObj(void);
//	virtual ~A_BoxObj(void);
};


A_BoxObj::A_BoxObj()
{
	m_Box.vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Box.vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Box.vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	m_vPosition = D3DXVECTOR3(25 - rand() % 50, 25 - rand() % 50, 25 - rand() % 50);
	m_vColor = D3DXVECTOR4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);

	// g_matWorld Matrix = S * R
	D3DXMATRIX matScale, matRotation, matWorld;
	D3DXMatrixScaling(&matScale, (rand() % 256) / 255.0f * 2.0f, (rand() % 256) / 255.0f * 2.0f, (rand() % 256) / 255.0f * 2.0f);
	D3DXMatrixRotationYawPitchRoll(&matRotation, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f);
	D3DXMatrixMultiply(&matWorld, &matScale, &matRotation);

	// OBB 기저벡터 변환( 회전 변환 이후 정규화 )
	m_Box.vCenter = m_vPosition;
	D3DXVECTOR3 vMax, vMin, vHalf;
	D3DXVec3TransformCoord(&m_Box.vAxis[0], &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &matWorld);
	D3DXVec3TransformCoord(&m_Box.vAxis[1], &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &matWorld);
	D3DXVec3TransformCoord(&m_Box.vAxis[2], &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &matWorld);
	D3DXVec3Normalize(&m_Box.vAxis[0], &m_Box.vAxis[0]);
	D3DXVec3Normalize(&m_Box.vAxis[1], &m_Box.vAxis[1]);
	D3DXVec3Normalize(&m_Box.vAxis[2], &m_Box.vAxis[2]);

	// g_matWorld Matrix = S * R * T
	matWorld._41 = m_vPosition.x;
	matWorld._42 = m_vPosition.y;
	matWorld._43 = m_vPosition.z;

	// OBB 기저벡터의 크기 계산
	D3DXVec3TransformCoord(&vMax, &m_Box.vMax, &matWorld);
	D3DXVec3TransformCoord(&vMin, &m_Box.vMin, &matWorld);
	vHalf = vMax - m_Box.vCenter;
	m_Box.fExtent[0] = D3DXVec3Dot(&m_Box.vAxis[0], &vHalf);
	m_Box.fExtent[1] = D3DXVec3Dot(&m_Box.vAxis[1], &vHalf);
	m_Box.fExtent[2] = D3DXVec3Dot(&m_Box.vAxis[2], &vHalf);

	// AABB
	D3DXVECTOR3 vVertex[8];
	vVertex[0] = D3DXVECTOR3(m_Box.vMin.x, m_Box.vMax.y, m_Box.vMin.z);
	vVertex[1] = D3DXVECTOR3(m_Box.vMax.x, m_Box.vMax.y, m_Box.vMin.z);
	vVertex[2] = D3DXVECTOR3(m_Box.vMax.x, m_Box.vMin.y, m_Box.vMin.z);
	vVertex[3] = D3DXVECTOR3(m_Box.vMin.x, m_Box.vMin.y, m_Box.vMin.z);

	vVertex[4] = D3DXVECTOR3(m_Box.vMin.x, m_Box.vMax.y, m_Box.vMax.z);
	vVertex[5] = D3DXVECTOR3(m_Box.vMax.x, m_Box.vMax.y, m_Box.vMax.z);
	vVertex[6] = D3DXVECTOR3(m_Box.vMax.x, m_Box.vMin.y, m_Box.vMax.z);
	vVertex[7] = D3DXVECTOR3(m_Box.vMin.x, m_Box.vMin.y, m_Box.vMax.z);

	vMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
	vMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
	for (int iVer = 0; iVer < 8; iVer++)
	{
		D3DXVec3TransformCoord(&vVertex[iVer], &vVertex[iVer], &matWorld);
		if (vVertex[iVer].x > vMax.x) vMax.x = vVertex[iVer].x;
		if (vVertex[iVer].y > vMax.y) vMax.y = vVertex[iVer].y;
		if (vVertex[iVer].z > vMax.z) vMax.z = vVertex[iVer].z;

		if (vVertex[iVer].x < vMin.x) vMin.x = vVertex[iVer].x;
		if (vVertex[iVer].y < vMin.y) vMin.y = vVertex[iVer].y;
		if (vVertex[iVer].z < vMin.z) vMin.z = vVertex[iVer].z;
	}

	m_Box.vMin = vMin;
	m_Box.vMax = vMax;
	m_matWorld = matWorld;
}
