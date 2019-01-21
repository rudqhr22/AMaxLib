#include "ARayCast.h"

//점포함테스트 방법
//a + b   0 < a,b /////   a+b < 1 
//



void ARayCast::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		//m_vCenter.x = pWorld->_41;
		//m_vCenter.y = pWorld->_42;
		//m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	UpdateRay();
}

void ARayCast::UpdateRay()
{
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);

		//뷰좌표 완성
		D3DXVECTOR3 v;
		v.x = (((2.0f * ptCursor.x) / g_iClientWidth) - 1) / m_matProj._11;		//요소로 스케일 뺴준다
		v.y = -(((2.0f * ptCursor.y) / g_iClientHeight) - 1) / m_matProj._22;	//요소로 스케일 뺴줌
		v.z = 1.0f;		

		//g_iClientWidth  = 툴에서 이거 리프레쉬를 정확히 해주어야 한다. 
		//g_iClientHeight = 툴에서 이거 리프레쉬를 정확히 해주어야 한다.

		D3DXMATRIX mWorldView = m_matWorld * m_matView;
		D3DXMATRIX mInverse;
		D3DXMatrixInverse(&mInverse, NULL, &mWorldView);



		m_Ray.vOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Ray.vDirection = D3DXVECTOR3(v.x, v.y, v.z);

		D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &mInverse);			//x y z 1
		D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &mInverse);	//x y z 0	방향이 바뀌면 안되기 떄문에 노말을 곱한다.
		D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
	}
}







//내적 방법 교점을 찾아낸다.
bool    ARayCast::GetIntersection(
	D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR3 vNormal,
	D3DXVECTOR3 v0,	D3DXVECTOR3 v1,	D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vEndStart = vEnd - vStart;
	D3DXVECTOR3 vv0Start= v0 - vStart;

	float fDistance = D3DXVec3Dot(&vNormal, &vEndStart);
	float startTatget = D3DXVec3Dot(&vNormal, &vv0Start);

	float TargetT = startTatget / fDistance;

	if (TargetT < 0.0f || TargetT >= 1.0f)
	{
		return false;
	}

	m_vIntersection = vStart + (vEndStart * TargetT);

	return true;
}

//외적 교점이 삼각형 안에 있는지 확인한다.
bool    ARayCast::PointInPolygon(D3DXVECTOR3 vert, D3DXVECTOR3 faceNormal,
	D3DXVECTOR3 v0,
	D3DXVECTOR3 v1,
	D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vTarget, vNormal;
	D3DXVECTOR3 vEdge1, vEdge2, vEdge0;
	float fDot = 0;
	vEdge0 = v1 - v0;
	vEdge1 = v2 - v1;
	vEdge2 = v0 - v2;

	vTarget = vert - v0;
	D3DXVec3Cross(&vNormal, &vEdge0, &vTarget);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot =D3DXVec3Dot(&faceNormal,&vNormal);
	if (fDot < 0.0f) return false;

	vTarget = vert - v1;
	D3DXVec3Cross(&vNormal, &vEdge1, &vTarget);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;


	vTarget = vert - v2;
	D3DXVec3Cross(&vNormal, &vEdge2, &vTarget);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;


	return true;
}




//UV로 하는 방법으로 위 방법 2가지를 동시에 한다.
bool	ARayCast::IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v)
{

	D3DXVECTOR3 edge0 = v1 - v0;
	D3DXVECTOR3 edge1 = v2 - v0;
	D3DXVECTOR3 uVec, vVec, vTec,vtemp;
	
	D3DXVec3Cross(&uVec,&dir,&edge1);
	FLOAT det = D3DXVec3Dot(&edge0, &uVec);		//행렬식

	if (det > 0)
	{
		vTec = orig - v0;
	}
	else
	{
		vTec = v0 - orig;
		det = -det;
	}

   	*u = D3DXVec3Dot(&vTec,&uVec);
	if (*u < 0.0f || *u >det) return false;


	D3DXVec3Cross(&vVec, &vTec, &edge0);
	*v = D3DXVec3Dot(&dir, &vVec);
	if (*v < 0.0f || *v + *u >det) return false;


	D3DXVec3Cross(&vtemp, &edge0, &edge1);
	*t = D3DXVec3Dot(&vtemp, &vTec);
	

	FLOAT finDet = 1.0f / det;

	*u *= finDet;
	*v *= finDet;
	*t *= finDet;

	return true;
}

ARayCast::ARayCast()
{
	m_fRayDistance = 10000.0f;


	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_vIntersection = D3DXVECTOR3(0, 0, 0);


	//m_vSrcVex[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_vSrcVex[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_vSrcVex[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_fPickDistance = 10000.0f;

	//m_vIntersection = D3DXVECTOR3(0, 0, 0);
	//m_vCenter = D3DXVECTOR3(0, 0, 0);
	//m_vDxR = D3DXVECTOR3(1, 1, 1);
}


ARayCast::~ARayCast()
{
}
