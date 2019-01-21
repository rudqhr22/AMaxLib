#include "AModel.h"
#include "ATexture.h"

vector<AInstatnce> AModel::m_pInstance;
ComPtr<ID3D11Buffer> AModel::m_pVBInstance=0;


HRESULT AModel::CreateInstance(ID3D11Device* m_pd3dDevice,UINT iNumInstance)
{
	HRESULT hr = S_OK;
	m_pInstance.resize(iNumInstance);

	for (int iSt = 0; iSt < m_pInstance.size(); iSt++)
	{
		D3DXMatrixIdentity(&m_pInstance[iSt].matWorld);
		float randTemp = rand() % 30 -20;
		m_pInstance[iSt].matWorld._41 = randTemp;
		m_pInstance[iSt].matWorld._42 = randTemp;
		m_pInstance[iSt].matWorld._43 = randTemp;


		D3DXMatrixTranspose(&m_pInstance[iSt].matWorld, &m_pInstance[iSt].matWorld);
	}
	m_pVBInstance.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_pInstance.at(0), m_pInstance.size(), sizeof(AInstatnce)));

	return hr;
}





bool AModel::Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile,
	bool bThread)
{
	return true;
}


void AModel::SetCollisionData(D3DXMATRIX& matWorld)
{
	m_matWorld = matWorld;
	m_vCenter.x = m_matWorld._41;
	m_vCenter.y = m_matWorld._42;
	m_vCenter.z = m_matWorld._43;

	m_vMove = m_vCenter - m_vPrevCenter;
	m_vPrevCenter = m_vCenter;

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;

	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	m_vSide.x = m_matWorld._11;
	m_vSide.y = m_matWorld._12;
	m_vSide.z = m_matWorld._13;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	
	m_CollsionBox.vCenter = m_vCenter;
	//// w= 1
	D3DXVECTOR3 vMax = m_InitBox.vMax;
	D3DXVECTOR3 vMin = m_InitBox.vMin;
	D3DXVec3TransformCoord(&m_CollsionBox.vMax, &vMax, &m_matWorld);
	D3DXVec3TransformCoord(&m_CollsionBox.vMin, &vMin, &m_matWorld);
	//// w= 0 * _41
	D3DXVec3TransformNormal(&m_CollsionBox.vAxis[0], &m_InitBox.vAxis[0], &m_matWorld);
	D3DXVec3TransformNormal(&m_CollsionBox.vAxis[1], &m_InitBox.vAxis[1], &m_matWorld);
	D3DXVec3TransformNormal(&m_CollsionBox.vAxis[2], &m_InitBox.vAxis[2], &m_matWorld);

	D3DXVec3Normalize(&m_CollsionBox.vAxis[0], &m_CollsionBox.vAxis[0]);
	D3DXVec3Normalize(&m_CollsionBox.vAxis[1], &m_CollsionBox.vAxis[1]);
	D3DXVec3Normalize(&m_CollsionBox.vAxis[2], &m_CollsionBox.vAxis[2]);

	m_CollsionBox.fExtent[0] = (m_CollsionBox.vMax.x - m_CollsionBox.vMin.x) * 0.5f;
	m_CollsionBox.fExtent[1] = (m_CollsionBox.vMax.y - m_CollsionBox.vMin.y) * 0.5f;
	m_CollsionBox.fExtent[2] = (m_CollsionBox.vMax.z - m_CollsionBox.vMin.z) * 0.5f;

	//m_vPos = D3DXVECTOR3(m_vCenter.x, m_vCenter.y, m_vCenter.z);

	//m_Sphere.vCenter = m_vCenter;
	//m_Sphere.fRadius = D3DXVec3Length(&(m_CollsionBox.vMax - m_CollsionBox.vCenter));
}

void AModel::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
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


	//D3DXMatrixTranspose 전치시키고 넘기는 이유
	//쉐이더는 mul(p,g_matworld) = mul함수가 p(Dot)g_matworld 내적으로 곱해지기 때문에.
	//쉐이더는 렬 행 순이라 순서라 전치 시켜서 순서에 맞게 한다.
	//그러므로 내적처리(곱셈)가 가능해진다.

	D3DXMatrixTranspose(&cbData.matWorld, &m_matWorld);		//전치시키는 함수
	D3DXMatrixTranspose(&cbData.matView, &m_matView);		//전치시키는 함수
	D3DXMatrixTranspose(&cbData.matProj, &m_matProj);		//전치시키는 함수

	SetCollisionData(m_matWorld);
}



bool		AModel::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	m_dxObj.g_pInputlayout.Attach(DX::CreateInputlayout(
		m_pd3dDevice.Get(),
		m_dxObj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxObj.g_pVSBlob.Get()->GetBufferPointer(),
		layout,
		numElements));



	return true;
}

