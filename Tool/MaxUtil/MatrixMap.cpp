#include "MatrixMap.h"



void		MatrixMap::Add(int iIndex, T_STR szName)
{
	m_pObj = NULL;
	m_iIndex = iIndex;
	m_szName = szName;
}


void	MatrixMap::SetNode(INode* pNode, int iTick, MatrixMap* pParentIndex)
{
	m_pINode = pNode;
	m_pObj = m_pINode->GetObjectRef();
	m_pControl = m_pINode->GetTMController();

	m_ClassType = CLASS_GEOM;

	if (m_pObj && m_pObj->ClassID() == Class_ID(BONE_CLASS_ID, 0))// 본 오브젝트   
	{
		m_ClassType = CLASS_BONE;
	}
	if (m_pObj && m_pObj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))  // 더미 오브젝트
	{
		m_ClassType = CLASS_DUMMY;
	}
	if (m_pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||        // 루트 바이패드 제외한 모든 바이패드들							 
		m_pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID)	     // 루트 바이패드 1개
	{
		m_ClassType = CLASS_BIPED;
	}

	m_NodeTM = pNode->GetNodeTM(iTick);
	m_InvNodeTM = Inverse(m_NodeTM);
}

//
//MatrixMap::MatrixMap(){}
//
//MatrixMap::~MatrixMap()
//{
//}
//




bool		MatrixMapMGR::Init()
{
	m_iCurIndex = 0;
	MatrixMapList.clear();
	return true;
}

int			MatrixMapMGR::Add(MatrixMap* pChild)
{
	if (pChild == NULL)
	{
		return 0;
	}
	for (auto itor = MatrixMapList.begin(); itor != MatrixMapList.end(); itor++)
	{
		MatrixMap *pPoint = (MatrixMap *)(*itor).second;
		if (pPoint == pChild)
		{
			return 0;
		}
	}
	MatrixMapList.insert(make_pair(m_iCurIndex++, pChild));
	return m_iCurIndex - 1;
}

int			MatrixMapMGR::Add(const TCHAR *pFileName)
{
	// 종복 방지 
	if (pFileName)
	{
		TCHAR szFileName[256];
		TCHAR Drive[MAX_PATH];
		TCHAR Dir[MAX_PATH];
		TCHAR FName[MAX_PATH];
		TCHAR Ext[MAX_PATH];
		_tsplitpath_s(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (auto itor = MatrixMapList.begin(); itor != MatrixMapList.end(); itor++)
		{
			MatrixMap *pPoint = (MatrixMap *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}

	MatrixMap *pPoint = NULL;

	SAFE_NEW(pPoint, MatrixMap);//	MatrixMap *pPoint = new MatrixMap;

	pPoint->Add(m_iCurIndex, pFileName);
	MatrixMapList.insert(make_pair(m_iCurIndex++, pPoint));
	return m_iCurIndex - 1;
}

MatrixMap*		MatrixMapMGR::GetPtr(DWORD index)
{
	auto itor = MatrixMapList.find(index);
	if (itor == MatrixMapList.end()) return NULL;
	MatrixMap *pPoint = (*itor).second;
	return pPoint;
}

MatrixMap*		MatrixMapMGR::GetPtr(const TCHAR* szName)
{
	for (auto itor = MatrixMapList.begin(); itor != MatrixMapList.end(); itor++)
	{
		MatrixMap *pPoint = (MatrixMap *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
		{
			return pPoint;
		}
	}
	return NULL;
}

int			MatrixMapMGR::GetID(MatrixMap* pChild)
{
	int iIndex = -1;
	for (auto itor = MatrixMapList.begin(); itor != MatrixMapList.end(); itor++)
	{
		MatrixMap *pPoint = (MatrixMap *)(*itor).second;
		if (pChild == pPoint)
		{
			iIndex = (*itor).first;
			break;
		}
	}
	return iIndex;
}

int			MatrixMapMGR::GetID(const TCHAR* szName)
{
	int iIndex = -1;
	for (auto itor = MatrixMapList.begin(); itor != MatrixMapList.end(); itor++)
	{
		MatrixMap *pPoint = (MatrixMap *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
		{
			iIndex = (*itor).first;
			break;
		}
	}
	return iIndex;
}

int			MatrixMapMGR::Count()
{
	return (int)MatrixMapList.size();
}

//bool		MatrixMapMGR::Release()
//{
//	return true;
//}