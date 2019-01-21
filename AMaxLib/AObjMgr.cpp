#include "AObjMgr.h"

int			AObjMgr::LoadObj(ID3D11Device* pd3dDevice,
	const TCHAR* strFileName,
	const TCHAR* strShaderName, bool bThread)
{
	TCHAR szFileName[256];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	// 종복 방지 
	if (strFileName)
	{

		_tsplitpath_s(strFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
		{
			AModel *pPoint = (AModel *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}

	if (GetFileTypeID(strFileName) == NULLFILE || m_pModelObj == NULL)
	{
		return -1;
	}

	m_pModelObj->Init();
	m_pModelObj->m_szName = szFileName;
	if (m_pModelObj->Load(pd3dDevice, strFileName, strShaderName, false))
	{
		m_MgrList.insert(make_pair(m_iCurIndex++, m_pModelObj));
		return m_iCurIndex - 1;
	}
	return -1;
}

FILE_TYPE	AObjMgr::GetFileTypeID(const TCHAR* pszFileName)
{
	TCHAR szFileName[256];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(pszFileName, Drive, Dir, FName, Ext);
	Ext[4] = 0;
	_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

	if (!_tcsicmp(Ext, _T(".ASE")))
	{
		AModel* pModelObj = NULL;
		pModelObj= new AAseObj;
		_ASSERT(pModelObj);
		m_pModelObj = pModelObj;
		pModelObj->m_ObjType = ASEFILE;
		return ASEFILE;
	}
	if (!_tcsicmp(Ext, _T(".kkb")))
	{
		AModel* pModelObj = NULL;
		pModelObj = new kkbObj;
		_ASSERT(pModelObj);
		m_pModelObj = pModelObj;
		pModelObj->m_ObjType = KKBFILE;
		return KKBFILE;
	}
	if (!_tcsicmp(Ext, _T(".SKM")))
	{
		AModel* pModelObj = NULL;
		pModelObj =new ASkinObj;
		_ASSERT(pModelObj);
		m_pModelObj = pModelObj;
		pModelObj->m_ObjType = SKMFILE;
		return SKMFILE;
	}
	if (!_tcsicmp(Ext, _T(".MAT")))
	{
		AModel* pModelObj = NULL;
		pModelObj = new ABoneObj;
		_ASSERT(pModelObj);
		m_pModelObj = pModelObj;
		pModelObj->m_ObjType = MATFILE;
		return MATFILE;
	}
	return NULLFILE;
}

void		AObjMgr::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{

	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (pPoint)
		{
			pPoint->SetMatrix(pWorld, pView, pProj);
		}
	}
}




////////////////////////////////////////////////////////////////////////////////
bool		AObjMgr::Init()
{
	m_iCurIndex = 0;
	m_MgrList.clear();
	return true;
}
AModel*	const	AObjMgr::GetPtr(int index)
{
	auto itor = m_MgrList.find(index);
	if (itor == m_MgrList.end()) return NULL;
	AModel *pPoint = (*itor).second;
	return pPoint;
}
AModel* const	AObjMgr::GetPtr(const TCHAR* szName)
{
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
		{
			return pPoint;
		}
	}

}
int			AObjMgr::GetID(AModel* pChild)
{
	int iIndex = -1;
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (pChild == pPoint)
		{
			iIndex = (*itor).first;
			break;
		}
	}
	return iIndex;
}
int			AObjMgr::GetID(const TCHAR* szName)
{
	int iIndex = -1;
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
		{
			iIndex = (*itor).first;
			break;
		}
	}
	return true;
}
int			AObjMgr::Count()
{
	return (int)m_MgrList.size();
}

bool		AObjMgr::Delete(int iIndex)
{
	AModel* const pPoint = GetPtr(iIndex);
	if (pPoint)
	{
		pPoint->Release();
		m_MgrList.erase(iIndex);
	}
	return true;
}
bool		AObjMgr::Delete(AModel* pPoint)
{
	if (pPoint)
	{
		pPoint->Release();
		m_MgrList.erase(GetID(pPoint));
	}
	return true;
}



bool		AObjMgr::Frame()
{
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (pPoint) 
		{
			pPoint->Frame();
		}
	}
	return true;
}
bool		AObjMgr::Render(ID3D11DeviceContext*    pContext)
{
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (pPoint)
		{
			pPoint->Render(pContext);
		}
	}
	return true;
}

bool		AObjMgr::Release()
{
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		AModel *pPoint = (AModel *)(*itor).second;
		if (pPoint)
		{
			pPoint->Release();
		}
	}
	return true;
}


AObjMgr::AObjMgr()
{
}


AObjMgr::~AObjMgr()
{
}
