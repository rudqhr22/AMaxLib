#pragma once
#include "AMesh.h"
#include "AAseObj.h"
#include <fstream>



struct SkinMesh : public ATri_PNCT5
{
public:
	int										m_iNumFace;
	vector<PNCT5_VERTEX>					m_VertexArray;
	vector<DWORD>							m_IndexArray;
	vector<ATri_PNCT5>						m_TriList;
	vector<SkinMesh*>						m_pSubMesh;
	vector<PNCT5_VERTEX*>					m_pChildMesh;
	PNCT5_VERTEX*							m_pDrawVertex;
	D3DXMATRIX								m_matInverse;
public:
	int IsEqulVertexList(vector<PNCT5_VERTEX>&  VertexArray, PNCT5_VERTEX& Vertex);
	int SetUniqueBuffer(vector <ATri_PNCT5>& TriList, int iMtrl, int iStartTri);
	SkinMesh(void)
	{
		m_pDrawVertex = NULL;
		m_iNumFace = 0;
	}
};


class ASkinObj : public AModel
{
public:
	fstream								m_FP;
	int									m_iMaxWeight;
	vector<SkinMesh*>					m_pSkinMeshList;
	vector<D3DXMATRIX>					m_matBipedList;

	vector<D3DXMATRIX>* GetMatrix() { return &m_matBipedList; };

/*	vector<AInstatnce>			m_pInstance;
	ComPtr<ID3D11Buffer>		m_pVBInstance*/;

public:
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();
	bool		Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	bool		Convert(const TCHAR* strPathName = 0);
	bool		LoadMesh(FILE	*fp, AMesh* pMesh, SkinMesh* pData, const TCHAR* szFileName);

//	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool		SetInputLayout();
	bool		UpdateBuffer();
	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, AModel* pParent);
	bool		LoadTexture(ID3D11Device* device, const TCHAR* fileName);

//	HRESULT	CreateInstance(ID3D11Device* m_pd3dDevice, UINT iNumInstance);
	bool	RenderInstancing(ID3D11DeviceContext* pContext);
//	bool	CreateVertexBuffer();
//	bool	CreateIndexBuffer();
public:
	ASkinObj();
	virtual ~ASkinObj();
};

