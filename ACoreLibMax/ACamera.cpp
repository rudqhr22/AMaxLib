#include "ACamera.h"

//ī�޶� Ŭ���� -> ������� ��ü�ϰڴ�
//�����, ��������� ���� ��ũ �Ѵ�.

//���Ϸ� ������ ó���� �Ѱ��, ����� �������� ������ �߻� �� ���� �ִ�.
//y,x,z  f

//D3DXMatrixRotationY(,���Ȱ�);
//D3DXMatrixLookAtLH		 �� ����� ������.
//D3DXMatrixPerspectiveFovLH ���� ��������� �����.



bool ACamera::SetViewMatrix(D3DXVECTOR3 vPos,D3DXVECTOR3 vTarget,D3DXVECTOR3 vUp)
{
	m_vDefaultEye= m_vPos = vPos;					 // ī�޶� ��ġ
	m_vDefaultLookAt = m_vTarget = vTarget;			 // ī�޶� Ÿ��
	m_vUp = vUp;									 // UP ����
	
	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &m_vUp);				

	D3DXQUATERNION qRotation;
	D3DXVECTOR3 vTrans, vScale, vAxis;
	D3DXMATRIX mInvView;	
	D3DXQUATERNION q;

	float fAngle;

	if (SUCCEEDED(D3DXMatrixDecompose(&vScale, &qRotation, &vTrans, &m_matView)))
	{
		D3DXQuaternionNormalize(&qRotation, &qRotation);
		D3DXQuaternionToAxisAngle(&qRotation, &vAxis, &fAngle);
		D3DXVec3Normalize(&vAxis, &vAxis);		// �ݵ�� ����ȭ �ؾ� �Ѵ�.

		q.x = sin(fAngle / 2) * vAxis.x;
		q.y = sin(fAngle / 2) * vAxis.y;
		q.z = sin(fAngle / 2) * vAxis.z;
		q.w = cos(fAngle / 2);
	}
	//������� �ؾ� ������� ���������.-> ���� ī�޶� ��ǥ
	D3DXMatrixInverse(&mInvView, NULL, &m_matView);		
	
	//Z�� 31����
	D3DXVECTOR3* pZBasis = (D3DXVECTOR3*)&mInvView._31;		//D3DX 31.x,31.y,31.z
	
	//Yaw�� 
	m_fCameraYawAngle = atan2f(pZBasis->x, pZBasis->z);		
	
															
	//Pitch��   //����
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fCameraPitchAngle = -atan2f(pZBasis->y, fLen);	//-�� �پ�� �Ѵ�.



	//������� ���Ϸ� �� ���
	//m_fCameraYawAngle = atan2f(pZBasis->x, pZBasis->z);
	//float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	//m_fCameraPitchAngle = -atan2f(pZBasis->y, fLen);	//-�� �پ�� �Ѵ�.


	//KFrustem::SetMatrix(NULL, &m_matView, &m_matProj);
	return UpdateVector();
}

//ī�޶� ���� ����
D3DXMATRIX ACamera::UpdateVector()
{
	//17���� 	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &m_vUp);			��ġ�� ��?

	//look ����
	m_vLook.x = m_matView._13;	//z
	m_vLook.y = m_matView._23;	//z
	m_vLook.z = m_matView._33;	//z

	//side ����
	m_vSide.x = m_matView._11;	//x
	m_vSide.y = m_matView._21;	//x
	m_vSide.z = m_matView._31;	//x

	//Up ����
	m_vUp.x = m_matView._12;	//y
	m_vUp.y = m_matView._22;	//y
	m_vUp.z = m_matView._32;	//y

	D3DXVec3Normalize(&m_vLook, &m_vLook);

	AFrustum::SetMatrix(NULL, &m_matView, &m_matProj);
	AFrustum::CreateFrustum();

	return m_matView;
}

D3DXMATRIX ACamera::SetObjectView(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin)
{
	D3DXMATRIX	matView;
	//D3DXVECTOR3 vCenter = (vMax + vMin) * 0.5f;
	//float fRadius = D3DXVec3Length(&(vMax - vMin)) * 0.5f;

	//D3DXVECTOR3 vTarget = D3DXVECTOR3(vCenter.x, vCenter.y, vCenter.z);
	//D3DXVECTOR3 vPos = (m_vLook *  (fRadius * 2)) * -1.0f;

	//D3DXMatrixLookAtLH(&m_matView, &vPos, &vTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//m_vPos = vPos;
	//m_vTarget = vTarget;

	return m_matView;
}



