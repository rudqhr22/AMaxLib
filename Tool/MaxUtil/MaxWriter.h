#pragma once
#include "MaxGlobal.h"
#include "MaxObj.h"

struct TriListSort
{
	bool operator()(ATri& rpStart, ATri& rpEnd)
	{
		return rpStart.iSubIndex < rpEnd.iSubIndex;
	}
};

struct TriListSame
{
	ATri					 m_Tri;
	TriListSame(ATri data) : m_Tri(data) {}

	bool operator()(ATri& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};

static int g_iSearchIndex = 0;
struct IsSameInt    
{
	bool operator()(ATri &value)
	{
		return value.iSubIndex == g_iSearchIndex;
	}
};


class MaxWriter : public MaxObj , public ASingleton<MaxWriter>
{
	friend class  ASingleton<MaxWriter>;
public:
	FILE*		m_pStream;	// 출력 파일 포인터
	TSTR		m_strFileName;	// 출력 할 파일명

public:
	bool 	Init(TSTR strName, Interface* Interface); // 초기화
	bool	Export();		// 파일 출력
	bool	Release();		// 메모리 소멸 및 해제	
	int		SetUniqueBuffer(AMesh* pMesh, int iMtrl, int iStartTri, vector <PNCT_VERTEX>& VertexArray, vector<int>& IndexArray);
	int		IsEqulVertexList(vector<PNCT_VERTEX>&  VertexArray, PNCT_VERTEX& Vertex);
public:
	void	ExpMatrix();
	void	ExpScene();
	void	ExpMaterial();
	void	ExpObject();
	int		ExpMesh(FILE* fp, AMesh* pMesh, int iMtrl = -1, int iAddCount = 0);
	void	ExpTexture(vector<ATexMap>& TexMap);
	void ExpAnimation(AMesh* pMesh);
	TCHAR* SaveFileDlg(TCHAR* szExt, TCHAR* szTitle);
public:
	MaxWriter();
	virtual ~MaxWriter();
};

#define I_WRITER MaxWriter::GetInstance()
