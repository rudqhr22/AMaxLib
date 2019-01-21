#pragma once

#include "AModel.h"
#include "ADxObjStd.h"


class ADxShape : public AModel
{
public:
	A_Box                       m_Box;
	A_Sphere                    m_Sphere;
	vector<AInstance>			m_pInstanceList;
public:
	//void	CreateOBBBox(float fExtX = 1.0f, float fExtY = 1.0f, float fExtZ = 1.0f,
	//	D3DXVECTOR3 vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//	D3DXVECTOR3 vDirX = D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	//	D3DXVECTOR3 vDirY = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	//	D3DXVECTOR3 vDirZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//void	CreateAABBBox(D3DXVECTOR3 vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	//	D3DXVECTOR3 vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
public:
	ADxShape() {};
	virtual ~ADxShape(void) {};
};



class ALineShape : public ADxShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	bool				SetInputLayout();
	bool				CreateVertexData();
	bool				CreateIndexData();
	bool				CreateVertexBuffer();
	bool			CreatePrimitiveType();
	bool				Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor);
public:
	ALineShape(void);
	virtual ~ALineShape(void);
};




class ADirectionLineShape : public ADxShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	bool			SetInputLayout();
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			CreateVertexBuffer();
	bool			CreatePrimitiveType();
	//draw 함수를 만들어서 뿌리게 할것이다.
public:
	ADirectionLineShape(void);
	virtual ~ADirectionLineShape(void);
};



class APlaneShape : public ADxShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			CreatePrimitiveType();
public:
	APlaneShape(void);
	virtual ~APlaneShape(void);
};


//class APlane : public ADxShape
//{
//public:
//	vector<PT_VERTEX>	m_PCTList;
//public:
//	bool			SetInputLayout();
//	bool			CreateVertexData();
//	bool			CreateIndexData();
//	bool			CreatePrimitiveType();
//
//public:
//	APlane(void);
//	virtual ~APlane(void);
//};






class ABoxShape : public ADxShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			CreatePrimitiveType();
public:
	ABoxShape(void);
	virtual ~ABoxShape(void);
};


class ASphereShape : public ADxShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			Render(ID3D11DeviceContext*	pImmediateContext);
	HRESULT			CreateDxResource();
	void CreateSphere(UINT Slices = 20, float fRadius = 1.0f);
	void MakeSpherePoint(std::vector<D3DXVECTOR3> &spherePoints,
		UINT Slices = 20, double r = 1.0f,
		D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0));
public:
	ASphereShape(void);
	virtual ~ASphereShape(void);
};



