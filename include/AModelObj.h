#pragma once
#include "AObjMgr.h"
#include "AAseObj.h"

class AModelObj : public AModel
{
public:
	vector<ObjWM*>				m_pModelList;
	vector<AModelObj*>			m_pSubObjList;
	bool						m_bDefferedMultiThread;
	AAnimation					m_Animation;

public:
	void		Add(ID3D11Device* pDevice, AModelObj* pObj);
	bool		Init();
	bool		Load(ID3D11Device* pd3dDevice, const TCHAR* strFileName, const TCHAR* strShaderName, bool bThread = false);
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Draw(ID3D11DeviceContext* pContext, ObjWM* pUint, bool bCommand);
	virtual bool		PreDraw(ID3D11DeviceContext*    pContext, ObjWM* pUnit, bool bCommand);
	virtual bool		PostDraw(ID3D11DeviceContext*    pContext, ObjWM* pUnit, bool bCommand);
	bool		Release();

	void		SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd);
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	// 1���� �޽��� �ټ��� ������Ʈ�� ����� ��쿡 ���ȴ�.
	bool		ObjectFrame();
	bool		ObjectRender(ID3D11DeviceContext*    pContext, bool bCommand = false);
	bool		Set(ID3D11Device* pd3dDevice, int iIndex);
	void		SetConstantBuffers(ID3D11DeviceContext* pContext, ObjWM* pUint, int iMesh);
	//bool		SetCommandRender(ID3D11DeviceContext* pContext);
	//bool		CommandRender(ID3D11DeviceContext* pContext);
	//bool		CommandDraw(ID3D11DeviceContext* pContext, ObjWM* pUnit);
	//virtual HRESULT	SetCommmandList(ID3D11DeviceContext* pContext, AMesh* pSubMesh, bool bSave = false);
	//virtual void	ExecuteCommandList(ID3D11DeviceContext* pContext, AMesh* pSubMesh, bool bClear = true);


public:
	AModelObj(void);
	AModelObj(int iIndex);
	//explicit AModelObj(AModelObj* myStuff)
	//{
	//	m_pSubObjList.push_back(move(myStuff));
	//}
	explicit AModelObj(AModelObj* myStuff)
	{
		AModelObj* pObj(myStuff);
		m_pSubObjList.push_back(move(pObj));
	}
	//explicit TObject(TObject* const myStuff)
	//{
	//	shared_ptr<TObject> pObj(myStuff);
	//	m_pSubObjList.push_back(move(pObj));
	//}
	AModelObj(AModelObj &obj);
	virtual ~AModelObj(void);
};

