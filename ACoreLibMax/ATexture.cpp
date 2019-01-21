#include "ATexture.h"

HRESULT ATexture::Load(ID3D11Device* device, const TCHAR* fileName)
{
	HRESULT hr = S_OK;

	//m_szName = dynamic_cast<TCHAR*>(fileName);
	TCHAR szFileName[MAX_PATH] = { 0, };
	_stprintf_s(szFileName, _T("%s%s"), m_szPath.c_str(), fileName);
	m_szName = fileName;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;

	//hr = D3DX11CreateShaderResourceViewFromFile(pDevice, szFileName, &loadInfo, NULL, &m_pTextureRV, NULL);

	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, szFileName, NULL, NULL, &m_TextureSRV, NULL)))
	{
		return hr;
	}
	return hr;
}

bool	ATexture::Apply(ID3D11DeviceContext*    pImmediateContext)
{
	if (!m_TextureSRV)
	{
		return false;
	}
	pImmediateContext->PSSetShaderResources(0, 1, &m_TextureSRV);
	return true;
}

void ATexture::SetPath(const TCHAR* pPath)
{
	m_szPath = pPath;
}


ATexture::ATexture()
{
	//m_szName = 0;
}


ATexture::~ATexture()
{

}

INT			ATextureMgr::Add(ID3D11Device*	 pDevice, const TCHAR *pFileName)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		//D3DX11_IMAGE_FILE_FORMAT 
		if (_tcsicmp(Ext, _T(".tga")) == 0)
		{
			_tcscpy(Ext, _T(".bmp"));
		}
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (auto it = m_TextureList.begin(); it != m_TextureList.end(); it++)
		{
			ATexture *pPoint = (ATexture *)(*it).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*it).first;
			}
		}
	}
	ATexture *pPoint = new ATexture;

	TCHAR szPath[MAX_PATH] = { 0, };
	_stprintf_s(szPath, _T("%s%s"), Drive, Dir);
	pPoint->SetPath(szPath);

	if (FAILED(pPoint->Load(pDevice, szFileName)))
	{
		return 0;
	}
	m_TextureList.insert(make_pair(++m_iIndex, pPoint));
	return m_iIndex;
}


INT			ATextureMgr::Add(ID3D11Device*	 pDevice, const TCHAR *pFileName, const TCHAR* szPath)
{
	HRESULT hr;
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	_tsplitpath(pFileName, Drive, Dir, FName, Ext);
	Ext[4] = 0;
	memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
	if (_tcsicmp(Ext, _T(".tga")) == 0)
	{
		_tcscpy(Ext, _T(".dds"));
	}
	_stprintf_s(szFileName, _T("%s%s"), FName, Ext);


	for (auto it = m_TextureList.begin(); it != m_TextureList.end(); it++)
	{
		ATexture* pTexture = (ATexture*)(*it).second;
		if (!_tcsicmp(pTexture->m_szName.c_str(), szFileName ))
		{
			return (*it).first;
		}
	}

	ATexture* pLoadTexture;
	pLoadTexture = new ATexture;

	//TCHAR szPath[MAX_PATH] = { 0, };
	pLoadTexture->SetPath(szPath);

	if (FAILED(hr = pLoadTexture->Load(pDevice, szFileName)))
	{
		return -1;
	}


	m_TextureList.insert(make_pair(++m_iIndex,pLoadTexture));
	return m_iIndex;

}

ATexture*	ATextureMgr::GetPtr(INT index)
{
	
	auto itor = m_TextureList.find(index);
	if (itor == m_TextureList.end()) return NULL;

	ATexture *pPoint = (*itor).second;
	return pPoint;
}


ATextureMgr::ATextureMgr()
{
	m_iIndex = 0;
	m_TextureList.clear();
}
ATextureMgr::~ATextureMgr()
{

}