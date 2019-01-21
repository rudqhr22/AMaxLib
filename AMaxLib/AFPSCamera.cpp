#include "AFPSCamera.h"
#include "AInput.h"


void AFPSCamera::SetCameraTarget(AModel* pTarget)
{
	m_pShape = pTarget;
}

bool	AFPSCamera::Frame()
{
	SetViewMatrix(D3DXVECTOR3(m_pShape->m_matWorld._41, m_pShape->m_matWorld._42 + 60, m_pShape->m_matWorld._43 - 30.0f),
		D3DXVECTOR3(m_pShape->m_matWorld._41, m_pShape->m_matWorld._42, m_pShape->m_matWorld._43),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	SetProjMatrix((float)D3DX_PI * 0.25f, (FLOAT)g_iClientWidth / (FLOAT)g_iClientHeight, 1.0f, 1000.0f);
	
	
	if (I_INPUT.KeyCheck(DIK_A))
	{
		m_pShape->m_matWorld._41 -= I_TIME.GetSPF() * 20;
	}

	if (I_INPUT.KeyCheck(DIK_D))
	{
		m_pShape->m_matWorld._41 += I_TIME.GetSPF() * 20;
	}

	if (I_INPUT.KeyCheck(DIK_W))
	{
		m_pShape->m_matWorld._43 += I_TIME.GetSPF() * 20;
	}

	if (I_INPUT.KeyCheck(DIK_S))///////////////////////////////////////////////////////////////////
	{
		m_pShape->m_matWorld._43 -= I_TIME.GetSPF() * 20;
	}


	//Update(D3DXVECTOR4(m_fCameraPitchAngle, m_fCameraYawAngle, m_fCameraRollAngle, fDistance));
	//UpdateVector();

	return true;
}


AFPSCamera::AFPSCamera()
{

	SetProjMatrix((float)D3DX_PI * 0.25f, (FLOAT)g_iClientWidth / (FLOAT)g_iClientHeight, 1.0f, 1000.0f);

}


AFPSCamera::~AFPSCamera()
{
}
