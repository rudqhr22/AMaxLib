#include "Sample.h"
#include "AObjMgr.h"
#include "ACharMgr.h"


bool		Sample::Init()
{
	m_Camera.SetViewMatrix(D3DXVECTOR3(0.0f, 200.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));


	I_CHARMGR.Init();

	if (!I_CHARMGR.Load(m_pd3dDevice.Get(), m_pImmediateContext.Get(), _T("CharacterTable.cit")))
	{
		return false;
	}




	m_CharObj.SetPlayerCharacter(L"TEST_CHAR0");

	m_State = IDLE;

	I_DRAW.DrawInit();

	return true;
}

bool		Sample::Frame()
{
m_CharObj.Frame();




	return true;
}



bool		Sample::Render()
{//
	m_CharObj.SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_CharObj.Render(m_pImmediateContext.Get());



	//m_zombie.SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	//m_zombie.RenderInstancing(m_pImmediateContext.Get());

	

	
	//I_DRAW.BoxLine(&m_CharObj.m_CollsionBox, m_pImmediateContext.Get(), m_pMainCamera);
	//I_DRAW.BoxLine(&m_zombie.m_CollsionBox, m_pImmediateContext.Get(), m_pMainCamera);

	return true;
}



bool		Sample::Release()
{
	I_CHARMGR.Release();
	m_CharObj.Release();

	return true;
}



Sample::Sample()
{
}

Sample::~Sample()
{
}




GAME_INT
GAME_RUN
GAME_END

