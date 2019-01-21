#include "ACharacterLoader.h"



BOOL	ACharacterLoader::OpenStream(const TCHAR* pFileName)
{
	m_pStream = _tfopen(pFileName, _T("rt"));
	if (m_pStream == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL ACharacterLoader::ComparisonData(TCHAR* pDestString, TCHAR* pSrcString)
{
	if (pSrcString == NULL)
	{
		pSrcString = m_pString;
	}
	if (!_tcsicmp(pSrcString, pDestString))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL ACharacterLoader::GetData(VOID* pData, INT DataType)
{
	if (pData != NULL)
	{
		switch (DataType)
		{
		case INT_DATA:
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, pData);
			break;
		case FLOAT_DATA:
			_stscanf(m_pBuffer, _T("%s%f"), m_pString, pData);
			break;
		case STRING_DATA:
		{
			TCHAR szTexPath[MAX_PATH] = TEXT("");
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, szTexPath);
			if (szTexPath[0] == '"')
			{
				// 문자열의 시작과 끝에 있는 (")문자를 삭제한다.
				// 또한 문자열중간에 공백까지도 반환문자열에 포함한다.
				_tcscpy(szTexPath, _tcsstr(m_pBuffer, _T("\"")));
				size_t filelen = _tcslen(szTexPath);
				szTexPath[filelen - 2] = 0;
				_tcscpy((TCHAR*)pData, &szTexPath[1]);
			}
			else
			{
				_tcscpy((TCHAR*)pData, &szTexPath[0]);
			}
		}
		break;
		case VERTEX_DATA:
		{
			_stscanf(m_pBuffer, _T("%s %f %f %f"),
				m_pString,
				&((D3DXVECTOR3*)pData)->x,
				&((D3DXVECTOR3*)pData)->z,
				&((D3DXVECTOR3*)pData)->y);
		}
		break;
		case FACE_DATA:
		case MATRIX_DATA:
		case NULL_DATA:
			break;
		}
	}
	return TRUE;
}
BOOL ACharacterLoader::GetDataFromFileNext(TCHAR* pString, VOID* pData, INT DataType)
{
	//LPCSTR pBuffer = NULL;	

	if (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, pString))
		{
			return GetData(pData, DataType);
		}
	}

	return FALSE;
}

BOOL ACharacterLoader::GetDataFromFile(TCHAR* pString, VOID* pData, INT DataType)
{

	fpos_t  FilePos;
	if (fgetpos(m_pStream, &FilePos))
	{
		//ErrorCloseStream(_T("작업도중 파일포인터를 잃어 버렸습니다."));
	}
	if (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, pString))
		{
			return GetData(pData, DataType);
		}
		else
		{
			if (fsetpos(m_pStream, &FilePos))
			{
				//ErrorCloseStream(_T("작업도중 파일포인터를 잃어 버렸습니다."));
			}
		}
	}

	return FALSE;
}
// 원하는 문자열을 찾고 찾은 문자열 시작위치로 옮긴다
BOOL ACharacterLoader::GetDataFromFileString(TCHAR* pString, VOID* pData, INT DataType)
{
	fpos_t  FilePos;

	while (!feof(m_pStream))
	{
		if (fgetpos(m_pStream, &FilePos))
		{
			//ErrorCloseStream(_T("작업도중 파일포인터를 잃어 버렸습니다."));
		}
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, pString))
		{
			if (GetData(pData, DataType))
			{
				if (fsetpos(m_pStream, &FilePos))
				{
					//ErrorCloseStream(_T("작업도중 파일포인터를 잃어 버렸습니다."));
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}
BOOL ACharacterLoader::GetNodeName(TCHAR* pString, VOID* pData, INT DataType)
{

	fpos_t  FilePos;
	if (fgetpos(m_pStream, &FilePos))
	{
		//ErrorCloseStream(_T("작업도중 파일포인터를 잃어 버렸습니다."));
	}
	if (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, pString))
		{
			if (pData != NULL)
			{
				switch (DataType)
				{
				case STRING_DATA:
				{
					TCHAR *pStr = NULL;
					TCHAR String1[20];
					TCHAR String2[20];
					TCHAR ResultString[40];
					memset(String1, 0, sizeof(TCHAR) * 20);
					memset(String2, 0, sizeof(TCHAR) * 20);
					memset(ResultString, 0, sizeof(TCHAR) * 40);
					bool check = false;
					int j = 0;
					if (_tcsstr(m_pBuffer, _T("Bip01 R UpperArm")))
					{
						int k;
						k = 0;
					}
					for (int i = 0; i<256; i++)
					{
						if (check)
						{
							if (m_pBuffer[i] == '"')
							{
								check = false;
								String2[j++] = 0;
								break;
							}
							String2[j++] = m_pBuffer[i];
						}
						else
						{
							if (m_pBuffer[i] == '"')
								check = true;
						}

					}
					//String2[filelen-1] = 0;					
					_tcscpy((TCHAR*)pData, &String2[0]);
				}
				break;
				}
			}
			return TRUE;
		}
		else
		{
			if (fsetpos(m_pStream, &FilePos))
			{
			//	ErrorCloseStream(_T("작업도중 파일포인터를 잃어 버렸습니다."));
			}
		}
	}

	return FALSE;
}

BOOL ACharacterLoader::GetDataFromFileLoop(const TCHAR* pString, VOID* pData, INT DataType)
{
	LPCSTR pBuffer = NULL;//사용처가 없기 때문에 없어도 됨..
						  // 왜 있느냐고 묻는다면. 예전에 수업하던 것이라. 기억이 안남.. 따지지마...!!!
	while (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, pString))
		{
			return GetData(pData, DataType);
		}
	}
	return FALSE;//ErrorCloseStream( "전체파일에서 검색할수 없습니다.");	
}

BOOL	ACharacterLoader::GetDataFromSkinMeshString(const TCHAR* pString, void* pData0, void* pData1)
{
	TCHAR szPath_0[MAX_PATH] = TEXT("");
	TCHAR szPath_1[MAX_PATH] = TEXT("");

	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s%s%s"), m_pString, szPath_0, szPath_1);
	if (!_tcsicmp(m_pString, pString))
	{
		_tcscpy((TCHAR*)pData0, &szPath_0[0]);
		_tcscpy((TCHAR*)pData1, &szPath_1[0]);
		return TRUE;
	}
	return FALSE;
}

ACharacterLoader::ACharacterLoader()
{

}


ACharacterLoader::~ACharacterLoader()
{
}
