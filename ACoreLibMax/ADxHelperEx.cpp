#include "ADxHelperEx.h"


namespace DX
{
	void ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext)
	{
		// Unbind all objects from the immediate context
		if (pd3dDeviceContext == NULL) return;

		ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		ID3D11DepthStencilView* pDSV = NULL;
		ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

		// Shaders
		pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->HSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->DSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

		// IA clear
		pd3dDeviceContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
		pd3dDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		pd3dDeviceContext->IASetInputLayout(NULL);

		// Constant buffers
		pd3dDeviceContext->VSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->HSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->DSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->GSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->PSSetConstantBuffers(0, 14, pBuffers);

		// Resources
		pd3dDeviceContext->VSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->HSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->DSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->GSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->PSSetShaderResources(0, 16, pSRVs);

		// Samplers
		pd3dDeviceContext->VSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->HSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->DSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->GSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->PSSetSamplers(0, 16, pSamplers);

		// Render targets
		pd3dDeviceContext->OMSetRenderTargets(8, pRTVs, pDSV);

		// States
		FLOAT blendFactor[4] = { 0,0,0,0 };
		pd3dDeviceContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
		pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
		pd3dDeviceContext->RSSetState(NULL);
	}


	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath)
	{
		HRESULT hr = S_OK;
		if (strFilePath == NULL) return nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL,
			NULL, &pSRV, NULL)))
		{
			return nullptr;
		}
		return pSRV;
	}
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  g_pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}

		bd.ByteWidth = iVertexSize * iNumVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		if (vertices != NULL)
		{
			g_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
		}
		else
		{
			g_pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer);
		}
		return pBuffer;
	}


	ID3D11Buffer*  CreateIndexBuffer(ID3D11Device*  g_pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iSize * iNumIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		InitData.pSysMem = indices;
		if (indices != NULL)
		{
			g_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);

		}
		else
		{
			g_pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer);

		}
		return pBuffer;
	}

	ID3D11Buffer*  CreateConstantBuffer(ID3D11Device*   g_pd3dDevice, void *data, UINT iNumIndex, UINT iSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iSize * iNumIndex;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		InitData.pSysMem = data;
		if (data != NULL)
		{
			g_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
		}
		else
		{
			g_pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer);
		}
		return pBuffer;
	}


	ID3D11InputLayout*	CreateInputlayout(ComPtr<ID3D11Device>  g_pd3dDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements)
	{
		HRESULT hr = S_OK;
		ID3D11InputLayout* pInutLayout = nullptr;
		if (FAILED(hr = g_pd3dDevice->CreateInputLayout(layout, numElements, lpData, dwSize, &pInutLayout)))
		{
			return nullptr;
		}
		return pInutLayout;
	}

	ID3D11GeometryShader*  LoadGeometryShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData,
		ID3DBlob** ppBlobOut,
		const char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11GeometryShader* pGeometryShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "GS", "gs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "gs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateGeometryShader(lpData, dwSize, NULL, &pGeometryShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		//D3DCreateBlob(dwSize, ppBlobOut);
		return pGeometryShader;
	}

	ID3D11VertexShader* LoadVertexShaderFile(ComPtr<ID3D11Device>   g_pd3dDevice,
		const void* pShaderFileData,
		ID3DBlob** ppBlobOut,
		const char *pFuntionName,
		bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11VertexShader* pVertexShader;
		ID3DBlob* pBlob = NULL;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "VS", "vs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "vs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			pBlob = *ppBlobOut;
			if (pBlob == nullptr) return nullptr;
			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}

		if (FAILED(hr = g_pd3dDevice->CreateVertexShader(lpData, dwSize, NULL, &pVertexShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr) {
			pBlob->Release();
		}
		else {
			*ppBlobOut = pBlob;
		}
		return pVertexShader;
	};


	ID3D11PixelShader*  LoadPixelShaderFile(ComPtr<ID3D11Device>  g_pd3dDevice,
		const void* pShaderFileData,
		const char *pFuntionName,
		bool bBinary,
		ID3DBlob** pRetBlob)
	{
		HRESULT hr = S_OK;
		ID3D11PixelShader* pPixelShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;
		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "PS", "ps_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "ps_5_0", &pBlob)))
				{
					return nullptr;
				}
			}

			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = g_pd3dDevice->CreatePixelShader(lpData, dwSize, NULL, &pPixelShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (pRetBlob == nullptr)
		{
			pBlob->Release();
		}
		else
		{
			*pRetBlob = pBlob;
		}
		return pPixelShader;
	}

	ID3D11HullShader* LoadHullShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData, ID3DBlob** ppBlobOut, char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11HullShader* pHullShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "HS", "hs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "hs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateHullShader(lpData, dwSize, NULL, &pHullShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		return pHullShader;
	};
	ID3D11DomainShader*  LoadDomainShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData, ID3DBlob** ppBlobOut, char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11DomainShader* pDomainShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "DS", "ds_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "ds_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateDomainShader(lpData, dwSize, NULL, &pDomainShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		return pDomainShader;
	};
	ID3D11ComputeShader*  LoadComputeShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData, ID3DBlob** ppBlobOut, char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11ComputeShader* pComputeShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "CS", "cs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "cs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateComputeShader(lpData, dwSize, NULL, &pComputeShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		return pComputeShader;
	};


	HRESULT CompileShaderFromFile(TCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

		ID3DBlob* pErrorBlob=NULL;
		hr= D3DX11CompileFromFileW(szFileName, NULL, NULL, szEntryPoint, szShaderModel,	dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	void ADxObject::PreRender(ComPtr<ID3D11DeviceContext>     pContext, UINT iVertexSize)
	{
		pContext->IASetInputLayout(g_pInputlayout.Get());
		UINT stride = iVertexSize;
		UINT offset = 0;
		if (iVertexSize == 0) stride = m_iVertexSize;

		//슬록에 버퍼갯수
		pContext->IASetVertexBuffers(0, 1, g_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(g_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		pContext->VSSetConstantBuffers(0, 1, g_pConstantBuffer.GetAddressOf());
		pContext->VSSetShader(g_pVertexShader.Get(), NULL, 0);
		pContext->PSSetShader(g_pPixelShader.Get(), NULL, 0);
		pContext->GSSetShader(g_pGeometryShader.Get(), NULL, 0);
		pContext->HSSetShader(g_pHullShader.Get(), NULL, 0);
		pContext->DSSetShader(g_pDomainShader.Get(), NULL, 0);
		
		pContext->PSSetShaderResources(0, 1, g_pTextureSRV.GetAddressOf());
	}

	void ADxObject::PostRender(ComPtr<ID3D11DeviceContext>     pContext, UINT iCount)
	{
		//if (iCount == 0) iCount = m_iNumIndex;
		//pContext->DrawIndexed(iCount, 0, 0);

		if (iCount == 0) iCount = m_iNumIndex;
		if (iCount != 0)
			pContext->DrawIndexed(iCount, 0, 0);
		else
			pContext->Draw(m_iNumVertex, 0);
	}

	bool ADxObject::Render(ComPtr<ID3D11DeviceContext>    pContext, UINT iVertexSize, UINT iCount)
	{
		PreRender(pContext, iVertexSize);
		PostRender(pContext, iCount);
		return true;
	}

	ADxObject::ADxObject()
	{
		g_pVertexBuffer = nullptr;
		g_pIndexBuffer = nullptr;
		g_pConstantBuffer = nullptr;
		g_pVertexShader = nullptr;
		g_pPixelShader = nullptr;
		//	g_pGeometryShader = nullptr;
		g_pInputlayout = nullptr;
		g_pVSBlob = nullptr;
		m_iPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_iCullMode = 1;
		m_iSamplerMode = 0;
		m_iNumVertex = 0;
		m_iNumIndex = 0;
		m_iVertexSize = 0;
		m_iBeginVB = 0;
		m_iBeginIB = 0;
		m_iIndexSize = sizeof(DWORD);
	}
	ADxObject::~ADxObject() {	}

}