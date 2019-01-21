#include "ASkinObj.h"
#include "ATexture.h"

int SkinMesh::IsEqulVertexList(vector<PNCT5_VERTEX>&  VertexArray, PNCT5_VERTEX& Vertex)
{
	for (int iVertex = 0; iVertex < VertexArray.size(); iVertex++)
	{
		if (VertexArray[iVertex] == Vertex)
		{
			return iVertex;
		}
	}
	return -1;
}

int SkinMesh::SetUniqueBuffer(vector <ATri_PNCT5>& TriList, int iMtrl, int iStartTri)
{
	int iNumFaces = TriList.size();
	m_VertexArray.reserve(iNumFaces * 3);
	m_IndexArray.reserve(iNumFaces * 3);

	ATri_PNCT5  triSame(iMtrl);
	if (iMtrl >= 0)
	{
		iNumFaces = count_if(TriList.begin(), TriList.end(), IsTriangleSame5(triSame));
	}

	for (int iFace = 0; iFace < iNumFaces; iFace++)
	{
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			int iPosReturn = IsEqulVertexList(m_VertexArray, TriList[iStartTri + iFace].vVertex[iCnt]);
			if (iPosReturn < 0)
			{
				m_VertexArray.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
				iPosReturn = m_VertexArray.size() - 1;
			}
			m_IndexArray.push_back(iPosReturn);
		}
	}

	return iNumFaces;
}
bool ASkinObj::Release()
{
	return true;
}
void ASkinObj::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	D3DXMatrixTranspose(&cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&cbData.matView, &m_matView);
	D3DXMatrixTranspose(&cbData.matProj, &m_matProj);
}

bool ASkinObj::Convert(const TCHAR* strPathName)
{
	return true;
}


bool ASkinObj::Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	if (szLoadName)
	{
		_tsplitpath(szLoadName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
	}
	memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
	_stprintf_s(szFileName, _T("%s%s"), Drive, Dir);

	FILE	*fp;
	_tfopen_s(&fp, szLoadName, _T("rb"));
	_ASSERT(fp != NULL);
	fread(&m_Scene, sizeof(AScene), 1, fp);

	TCHAR szBuffer[128] = { 0, };
	size_t convertedChars = 0;

	for (int iter = 0; iter < m_Scene.iNumMesh; iter++)
	{
		AMesh* pMesh = new AMesh();
		SkinMesh* pData = new SkinMesh();

		TCHAR szName[256] = { 0,0 };
		TCHAR szParentName[256] = { 0,0 };

		// 노드 이름
		int iLength = 0;
		fread(&iLength, sizeof(int), 1, fp);
		fread(szName, sizeof(TCHAR)*iLength, 1, fp);

		pMesh->m_strNodeName = szName;
		// 노드 부모 이름
		fread(&iLength, sizeof(int), 1, fp);
		if (iLength > 0)
		{
			fread(szParentName, sizeof(TCHAR)*iLength, 1, fp);
			pMesh->m_strParentName = szParentName;
		}
		// 월드 행렬
		fread(&pMesh->m_matWorld, sizeof(D3DXMATRIX), 1, fp);
		//m_matWorld = pMesh->m_matWorld;

		fread(&pMesh->m_iNumMtrl, sizeof(int), 1, fp);
		if (pMesh->m_iNumMtrl <= 0)
		{
			LoadMesh(fp, pMesh, pData, szFileName);
			//SAFE_NEW_ARRAY(pMesh->m_pDrawVertex, PNCT5_VERTEX, pMesh->m_iNumVertexs );
		}
		else
		{
			for (int iter = 0; iter < pMesh->m_iNumMtrl; iter++)
			{
				AMesh*  pSubMesh = new AMesh();
				SkinMesh*  pSubData = new SkinMesh();
				if (LoadMesh(fp, pSubMesh, pSubData, szFileName))
				{
					SAFE_NEW_ARRAY(pSubData->m_pDrawVertex, PNCT5_VERTEX, pSubMesh->m_iNumVertexs);
					pMesh->m_pSubMesh.push_back(pSubMesh);
					pData->m_pSubMesh.push_back(pSubData);
				}
			}
		}
		m_pMesh.push_back(pMesh);
		m_pSkinMeshList.push_back(pData);
	}

	// 드레스 포즈의 각 바이패드 행렬 로드
	int iNumMatrix;
	fread(&iNumMatrix, sizeof(int), 1, fp);
	m_matBipedList.resize(iNumMatrix);
	for (int iMat = 0; iMat < iNumMatrix; iMat++)
	{
		fread(&m_matBipedList[iMat], sizeof(D3DXMATRIX), 1, fp);
	}

	fclose(fp);
#if 0
	for (int i = 0; i < m_pData[0]->m_VertexArray.size(); i++)
	{
		D3DXVec3TransformCoord(&m_pData[0]->m_VertexArray[i].p, &m_pData[0]->m_VertexArray[i].p,
			&m_matBipedList[m_pData[0]->m_VertexArray[i].i0.x]);
	}
#endif

	m_dxObj.m_iNumVertex = m_iMaxVertex;
	m_dxObj.m_iNumIndex = m_iMaxIndex;
	m_dxObj.m_iVertexSize = sizeof(PNCT5_VERTEX);

	if (!bThread && !Set(pDevice, pLoadShaderFile))
	{
		return false;
	}

	return true;
}