D3DXMATRIX ACamera::SetProjMatrix(FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane, FLOAT fFarPlane)
{
	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	//���� ��������� �����. x (-1~1) y (-1~1) z (0~1)
	D3DXMatrixPerspectiveFovLH(&m_matProj, 
		m_fFOV,		  //�þ߰� : 
		m_fAspect,    //��Ⱦ�� : ���ο� ���� ������ �ٸ��� ������, ��ȸ�� ���ؼ�  ���� ���� ������ ���� �� �ִ�.
		m_fNearPlane, //�ٸ��, ī�޶� �ٶ󺼼� �ִ� �ּҰŸ�
		m_fFarPlane); //ī�޶� �ٶ󺼼� �ִ� �ִ�Ÿ�

	return m_matProj;
}

D3DXMATRIX ACamera::Update(D3DXVECTOR4 vDirValue)
{
	D3DXMATRIX matRotation;
	//���Ϸ����� �̿�, ������� ȸ�� �ۿ� ����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	D3DXQuaternionRotationYawPitchRoll(&m_qRotation,vDirValue.y,vDirValue.x,vDirValue.z);	//ȸ��������� ������.
	
	m_vPos += m_vLook * vDirValue.w;
	m_fRadius += vDirValue.w;

	//������� ȸ���� �̵��� ���� �Ҽ��� ���� �׷��� �Ʒ� �Լ��� ����Ѵ�.
	//������� ���ؼ� ��ķ� ��ȯ���ִ� �Լ�, ������� ȸ���� �̵��� �ѹ��� ó�� ���ټ� �ִ� �Լ�
	//ȸ���� �߽��� �ٲܼ� �ִ�,
	//������� ���ȭ�ؼ� �Ѱ��ش�
	D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &m_qRotation, &m_vPos);	  //ȸ�� , �̵� 


	//ī�޶��� �ݴ�� �����̱� ������ ������� ����.
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);

	return UpdateVector();
}



