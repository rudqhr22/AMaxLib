#include "ACharMgr.h"

////////////////////////////////////////////////////////////////////////////////
bool		ACharMgr::Init()
{
	m_iCurIndex = 0;
	m_MgrList.clear();
	return true;
}
ACharacter*	const	ACharMgr::GetPtr(int index)
{
	auto itor = m_MgrList.find(index);
	if (itor == m_MgrList.end()) return NULL;
	ACharacter *pPoint = (*itor).second;
	return pPoint;
}

ACharacter* const	ACharMgr::GetPtr(const TCHAR* szName)
{
	for (auto itor = m_MgrList.begin(); itor != m_MgrList.end(); itor++)
	{
		ACharacter *pPoint = (ACharacter *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
		{
			return pPoint;
		}
	}

}
int			ACharMgr::GetID(AModel* pChild)
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
int			ACharMgr::GetID(const TCHAR* szName)
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
int			ACharMgr::Count()
{
	return (int)m_MgrList.size();
}

bool		ACharMgr::Delete(int iIndex)
{
	AModel* const pPoint = GetPtr(iIndex);
	if (pPoint)
	{
		pPoint->Release();
		m_MgrList.erase(iIndex);
	}
	return true;
}
bool		ACharMgr::Delete(AModel* pPoint)
{
	if (pPoint)
	{
		pPoint->Release();
		m_MgrList.erase(GetID(pPoint));
	}
	return true;
}



bool		ACharMgr::Frame()
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
bool		ACharMgr::Render(ID3D11DeviceContext*    pContext)
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

bool		ACharMgr::Release()
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




int		ACharMgr::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*    pImmediateContext,
	const TCHAR* strFileName,
	const TCHAR* strShaderName,
	int		iMatrixIndex)
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
			ACharacter *pPoint = (ACharacter *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}

	ACharacter* pChild = 0;
	SAFE_NEW(pChild, ACharacter);
	pChild->m_szName = szFileName;

	pChild->m_pBoneObject = (ABoneObj*)I_OBJMGR.GetPtr(iMatrixIndex);
	// 기본값 = 전체 프레임을 에니메이션 한다.

	pChild->SetActionFrame(pChild->m_pBoneObject->m_Scene.iFirstFrame,
		pChild->m_pBoneObject->m_Scene.iLastFrame,
		iMatrixIndex);

	/*if( pChild->Add( pd3dDevice,pImmediateContext,
	strFileName, strShaderName,
	iMatrixIndex ) )*/
	{
		m_MgrList.insert(make_pair(m_iCurIndex++, pChild));
		return m_iCurIndex - 1;
	}
	return -1;
};
bool	ACharMgr::Load(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*    pImmediateContext,
	const TCHAR* strFileName)
{
	const TCHAR* Sections[] = { 
		_T("*SKIN_UPPER_BODY_COUNT"), 	_T("*SKIN_UPPER_BODY"),
		_T("*SKIN_LOWER_BODY_COUNT"),	_T("*SKIN_LOWER_BODY"),
		//_T("*SKIN_ARM_COUNT"),		_T("*SKIN_ARM"),
		//_T("*SKIN_HAND_COUNT"),		_T("*SKIN_HAND"),
		//_T("*SKIN_FOOT_COUNT"),		_T("*SKIN_FOOT"),
		//_T("*SKIN_FACE_COUNT"),		_T("*SKIN_FACE"),
		//_T("*SKIN_HAIR_COUNT"),		_T("*SKIN_HAIR"),
		//_T("*SKIN_WEAPON_COUNT"),	_T("*SKIN_WEAPON"),
		//_T("*SKIN_DEFENSE_COUNT"),	_T("*SKIN_DEFENSE"),
		//_T("*SKIN_ETCETERA_COUNT"),	_T("*SKIN_ETCETERA")		,
	};


	int iNumSection = sizeof(Sections) / sizeof(Sections[0]);

	m_Parser.OpenStream(strFileName);
	TCHAR	strCharacterName[MAX_PATH] = { 0, };
	TCHAR	strMeshName[MAX_PATH] = { 0, };
	TCHAR	strShaderName[MAX_PATH] = { 0, };
	TCHAR	strMatrixName[MAX_PATH] = { 0, };
	INT		strMatrixCount;

	int		iNumCharacter = 0;


	if (m_Parser.GetDataFromFileLoop(_T("*NUMCHARACTER"), &iNumCharacter, INT_DATA))
	{
		float a, b, c;
		for (int iNum = 0; iNum < iNumCharacter; iNum++)
		{
			m_Parser.GetDataFromFileLoop(_T("*CHARACTERNAME"), strCharacterName, STRING_DATA);
			m_Parser.GetDataFromFileLoop(_T("*SHADERFILENMAE"), strShaderName, STRING_DATA);
			m_Parser.GetDataFromFileLoop(_T("*MATRIXMATRIXCOUNT"), &strMatrixCount, INT_DATA);
			/*m_Parser.GetDataFromFileLoop(_T("*MATRIXFILENAME"), strMatrixName, STRING_DATA);*/


			//m_Parser.GetDataFromFileLoop(_T("*WORLDPOSITION"), 
			//	&vPos, VERTEX_DATA);

			int		iNumSkin = 0;
			ACharacter* pChar = 0;
			int iCharacterIndex = -1;
			int iMatrixIndex = -1; 
			// 케릭터 전체에서 사용

			for (int i = 0; i < strMatrixCount; i++)
			{
				m_Parser.GetDataFromFileLoop(_T("*MATRIXFILENAME"), strMatrixName, STRING_DATA);
				if (_tcsicmp(strMatrixName, _T("null")))
				{
					iMatrixIndex = I_OBJMGR.LoadObj(pd3dDevice, strMatrixName, _T("../../Data/Shader/MatrixViewer.hlsl"));
				}
			}
			iCharacterIndex = Create(pd3dDevice, pImmediateContext,
				strCharacterName, strShaderName,
				iMatrixIndex);

			if (iCharacterIndex < 0) continue;
			pChar = GetPtr(iCharacterIndex);

			for (int iMesh = 0; iMesh < iNumSection; iMesh += 2)
			{
				m_Parser.GetDataFromFileLoop(Sections[iMesh], &iNumSkin, INT_DATA);
				for (int iSkin = 0; iSkin < iNumSkin; iSkin++)
				{
					m_Parser.GetDataFromSkinMeshString(Sections[iMesh + 1], strMeshName, strMatrixName);
					int iModelMatrixIndex = -1;
					if (_tcsicmp(strMatrixName, _T("null")))
					{
						// 메쉬 단위로 별도의 매트릭스 사용시 적용됨.
						iModelMatrixIndex = I_OBJMGR.LoadObj(pd3dDevice, strMatrixName, _T("../../Data/Shader/MatrixViewer.hlsl"));
					}
					if (pChar)
					{
						pChar->Add(pd3dDevice, pImmediateContext,
							strMeshName, strShaderName,
							iMatrixIndex,
							iModelMatrixIndex);
					}
				}
			}
		}
	}
	m_Parser.CloseStream();
	return true;
}

ACharMgr::ACharMgr()
{
}


ACharMgr::~ACharMgr()
{
}