bool ASkinObj::LoadMesh(FILE	*fp, AMesh* pMesh, SkinMesh* pData, const TCHAR* szLoadFileName)
{
	char szBuffer[128] = { 0, };
	size_t convertedChars = 0;

	fread(&pMesh->m_iNumVertexs, sizeof(int), 1, fp);
	fread(&pMesh->m_iNumFace, sizeof(int), 1, fp);
	fread(&pMesh->m_iNumTex, sizeof(int), 1, fp);

	pData->m_VertexArray.resize(pMesh->m_iNumVertexs);
	pData->m_IndexArray.resize(pMesh->m_iNumFace * 3);

	fread(&pData->m_VertexArray.at(0), sizeof(PNCT5_VERTEX) * pMesh->m_iNumVertexs, 1, fp);
	fread(&pData->m_IndexArray.at(0), sizeof(unsigned int) * pMesh->m_iNumFace * 3, 1, fp);

	m_iMaxVertex += pData->m_VertexArray.size();
	m_iMaxIndex += pData->m_IndexArray.size();

	for (int itex = 0; itex < pMesh->m_iNumTex; itex++)
	{
		int iLen = _tcslen(pMesh->m_szTexName[itex]);
		fread(&iLen, sizeof(int), 1, fp);
		fread(&pMesh->m_iTexType[itex], sizeof(int), 1, fp);
		ZeroMemory(pMesh->m_szTexName[itex], sizeof(TCHAR) * 30);
		fread(pMesh->m_szTexName[itex], sizeof(TCHAR) * iLen, 1, fp);

		TCHAR szFileName[MAX_PATH];
		TCHAR Drive[MAX_PATH];
		TCHAR Dir[MAX_PATH];
		TCHAR FName[MAX_PATH];
		TCHAR Ext[MAX_PATH];
		if (szLoadFileName)
		{
			_tsplitpath(szLoadFileName, Drive, Dir, FName, Ext);
			Ext[4] = 0;
			memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		}
		T_STR loadFile = Drive;
		loadFile += Dir;
		loadFile += pMesh->m_szTexName[itex];
		// 디퓨즈 텍스쳐
		if (pMesh->m_iTexType[itex] == ID_BASIS_DI)
		{
			pMesh->m_iDiffuseTex = I_TEXTURE.Add(g_pd3dDevice.Get(), loadFile.c_str());
		}
	}
	return true;
}


bool ASkinObj::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 96, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxObj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
		m_dxObj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxObj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));

	return true;
}

////HRESULT ASkinObj::CreateInstance(ID3D11Device* m_pd3dDevice,UINT iNumInstance)
////{
////	HRESULT hr = S_OK;
////	m_pInstance.resize(iNumInstance);
////
////	for (int iSt = 0; iSt < m_pInstance.size(); iSt++)
////	{
////		D3DXMatrixIdentity(&m_pInstance[iSt].matWorld);
////		float randTemp = rand() % 30 -20;
////		m_pInstance[iSt].matWorld._41 = randTemp;
////		m_pInstance[iSt].matWorld._42 = randTemp;
////		m_pInstance[iSt].matWorld._43 = randTemp;
////
////
////		D3DXMatrixTranspose(&m_pInstance[iSt].matWorld, &m_pInstance[iSt].matWorld);
////	}
////	m_pVBInstance.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_pInstance.at(0), m_pInstance.size(), sizeof(AInstatnce)));
////
////	return hr;
////}
//


