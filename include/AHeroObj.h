#pragma once
#include "ACharMgr.h"

#include "ACamera.h"

#define MAX_BONE_MATRICES 255

#define HERO_IDLE L"HERO_IDLE.Mat" 


struct CBConstBoneWorld
{
	D3DXMATRIX    g_mConstBoneWorld[MAX_BONE_MATRICES];
};



class AHeroObj : public AModel
{
public:
	//bool obbCollide			 = false;
	//ACamera *				m_PlayerCamera;
	int						m_iObjectIndex;
	ACharacter*				m_pChar;
	T_STR					m_szName;
	ABoneObj*				m_pBoneObject;
	bool					m_bConstantBufferType;
	int						m_iStartFrame;
	int						m_iLastFrame;
	int						m_iCurrentFrame;
	float					m_fFrame;
	float					m_fLerpTime;
	float					m_fSpeed;
	D3DXMATRIX				m_pMatrix[255];
	CBConstBoneWorld		m_cbBoneData;
	ComPtr<ID3D11Buffer>    m_pCBConstBoneWorld;
	int						m_iMatrixIndex;
	bool					m_bBoneRender;

public:
//	bool		aabbCollide = false;
	//void		CheckAABB(A_Box targetBox);
	//void		CheckOBBInPlane(A_Box&  pBox);

	bool		SetANIM(const TCHAR* szName);
	void		SetBoneMatrices(vector<D3DXMATRIX>* pList);
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();

	void		SetActionFrame(int iStart, int iEnd, int iCharacterMatrixIndex = -1, int iModelMatrixIndex = -1);
	bool		CreateConstantBuffer();


	//bool		SetPlayerCharacter(const TCHAR* fileName);// , D3DXVECTOR3 minBox, D3DXVECTOR3 maxBox);

	bool		SetPlayerCharacter(const TCHAR* fileName, float x=0,float y=0, float z=0);

	D3DXVECTOR3 GetPlayerPos();
	INT GetPlayerPosX();
	INT GetPlayerPosY();
	INT GetPlayerPosZ();

//	bool CollRect_Rect(A_Box& Dest, A_Box& Src);
	bool	RenderInstancing(ID3D11DeviceContext* pContext);
public:
	AHeroObj();
	virtual ~AHeroObj();
};




