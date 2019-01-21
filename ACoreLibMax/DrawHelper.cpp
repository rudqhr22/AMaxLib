#include "DrawHelper.h"

void	DrawHelper::DrawInit()
{
	//m_pLine = new ALineShape;
	//m_pLine.Set(g_pd3dDevice.Get(), L"../../data/shader/Line.hlsl", 0, true);
	m_pLine.Set(g_pd3dDevice.Get(), L"../../data/shader/Line.hlsl", 0, true);
}

//
//void	DrawHelper::DrawAseLine(AAseObj* pObj, ACamera* pCamera, ID3D11DeviceContext* m_pImmediateContext)
//{
//	for (int i = 0; i < pObj->m_MeshList.size(); i++)
//	{
//		if (pObj->m_MeshList[i]->m_bSelect == true)
//		{
//			m_pLine->SetMatrix(0, pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
//			D3DXVECTOR4 vColor = vColor = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
//
//			D3DXVECTOR3 vPoint[16];
//			vPoint[0] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[1] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[2] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//			vPoint[3] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//
//
//			vPoint[4] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[5] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[6] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//			vPoint[7] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//
//			vPoint[8] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[9] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//			vPoint[10] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[11] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMin.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//
//
//			vPoint[12] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[13] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMax.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//			vPoint[14] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMax.z);
//			vPoint[15] = D3DXVECTOR3(pObj->m_MeshList[i]->m_Box.vMax.x, pObj->m_MeshList[i]->m_Box.vMin.y, pObj->m_MeshList[i]->m_Box.vMin.z);
//
//
//
//			m_pLine->Draw(m_pImmediateContext, vPoint[0], vPoint[1], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[1], vPoint[3], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[2], vPoint[3], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[0], vPoint[2], vColor);
//
//			m_pLine->Draw(m_pImmediateContext, vPoint[4], vPoint[5], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[5], vPoint[7], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[6], vPoint[7], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[4], vPoint[6], vColor);
//
//
//			m_pLine->Draw(m_pImmediateContext, vPoint[8], vPoint[9], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[9], vPoint[11], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[10], vPoint[11], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[8], vPoint[10], vColor);
//
//			m_pLine->Draw(m_pImmediateContext, vPoint[12], vPoint[13], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[13], vPoint[15], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[14], vPoint[15], vColor);
//			m_pLine->Draw(m_pImmediateContext, vPoint[12], vPoint[14], vColor);
//
//		}
//	}
//
//
//}
//


void	DrawHelper::BoxLine(A_Box* pObj, ID3D11DeviceContext* m_pImmediateContext, ACamera* pCamera)
{
	m_pLine.SetMatrix(0, pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
	D3DXVECTOR4 vColor = vColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	D3DXVECTOR3 vPoint[16];
	vPoint[0] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMax.y,  pObj->vMax.z);
	vPoint[1] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMax.y,  pObj->vMax.z);
	vPoint[2] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMax.y,  pObj->vMin.z);
	vPoint[3] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMax.y,  pObj->vMin.z);


	vPoint[4] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMin.y,  pObj->vMax.z);
	vPoint[5] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMin.y,  pObj->vMax.z);
	vPoint[6] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMin.y,  pObj->vMin.z);
	vPoint[7] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMin.y,  pObj->vMin.z);

	vPoint[8] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMax.y,  pObj->vMax.z);
	vPoint[9] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMax.y,  pObj->vMin.z);
	vPoint[10] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMin.y,  pObj->vMax.z);
	vPoint[11] = D3DXVECTOR3( pObj->vMin.x,  pObj->vMin.y,  pObj->vMin.z);


	vPoint[12] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMax.y,  pObj->vMax.z);
	vPoint[13] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMax.y,  pObj->vMin.z);
	vPoint[14] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMin.y,  pObj->vMax.z);
	vPoint[15] = D3DXVECTOR3( pObj->vMax.x,  pObj->vMin.y,  pObj->vMin.z);

	m_pLine.Draw(m_pImmediateContext, vPoint[0], vPoint[1], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[1], vPoint[3], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[2], vPoint[3], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[0], vPoint[2], vColor);
		   
	m_pLine.Draw(m_pImmediateContext, vPoint[4], vPoint[5], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[5], vPoint[7], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[6], vPoint[7], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[4], vPoint[6], vColor);
		   
	m_pLine.Draw(m_pImmediateContext, vPoint[8], vPoint[9], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[9], vPoint[11], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[10], vPoint[11], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[8], vPoint[10], vColor);
		   
	m_pLine.Draw(m_pImmediateContext, vPoint[12], vPoint[13], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[13], vPoint[15], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[14], vPoint[15], vColor);
	m_pLine.Draw(m_pImmediateContext, vPoint[12], vPoint[14], vColor);

}


DrawHelper::DrawHelper()
{
	
}


DrawHelper::~DrawHelper()
{
}