bool	ASkinObj::RenderInstancing(ID3D11DeviceContext* pContext)
{

	PreRender(pContext);


	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject];
		
		D3DXMATRIX matWorld = pMesh->m_matCalculation * (this->m_matWorld);
		D3DXMatrixTranspose(&cbData.matWorld, &matWorld);
		UpdateConstantBuffer(pContext, this);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub];
				if (pSubMesh->m_iNumFace < 1) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
				//pContext->DrawIndexed(pSubMesh->m_dxObj.m_iNumIndex,pSubMesh->m_dxObj.m_iBeginIB, pSubMesh->m_dxObj.m_iBeginVB);
				//pContext->DrawIndexedInstanced(pSubMesh->m_dxObj.m_iNumIndex, m_pInstance.size(), pMesh->m_dxObj.m_iBeginIB, pMesh->m_dxObj.m_iBeginVB, 1);

			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;
			pContext->PSSetShaderResources(0, 1, pMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
		//	pContext->DrawIndexed(pMesh->m_dxObj.m_iNumIndex,	pMesh->m_dxObj.m_iBeginIB,				pMesh->m_dxObj.m_iBeginVB);
			//pContext->DrawIndexedInstanced(m_dxObj.m_iNumIndex, m_pInstance.size(), pMesh->m_dxObj.m_iBeginIB, pMesh->m_dxObj.m_iBeginVB, 1);

		}
	}


	{
		ID3D11Buffer* vb[2] = { m_dxObj.g_pVertexBuffer.Get(),	m_pVBInstance.Get() };
		UINT stride[2] = { sizeof(PNCT5_VERTEX), sizeof(AInstatnce) };
		UINT offset[2] = { 0, 0 };
		pContext->IASetVertexBuffers(0, 2, vb, stride, offset);
		pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
	}


	pContext->DrawIndexedInstanced(m_dxObj.m_iNumIndex, m_pInstance.size(), m_dxObj.m_iBeginIB, m_dxObj.m_iBeginVB, 0);
	//pContext->DrawIndexed(m_dxObj.m_iNumIndex, 0, 0);	//1개만 뿌려진다
	return true;
}

bool ASkinObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}


bool ASkinObj::Draw(ID3D11DeviceContext*    pContext, AModel* pParent)
{

	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject];
		D3DXMATRIX matWorld = pMesh->m_matCalculation * pParent->m_matWorld;
		D3DXMatrixTranspose(&cbData.matWorld, &matWorld);
		UpdateConstantBuffer(pContext, pParent);
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub];
				if (pSubMesh->m_iNumFace < 1) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_dxObj.m_iNumIndex,
					pSubMesh->m_dxObj.m_iBeginIB,					pSubMesh->m_dxObj.m_iBeginVB);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;
			pContext->PSSetShaderResources(0, 1, pMesh->m_dxObj.g_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_dxObj.m_iNumIndex,
				pMesh->m_dxObj.m_iBeginIB,
				pMesh->m_dxObj.m_iBeginVB);
		}
	}

	return true;
}


bool		ASkinObj::LoadTexture(ID3D11Device* device, const TCHAR* fileName)
{

	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		AMesh* pMesh = m_pMesh[dwObject];

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				if (pMesh->m_pSubMesh[dwSub]->m_iDiffuseTex < 0) continue;
				pMesh->m_pSubMesh[dwSub]->m_pTextureSRV = I_TEXTURE.GetPtr(pMesh->m_pSubMesh[dwSub]->m_iDiffuseTex)->m_TextureSRV;
			}
		}
		else
		{
			if (pMesh->m_iDiffuseTex < 0) continue;
			pMesh->m_pTextureSRV = I_TEXTURE.GetPtr(pMesh->m_iDiffuseTex)->m_TextureSRV;
		}
	}

	////m_GeomObjList[asd]->m_iMtrlRef;
	//m_MtrlList;
	//m_MtrlList[0].m_TexMaps[0].m_strTextureName;
	//m_dxObj.g_pTextureSRV = I_TEXTURE.GetPtr(m_MtrlList[0].m_TexMaps[0].m_dwIndex)->m_TextureSRV;

	return true;
}

