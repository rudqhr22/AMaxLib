#pragma once
#include "ADxStd.h"
#include "AStd.h"

struct A_RAY
{
	float			fExtent;		//��?
	D3DXVECTOR3		vOrigin;		//����
	D3DXVECTOR3		vDirection;		//���̹���
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

	//���� ��� ������ ã�Ƴ���.
	bool    GetIntersection(
		D3DXVECTOR3 vStart,	D3DXVECTOR3 vEnd,	D3DXVECTOR3 vNormal,
		D3DXVECTOR3 v0,
		D3DXVECTOR3 v1,
		D3DXVECTOR3 v2);		//���� ����, ����??(���׸�Ʈ), 

	//���� ������ �ﰢ�� �ȿ� �ִ��� Ȯ���Ѵ�.
	bool    PointInPolygon(	D3DXVECTOR3 vert, D3DXVECTOR3 faceNormal,
		D3DXVECTOR3 v0,
		D3DXVECTOR3 v1,
		D3DXVECTOR3 v2);	//��ǥ��, ���� �ﰢ��, v0 v1 v2

	//UV�� �ϴ� ������� �� ��� 2������ ���ÿ� �Ѵ�.
	bool	IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
		D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
		FLOAT* t, FLOAT* u, FLOAT* v);

public:
	ARayCast();
	~ARayCast();
};

