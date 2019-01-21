#pragma once
#include "ADxStd.h"
#include "AStd.h"

struct A_RAY
{
	float			fExtent;		//축?
	D3DXVECTOR3		vOrigin;		//원점
	D3DXVECTOR3		vDirection;		//레이방향
	A_RAY()
	{
		fExtent = -1;
	}
};



class ARayCast
{
public:
	A_RAY		m_Ray;
	D3DXVECTOR3 m_vIntersection;
	float		m_fRayDistance;
public:
	D3DXMATRIX  m_matWorld;
	D3DXMATRIX  m_matView;
	D3DXMATRIX  m_matProj;

	D3DXVECTOR3 qvec;
	D3DXVECTOR3 tvec;
	D3DXVECTOR3 pvec;

public:
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	void UpdateRay();

	//내적 방법 교점을 찾아낸다.
	bool    GetIntersection(
		D3DXVECTOR3 vStart,	D3DXVECTOR3 vEnd,	D3DXVECTOR3 vNormal,
		D3DXVECTOR3 v0,
		D3DXVECTOR3 v1,
		D3DXVECTOR3 v2);		//라인 시작, 엔드??(세그먼트), 

	//외적 교점이 삼각형 안에 있는지 확인한다.
	bool    PointInPolygon(	D3DXVECTOR3 vert, D3DXVECTOR3 faceNormal,
		D3DXVECTOR3 v0,
		D3DXVECTOR3 v1,
		D3DXVECTOR3 v2);	//목표점, 현재 삼각형, v0 v1 v2

	//UV로 하는 방법으로 위 방법 2가지를 동시에 한다.
	bool	IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
		D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
		FLOAT* t, FLOAT* u, FLOAT* v);

public:
	ARayCast();
	~ARayCast();
};

