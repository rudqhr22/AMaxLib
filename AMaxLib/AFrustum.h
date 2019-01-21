#pragma once
#include "AStd.h"
#include "ADxObjStd.h"
#include "ADxShape.h"

enum A_POSITION
{
	P_BACK = 0,    // Point is positioned behind plane
	P_FRONT,   // Point is positioned in front of plane	
	P_ONPLANE, // Point is positioned on plane
	P_SPANNING // Point is spanning plane
};

class AFrustum
{
public:
	D3DXMATRIX						m_mFstummatViewProj;
	D3DXMATRIX						m_mFrustumWorld;
	D3DXMATRIX						m_mFrustumView;
	D3DXMATRIX						m_mFrustumProj;

public:
	D3DXVECTOR3						m_vFrustumPos[8];
	vector<PNCT_VERTEX>				m_vFrustumVertexList;
	D3DXPLANE						m_FrustumPlane[6];

	ABoxShape*						m_pBoxShape;


public:
	bool		Init();
	bool		Frame();
	bool		PreRender(ID3D11DeviceContext*    pImmediateContext);
	bool		PostRender(ID3D11DeviceContext*    pImmediateContext);
	bool		Render(ID3D11DeviceContext*    pImmediateContext);
	bool		Release();

public:
	void	SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

	bool	CreateFrustum();
	void	CreateFrustumPlane(D3DXPLANE* pPlane, int a, int b, int c);
	
	BOOL	CheackFrustuminPoint(D3DXVECTOR3* pv);
	BOOL	CheckOBBInPlane(A_Box*  pBox);
	BOOL	CheackFrustuminOBB(A_Box*  pBox);
	A_POSITION CheckPoitionOBBInPlane(A_Box*  pBox);
public:
	AFrustum();
	~AFrustum();
};

