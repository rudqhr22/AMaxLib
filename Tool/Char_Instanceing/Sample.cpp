#include "Sample.h"


int fun()
{
	return 0;
}


bool		Sample::Init()
{


	m_Camera.SetViewMatrix(D3DXVECTOR3(0.0f, 200.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	I_CHARMGR.Init();

	if (!I_CHARMGR.Load(m_pd3dDevice.Get(), m_pImmediateContext.Get(), _T("CharacterTable.cit")))
	{
		return false;
	}

	testobj.SetPlayerCharacter(L"guard");
	testobj.CreateInstance(m_pd3dDevice.Get(),50);





	//testobj.SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	//testobj.RenderInstancing(m_pImmediateContext.Get());
	//m_pDeferredContext->FinishCommandList(!m_bClearStateUponFinishCommandList,m_pCommandList.GetAddressOf());
	return true;
}

bool		Sample::Frame()
{

	testobj.Frame();
	

	if(I_INPUT.KeyCheck(DIK_Y) == KEY_PUSH)
	{
		int a = 0;
	}
	if (I_INPUT.KeyCheck(DIK_Y) == KEY_HOLD)
	{
		int a = 0;
	}
	if (I_INPUT.KeyCheck(DIK_Y) == KEY_FREE)
	{
		int a = 0;
	}

	return true;
}



bool		Sample::Render()
{


	testobj.SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	testobj.RenderInstancing(m_pImmediateContext.Get());

	//testobj.Render(m_pDeferredContext.Get());
//	m_pDeferredContext->FinishCommandList(m_pCommandList);

	return true;
}



bool		Sample::Release()
{

	return true;
}



Sample::Sample()
{
}

Sample::~Sample()
{
}



int WINAPI wWinMain(HINSTANCE hN, HINSTANCE hP, LPWSTR lpCmd, int nCmd)
{
	Sample core; 	
	WCHAR filName[] = L"KKB";	
	LPWSTR title = filName;

	if (core.SetWindow(hN, title, 1024, 768)) 
	{
		core.GameRun();

	}
}

//
//GAME_INT
//GAME_RUN
//GAME_END
//