bool ASkinObj::UpdateBuffer()
{
	if (!CombineBuffer(m_dxObj.g_pVertexBuffer.Get(), m_dxObj.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}


bool		ASkinObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
{
	size_t dstOffset = 0;
	size_t vbOffset = 0;
	size_t dstibOffset = 0;
	size_t ibOffset = 0;
	{
		for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
		{
			auto pMesh = m_pMesh[dwObject];
			auto pData = m_pSkinMeshList[dwObject];
			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
				{
					auto pSubMesh = pMesh->m_pSubMesh[dwSub];
					auto pSubData = pData->m_pSubMesh[dwSub];
					if (pData->m_VertexArray.size() < 3) continue;

					pSubMesh->m_dxObj.m_iNumVertex = pData->m_VertexArray.size();
					pSubMesh->m_dxObj.m_iVertexSize = m_dxObj.m_iVertexSize;

					pSubMesh->m_dxObj.m_BoxVB.left = dstOffset;
					pSubMesh->m_dxObj.m_BoxVB.right = dstOffset + pSubMesh->m_dxObj.m_iNumVertex*m_dxObj.m_iVertexSize;
					pSubMesh->m_dxObj.m_BoxVB.top = 0; pSubMesh->m_dxObj.m_BoxVB.bottom = 1;
					pSubMesh->m_dxObj.m_BoxVB.front = 0; pSubMesh->m_dxObj.m_BoxVB.back = 1;

					g_pImmediateContext->UpdateSubresource(
						pVB, 0,
						&pSubMesh->m_dxObj.m_BoxVB,
						(uint8_t*)&pSubData->m_VertexArray.at(0),
						0, 0);


					/*g_pImmediateContext->CopySubresourceRegion(
					m_dxObj.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
					(void*)&pSubMesh->m_VertexArray.at(0),
					0, &pSubMesh->m_dxObj.m_BoxVB);*/

					pSubMesh->m_dxObj.m_iBeginVB = vbOffset;
					vbOffset += pSubMesh->m_dxObj.m_iNumVertex;
					dstOffset = pSubMesh->m_dxObj.m_BoxVB.right;

					pSubMesh->m_dxObj.m_iNumIndex = pSubData->m_IndexArray.size();
					pSubMesh->m_dxObj.m_BoxIB.left = dstibOffset;
					pSubMesh->m_dxObj.m_BoxIB.right = dstibOffset + pSubMesh->m_dxObj.m_iNumIndex * sizeof(DWORD);
					pSubMesh->m_dxObj.m_BoxIB.top = 0;		pSubMesh->m_dxObj.m_BoxIB.bottom = 1;
					pSubMesh->m_dxObj.m_BoxIB.front = 0;	pSubMesh->m_dxObj.m_BoxIB.back = 1;

					g_pImmediateContext->UpdateSubresource(pIB, 0,
						&pSubMesh->m_dxObj.m_BoxIB, (void*)&pSubData->m_IndexArray.at(0), 0, 0);

					pSubMesh->m_dxObj.m_iBeginIB = ibOffset;
					ibOffset += pSubMesh->m_dxObj.m_iNumIndex;
					dstibOffset = pSubMesh->m_dxObj.m_BoxIB.right;

					//texture
					if (pSubMesh->m_iDiffuseTex < 0) continue;
					pSubMesh->m_dxObj.g_pTextureSRV = I_TEXTURE.GetPtr(pSubMesh->m_iDiffuseTex)->m_TextureSRV;
				}
			}
			else
			{
				if (pData->m_VertexArray.size() < 3) continue;

				pMesh->m_dxObj.m_iNumVertex = pData->m_VertexArray.size();
				pMesh->m_dxObj.m_iVertexSize = m_dxObj.m_iVertexSize;

				pMesh->m_dxObj.m_BoxVB.left = dstOffset;
				pMesh->m_dxObj.m_BoxVB.right = dstOffset + pMesh->m_dxObj.m_iNumVertex*m_dxObj.m_iVertexSize;
				pMesh->m_dxObj.m_BoxVB.top = 0; pMesh->m_dxObj.m_BoxVB.bottom = 1;
				pMesh->m_dxObj.m_BoxVB.front = 0; pMesh->m_dxObj.m_BoxVB.back = 1;

				g_pImmediateContext->UpdateSubresource(pVB, 0, &pMesh->m_dxObj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);

				pMesh->m_dxObj.m_iBeginVB = vbOffset;
				vbOffset += pMesh->m_dxObj.m_iNumVertex;
				dstOffset = pMesh->m_dxObj.m_BoxVB.right;

				pMesh->m_dxObj.m_iNumIndex = pData->m_IndexArray.size();
				pMesh->m_dxObj.m_BoxIB.left = dstibOffset;
				pMesh->m_dxObj.m_BoxIB.right = dstibOffset + pMesh->m_dxObj.m_iNumIndex * sizeof(DWORD);
				pMesh->m_dxObj.m_BoxIB.top = 0; pMesh->m_dxObj.m_BoxIB.bottom = 1;
				pMesh->m_dxObj.m_BoxIB.front = 0; pMesh->m_dxObj.m_BoxIB.back = 1;

				g_pImmediateContext->UpdateSubresource(pIB, 0, &pMesh->m_dxObj.m_BoxIB, (void*)&pData->m_IndexArray.at(0), 0, 0);
				pMesh->m_dxObj.m_iBeginIB = ibOffset;
				ibOffset += pMesh->m_dxObj.m_iNumIndex;
				dstibOffset = pMesh->m_dxObj.m_BoxIB.right;

				//texture
				if (pMesh->m_iDiffuseTex <= 0) continue;
				pMesh->m_dxObj.g_pTextureSRV = I_TEXTURE.GetPtr(pMesh->m_iDiffuseTex)->m_TextureSRV;
			}
		}
	}
	return true;

}

ASkinObj::ASkinObj()
{
}


ASkinObj::~ASkinObj()
{
}