bool		AModel::Set(ID3D11Device* device, const TCHAR* shaderName, const TCHAR* fileName,bool pass )	//const TCHAR* fileName,  float l , float t , float r, float b )
{
	m_pd3dDevice = device;

	if (!CompileShader(device, shaderName)) {
		MessageBox(0, _T("CompileShader 실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	if (!SetInputLayout() ) 
	{
		MessageBox(0, _T("SetInputLayout 실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	if (!CreateVertexData()) {
		MessageBox(0, _T("CreateVertexData 실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (!CreateIndexData()) {
		MessageBox(0, _T("CreateIndexData 실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (!CreateVertexBuffer()) 
	{
		MessageBox(0, _T("CreateVertexBuffer 실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	if (!CreateIndexBuffer()) {
		MessageBox(0, _T("CreateIndexBuffer 실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	if (!CreateConstantBuffer()) {
		MessageBox(0, _T("CreateConstantBuffer 실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	if (!UpdateBuffer()) {
		MessageBox(0, _T("UpdateBuffer 실패"), _T("Fatal error"), MB_OK);
		return false;
	}


	if (!CreatePrimitiveType()) {
		return false;
	}

	if (pass == true) { return true; }

	if (!LoadTexture(m_pd3dDevice.Get(), fileName)) {
		MessageBox(0, _T("LoadTexture 실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	//LoadTexture(m_pd3dDevice.Get(), fileName);
	//if (!LoadTexture(m_pd3dDevice.Get(),fileName)) 
	//{
	//	MessageBox(0, _T("LoadTexture 실패"), _T("Fatal error"), MB_OK);
	//	return false;
	//}


	SetCollisionData(m_matWorld);
	return Init();

	//return true;
}

bool		AModel::UpdateBuffer() { return true; };

bool		AModel::CreateVertexData() { return true; };

bool		AModel::CreateIndexData() { return true; };

bool		AModel::CreateVertexBuffer() 
{
	if (m_dxObj.m_iNumVertex <= 0) return true;
	void** pData = nullptr;

	if (m_PNCTList.size() > 0) pData = (void**)&m_PNCTList.at(0);

	m_dxObj.g_pVertexBuffer.Attach(
		DX::CreateVertexBuffer(m_pd3dDevice.Get(),
		pData,
		m_dxObj.m_iNumVertex,
		m_dxObj.m_iVertexSize));
	return true;
};

bool		AModel::CreateIndexBuffer() 
{ 
	if (m_dxObj.m_iNumIndex <= 0) return true;

	void** pData = nullptr;
	
	if (m_IndexList.size() > 0) pData = (void**)&m_IndexList.at(0);

	m_dxObj.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice.Get(),
		pData,
		m_dxObj.m_iNumIndex,
		m_dxObj.m_iIndexSize));

	return true; 
};

bool		AModel::CreateConstantBuffer() 
{
	cbData.vColor = D3DXVECTOR4(1, 1, 1, 1);
	m_dxObj.g_pConstantBuffer.Attach(DX::CreateConstantBuffer(m_pd3dDevice.Get(), &cbData, 1, sizeof(VS_CONSTANT_BUFFER)));
	return true; 
};

//디바이스와 픽쉘 쉐이더
bool		AModel::CompileShader(ID3D11Device* device, const TCHAR* fileName)
{
	HRESULT hr;
	//DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	m_dxObj.g_pVertexShader.Attach(DX::LoadVertexShaderFile(device, fileName, m_dxObj.g_pVSBlob.GetAddressOf()));
	if (m_dxObj.g_pVertexShader == NULL) return false;
	

	m_dxObj.g_pPixelShader.Attach(DX::LoadPixelShaderFile(device, fileName));
	if (m_dxObj.g_pPixelShader == NULL) return false;

	m_dxObj.g_pGeometryShader.Attach(DX::LoadGeometryShaderFile(device, fileName, m_dxObj.g_pGSBlob.GetAddressOf()));
	m_dxObj.g_pHullShader.Attach(DX::LoadHullShaderFile(device, fileName, m_dxObj.g_pHSBlob.GetAddressOf()));
	m_dxObj.g_pDomainShader.Attach(DX::LoadDomainShaderFile(device, fileName, m_dxObj.g_pDSBlob.GetAddressOf()));
	m_dxObj.g_pComputeShader.Attach(DX::LoadComputeShaderFile(device, fileName, m_dxObj.g_pCSBlob.GetAddressOf()));
	
	return true;
}

//디바이스와 블록넘 넘길수 있게
//bool		AModel::CreateLayout(ID3D11Device* device) 
//{
//	return true; 
//};


//ID3D11ShaderResourceView*		AModel::LoadTexture(ID3D11Device* device, const TCHAR* fileName)
//{
//		
//	HRESULT hr = S_OK;
//	//m_dxObj.g_pTextureSRV.Attach(DX::CreateShaderResourceView(device, fileName));
//	
//	//I_TEXTURE.Add(device, fileName, m_dxObj.g_pTextureSRV);
//
//	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_dxObj.g_pTextureSRV, NULL))) {
//			return false;
//	}
//
//	return m_dxObj.g_pTextureSRV.Get();
//}


bool		AModel::LoadTexture(ID3D11Device* device, const TCHAR* fileName)
{

	HRESULT hr = S_OK;
	//m_dxObj.g_pTextureSRV.Attach(DX::CreateShaderResourceView(device, fileName));

	//I_TEXTURE.Add(device, fileName, m_dxObj.g_pTextureSRV);

	//if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_dxObj.g_pTextureSRV, NULL))) {
	//	return false;
	//}

	m_dxObj.g_pTextureSRV.Attach(DX::CreateShaderResourceView(device, fileName));

	return true;
}


HRESULT		AModel::TextureInfo(const TCHAR* texture)
{
	HRESULT hr=S_OK;
	//D3DX11_IMAGE_INFO ImageInfo;
	//if (texture == NULL) return S_OK;

	//hr = D3DX11GetImageInfoFromFile(texture, NULL, &ImageInfo, &hr);

	//weidght =ImageInfo.Width;
	//height = ImageInfo.Height;

	//D3DX11_IMAGE_LOAD_INFO LInfo;
	//ZeroMemory(&LInfo, sizeof(LInfo));
	//LInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_BC1_UNORM
	//LInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	//LInfo.pSrcInfo = &ImageInfo;
	//LInfo.Usage = D3D11_USAGE_STAGING;
	
	//ID3D11Resource *pTexture = NULL;
	//hr = D3DX11CreateTextureFromFile(m_pd3dDevice.Get(), texture, &LInfo, NULL, &pTexture, NULL);

	//ID3D11Texture2D *pTexture2D = NULL;
	//hr = pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D);
	//SAFE_RELEASE(pTexture);

	//D3D11_TEXTURE2D_DESC td2d_Desc;
	//pTexture2D->GetDesc(&td2d_Desc);

	//weidght = td2d_Desc.Width;
	//height = td2d_Desc.Height;

	return hr;
}

bool AModel::Init()
{
//	GameObject::Init();

	SetCollisionData(m_matWorld);
	return true;
}
bool AModel::Frame() 
{
	SetCollisionData(m_matWorld);
	return true; 
}
bool AModel::Release() { return true; }

bool	AModel::Render(ID3D11DeviceContext* context)
{


	PreRender(context);
	PostRender(context);
	
	//DXGI_SWAP_CHAIN_DESC Desc;
	//GetSwapChain()->GetDesc(&Desc);
	


	return true;
};

void AModel::UpdateConstantBuffer(ID3D11DeviceContext* pContext, AModel* pParent)
{
	//D3DXMatrixPerspectiveFovLH(&cbData.matProj, (float)D3DX_PI*0.5f, (g_iClientWidth / (float)g_iClientHeight), 1, 100.0f);
	//D3DXMatrixTranspose(&cbData.matProj, &cbData.matProj);

	if (pParent != NULL && pParent->m_dxObj.g_pConstantBuffer != nullptr)
	{
		pContext->UpdateSubresource(pParent->m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &pParent->cbData, 0, 0);
	}
	else
	{
		if (m_dxObj.g_pConstantBuffer != nullptr)
			pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
	}

	//if (m_dxObj.g_pConstantBuffer != nullptr) 
	//{
	//	pContext->UpdateSubresource(m_dxObj.g_pConstantBuffer.Get(), 0, NULL, &cbData, 0, 0);
	//}
	
}

bool	AModel::PreRender(ID3D11DeviceContext* context)
{
	context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_dxObj.m_iPrimitiveType);



	m_dxObj.PreRender(context, m_dxObj.m_iVertexSize);

	return true;
};

bool	AModel::PostRender(ID3D11DeviceContext* context) 
{
	UpdateConstantBuffer(context);
	m_dxObj.PostRender(context, m_dxObj.m_iNumIndex);
	return true;
};

bool		AModel::CreatePrimitiveType()
{
	m_dxObj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return true;
}

AModel::AModel()
{

	m_InitBox.vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_InitBox.vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	//m_InitBox.vMax = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	//m_InitBox.vMin = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);

	cbData.vColor = D3DXVECTOR4(1, 1, 1, 1);
	m_vPrevCenter = m_vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matControlWorld);

	//D3DXMatrixIdentity(&cbData.matWorld);
	//D3DXVECTOR3 Eye(0.0f, 0.0f, -1.0f);
	//D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	//D3DXMatrixLookAtLH(&cbData.matView,&Eye,&At,&Up);
	//D3DXMatrixPerspectiveFovLH(&cbData.matProj,(float)D3DX_PI*0.5f,(g_iClientWidth/ (float)g_iClientHeight),1,100.0f);
	//D3DXMatrixTranspose(&cbData.matWorld, &cbData.matWorld);
	//D3DXMatrixTranspose(&cbData.matView, &cbData.matView);
	//D3DXMatrixTranspose(&cbData.matProj, &cbData.matProj);

}


AModel::~AModel()
{

	

}
