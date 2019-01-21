#include "ALoadToken.h"

bool	ALoadToken::CutDirName(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	_tsplitpath_s(strFileName, Drive, Dir, FName, Ext);
	m_szDirName = Drive;
	m_szDirName += Dir;
	m_szName = FName;
	m_szName += Ext;

	return true;
}


bool ALoadToken::LoadBuffer(const TCHAR* strFileName)
{

	LARGE_INTEGER FileSize;
	//파일 오픈!!!


	CutDirName(strFileName);

	m_hHandle = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hHandle == NULL) return false;

	GetFileSizeEx(m_hHandle, &FileSize);	//총 읽은 파일 사이트 확인

	m_pmbStaticMeshData = new char[FileSize.LowPart];	//사이즈 만큼 멀티바이트를 할당

	DWORD dwBytesRead;	//얼만큼 읽었는지, ㅁㅏ지막이 비동기 인듯
	if (ReadFile(m_hHandle, m_pmbStaticMeshData, FileSize.LowPart, &dwBytesRead, NULL) == FALSE)	//리드파일에 따라서 비동기로 사용 할수 있다.
	{
		delete[] m_pmbStaticMeshData;
		CloseHandle(m_hHandle);
		return false;
	}
	

	//밑 작업은 멀티 바이트 데이터를 유니코드로 ////카피 구지 안해도 된다????????
	m_pwcStaticMeshData = new TCHAR[FileSize.LowPart];

	TCHAR* szMeshData = NULL;
	TCHAR* SearchString = NULL;
	size_t convertedChars = 0;

	//멀티 바이트 데이터를 유니코드로 카피
	mbstowcs_s(&convertedChars, m_pwcStaticMeshData, FileSize.LowPart, m_pmbStaticMeshData, _TRUNCATE);

	delete[] m_pmbStaticMeshData;	//멀티바이트 메쉬데이터는 필요 없으니 지워준다.
	m_pmbStaticMeshData = NULL;

	//메쉬데이터를 토큰 데이터에 넣어준다
	m_pwcTokenData = m_pwcStaticMeshData;	
	//m_pwcSearchData = m_pwcStaticMeshData;	


	CloseHandle(m_hHandle);


	BeginToken();		

	return true;
}



bool ALoadToken::BeginToken()
{
	m_TokenIndex = 0;
	m_TokenMaxSize = 0;
	
	T_STR		strTokenData;
	strTokenData = m_pwcTokenData;

	//m_pwcToken = "*"
	size_t start, stop;
	size_t end = strTokenData.length();
	//=================================================================================================
	for (start = strTokenData.find_first_not_of(m_pwcToken);			//토큰이 시작되지 않은 첫번쨰  
		0 <= start && start < end;								
		start = strTokenData.find_first_not_of(m_pwcToken, stop + 1))	
	{
		stop = strTokenData.find_first_of(m_pwcToken, start);			//다음 토큰을 찾아서 stop

		//예외사항
		if (stop < 0 || stop > end) {
			stop = end;
		}
		m_ListTokens.push_back(strTokenData.substr(start, stop - start));
	}
	//=================================================================================================
	
	m_TokenMaxSize = (DWORD)m_ListTokens.size();

	return true;
}


//문자를 비교함
const TCHAR* ALoadToken::GetSearchString(const TCHAR* szSearchData, bool bLoop)
{
	//찾을 스트링 검색
	const TCHAR* szSearchStr = NULL;
	GetNextTokenString();		//다음라인
	if (!bLoop)					//반복하지 않겟다면 한줄 찾고, 리턴
	{
		szSearchStr = _tcsstr(m_pwcTokenData.c_str(), szSearchData);	//스트링 비교함수
		if (szSearchStr == NULL)
		{
			--m_TokenIndex;
		}
	}
	else
	{	//기본값으로 반복
		szSearchStr = _tcsstr(m_pwcTokenData.c_str(), szSearchData);	//스트링 비교함수
		while (szSearchStr == NULL)
		{
			GetNextTokenString();
			szSearchStr = _tcsstr(m_pwcTokenData.c_str(), szSearchData);
			if (_tcsstr(m_pwcTokenData.c_str(), szSearchData) != NULL)
			{
				break;
			}
		}
	}
	return szSearchStr;
}

const TCHAR* ALoadToken::GetNextTokenString()
{
	if (m_TokenIndex + 1 >= m_TokenMaxSize)			//최대치 넘으가면 널을 반환
	{
		return NULL;
	}

	m_pwcTokenData = m_ListTokens[++m_TokenIndex].c_str();	//토큰 벡터 리스트[라인임] 다음을 넣어준다.
	return m_pwcTokenData.c_str();
}

//토큰에 있는 문자를 넘김
const TCHAR* ALoadToken::GetCurrentTokenString()
{
	return m_pwcTokenData.c_str();
}


//다음 라인으로 넘어가면서, 배열에 있는 문자열을 돌아가면서 모두 비교를 한다. 그래서 넘어가는 순서가 제일 중요함.
INT ALoadToken::SearchTokenArray(INT NumObjType, const TCHAR** pObjectType, DWORD dwNextObjectIndex)
{
	INT iIndex = -1;
	bool IsLineCheck = true;

	while (IsLineCheck)
	{
		//if (dwNextObjectIndex > 0 && dwNextObjectIndex - 1 <= m_TokenIndex)
		//{
		//	//아무 것도 없을 떄 -1을 리턴 한다.?
		//	break;
		//}

		if (GetNextTokenString() == NULL) break;

		for (INT iType = 0; iType < NumObjType; iType++)
		{
			if (_tcsstr(m_pwcTokenData.c_str(), pObjectType[iType]) != NULL)
			{
				iIndex = iType;
				IsLineCheck = false;
				break;
			}
		}

	}
	return iIndex;
}

ALoadToken::ALoadToken()
{
	ZeroMemory(m_pString, sizeof(TCHAR) * 256);
	m_pmbStaticMeshData = 0;
	m_pwcStaticMeshData = 0;
	m_pwcToken = _T("*");
}


ALoadToken::~ALoadToken()
{
}