bool ACamera::Frame()
{
	//if (g_InputData.bSpace)
	//{
	//	m_fSpeed += 10.0f * g_fSPF;
	//}
	//else
	//{
	//	m_fSpeed -= 10.0f * g_fSPF;
	//	if (m_fSpeed < 1.0f) m_fSpeed = 1.0f;
	//}
	//if (g_InputData.bWKey)
	//{
	//	m_vPos += m_vLook * m_fSpeed * 10.0f * g_fSPF;
	//}
	//if (g_InputData.bSKey)
	//{
	//	m_vPos += -m_vLook * m_fSpeed *10.0f * g_fSPF;
	//}
	//if (g_InputData.bAKey)
	//{
	//	m_vPos += -m_vSide * m_fSpeed *10.0f * g_fSPF;
	//}
	//if (g_InputData.bDKey)
	//{
	//	m_vPos += +m_vSide * m_fSpeed *10.0f * g_fSPF;
	//}
	//if (g_InputData.bLeftHold)
	//{
	//	m_fPitch += D3DXToRadian(g_InputData.iMouseValue[1] * 0.1f);
	//	m_fYaw += D3DXToRadian(g_InputData.iMouseValue[0] * 0.1f);
	//	//m_fRoll += I_Input.m_MouseState.lZ;
	//	// mat = y Matrix * x Matrix * z Matrix;
	//	D3DXMATRIX matRotation;
	//	D3DXQUATERNION qRotation;
	//	D3DXQuaternionRotationYawPitchRoll(	&qRotation, m_fYaw, m_fPitch, m_fRoll);
	//	
	//	D3DXMatrixRotationQuaternion(&matRotation,			&qRotation);
	//	/*	D3DXMATRIX yRot, xRot;
	//	D3DXMatrixRotationY(&yRot, m_fYaw);
	//	D3DXMatrixRotationX(&xRot, m_fPitch);
	//	matRotation = yRot * xRot;*/
	//	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	//}
	////D3DXMatrixLookAtLH(&m_matView,
	////	&m_vPos, // ī�޶� ��ġ
	////	&m_vTarget, // ī�޶� Ÿ��
	////	&m_vUp);
	
	//m_vLook.x = m_matView._13;
	//m_vLook.y = m_matView._23;
	//m_vLook.z = m_matView._33;
	//m_vSide.x = m_matView._11;
	//m_vSide.y = m_matView._21;
	//m_vSide.z = m_matView._31;
	//m_vUp.x = m_matView._12;
	//m_vUp.y = m_matView._22;
	//m_vUp.z = m_matView._32;
	//D3DXVec3Normalize(&m_vLook, &m_vLook);
	//D3DXVec3Normalize(&m_vSide, &m_vSide);
	//D3DXVec3Normalize(&m_vUp, &m_vUp);
	
	//m_matView._41 = -D3DXVec3Dot(&m_vPos, &m_vSide);
	//m_matView._42 = -D3DXVec3Dot(&m_vPos, &m_vUp);
	//m_matView._43 = -D3DXVec3Dot(&m_vPos, &m_vLook);
	//Update(D3DXVECTOR4(m_fPitch, m_fYaw, m_fRoll, m_fRadius));
	//UpdateVector();


	if (g_InputData.bLeftHold)
	{
		m_fCameraYawAngle += D3DXToRadian(g_InputData.iMouseValue[0] * 0.1f);
		m_fCameraPitchAngle += D3DXToRadian(g_InputData.iMouseValue[1] * 0.1f);
	}

	// ī�޶� �̵� �Ÿ� = �ӵ� + ( ����ð� * ���콺 �� ������ )
	float fValue = g_InputData.iMouseValue[2];
	float fDistance = m_fSpeed * fValue * g_fSPF;
	
	if (g_InputData.bSpace)	m_fSpeed += g_fSPF * 10.0f;
	else						m_fSpeed -= g_fSPF * 10.0f;
	// �ּҰ����� ����
	if (m_fSpeed < 1.0f) m_fSpeed = 1.0f;

	if (g_InputData.bWKey)		MoveLook(g_fSPF * 5.0f * m_fSpeed);
	if (g_InputData.bSKey)		MoveLook(-g_fSPF * 5.0f* m_fSpeed);
	if (g_InputData.bDKey)		MoveSide(g_fSPF * 5.0f* m_fSpeed);
	if (g_InputData.bAKey)		MoveSide(-g_fSPF * 5.0f* m_fSpeed);
	if (g_InputData.bQKey)		MoveUp(g_fSPF * 5.0f* m_fSpeed);
	if (g_InputData.bEKey)		MoveUp(-g_fSPF * 5.0f* m_fSpeed);


	Update(D3DXVECTOR4(m_fCameraPitchAngle, m_fCameraYawAngle, m_fCameraRollAngle, fDistance));
	UpdateVector();

	return true;
}

void ACamera::MoveLook(float fValue)
{
	m_vPos += m_vLook * fValue;
}
void ACamera::MoveSide(float fValue)
{
	m_vPos += m_vSide * fValue;
}
void ACamera::MoveUp(float fValue)
{
	m_vPos += m_vUp * fValue;
}


void ACamera::SetRadius(float fDefaultRadius, float fMinRadius, float fMaxRadius)
{
	m_fDefaultRadius = m_fRadius = fDefaultRadius;
	m_fMinRadius = fMinRadius;
	m_fMaxRadius = fMaxRadius;
}

ACamera::ACamera()
{
	m_fCameraYawAngle=0.0f;
	m_fCameraPitchAngle= 0.0f;
	m_fCameraRollAngle= 0.0f;

	m_fPitch = 0.0f;
	m_fYaw = 0.0f;
	m_fRoll = 0.0f;
	m_fSpeed = 0.0f;


	SetViewMatrix(D3DXVECTOR3(0.0f, 0.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	SetProjMatrix(D3DX_PI * 0.25, (float)g_iClientWidth / (float)g_iClientHeight, 0.1f, 1000.0f);
}


ACamera::~ACamera()
{
}
