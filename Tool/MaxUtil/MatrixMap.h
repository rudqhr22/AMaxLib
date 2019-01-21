#pragma once
#include "Maxstd.h"


#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

struct MatrixMap
{
public:
	OBJECTCLASSTYPE			m_ClassType;
	int						m_iIndex;		// 에니메이션 행렬의 인덱스 // 메쉬출력시 정점의 영향을 미치는 행렬 및 가중치.
	T_STR					m_szName;		// 에니메이션 행렬의 이름	
	T_STR					m_szParentName;		// 에니메이션 행렬의 이름	

	vector<P3NCVERTEX>		m_VertexList;	// 메쉬 페이스 리스트
	INode*					m_pINode;
	Object*					m_pObj;

	Matrix3					m_NodeTM;
	Matrix3					m_InvNodeTM;


	Control*				m_pControl;
//	AMesh					m_aMesh;


	void		Add(int iIndex, T_STR szName);
	void		SetNode(INode* pNode, int iTick, MatrixMap* pParentIndex = 0);
	void					Release() {};
public:
//	MatrixMap();
//	~MatrixMap();
};




class MatrixMapMGR : public ASingleton<MatrixMapMGR>
{
	friend class ASingleton<MatrixMapMGR>;
public:
	map <int, MatrixMap*>					MatrixMapList;
	map <int, MatrixMap*>::iterator			MatrixMapItor;

	int										m_iCurIndex;


public:			

	bool		Init();

	int			Add(MatrixMap*);
	int			Add(const TCHAR *pFileName);

	MatrixMap*		GetPtr(DWORD index);

	MatrixMap*		GetPtr(const TCHAR* szName);

	int			GetID(MatrixMap*);

	int			GetID(const TCHAR* szName);

	int			Count();

	bool		Release();


public:
	MatrixMapMGR() {};
	virtual ~MatrixMapMGR() {};

};
#define I_MATRIXMAP MatrixMapMGR::GetInstance()


