#pragma once
#include "ARECT.h"
#include "APOINT.h"
#include "ADxHelperEx.h"
#include "ADxObjStd.h"
#include "ADxState.h"
#include "AAseObjStd.h"
#include "AStd.h"
#include "ADevice.h"

//#include "AMesh.h"
//#include "AAseObj.h"

class AMesh;

#define LEFT false
#define RIGHT true

typedef vector<ARECT>  RECT_ARRAY;

struct AInstatnce
{
	D3DXMATRIX matWorld;
};



struct VS_CBChangesEveryFrame
{
	D3DXMATRIX      matNormal;
	D3DXVECTOR3		vLightPos;
	float			f1;
	D3DXVECTOR3		vEye;
	float			f2;
	D3DXVECTOR3		vEyeDir;
	float			f3;
};

struct VS_CBNeverChanges
{
	D3DXVECTOR4				cb_AmbientLightColor;
	D3DXVECTOR4				cb_DiffuseLightColor;
	D3DXVECTOR4				cb_SpecularLightColor;
};

struct VS_CONSTANT_BUFFER
{
	D3DXMATRIX matWorld;// c0						
	D3DXMATRIX matView;	// c4						
	D3DXMATRIX matProj;	// c8						
	D3DXVECTOR4 vColor; // 12
};


struct CB_ENVIRONMENT
{
	//D3DXMATRIX		matWorld;  	
	D3DXMATRIX		matNormal;
	D3DXVECTOR4		vLightVector;	//빛벡터
	D3DXVECTOR4		vEyePos;		//시선 위치?
	D3DXVECTOR4		vEyeDir;		//시선 방향
};

class AModel 
{
public:
	A_Box                       m_CollsionBox;
	A_Box						m_InitBox;
	A_Sphere                    m_Sphere;
	A_Plane                     m_Plane;
	float						m_fTickSpeed;

	

public:
	D3DXVECTOR3 m_vCenter;
	D3DXVECTOR3 m_vPrevCenter;
	D3DXVECTOR3 m_vMove;
	D3DXVECTOR3 m_vLook; // Z
	D3DXVECTOR3 m_vUp; // Y
	D3DXVECTOR3 m_vSide; // X

	//D3DXVECTOR3 m_vPos;
public:

	ComPtr<ID3D11Device>			m_pd3dDevice;
	ARECT						 m_rtPos;
	DX::ADxObject				 m_dxObj;
	


	VS_CONSTANT_BUFFER			 cbData;

	vector<AMesh*>	m_pMesh;

	//vector<PT_VERTEX>			 m_PTList;
	//vector<PCT_VERTEX>		 m_PCTList;
	vector<PNCT_VERTEX>			 m_PNCTList;
	vector<DWORD>				 m_IndexList;

	D3DXMATRIX				m_matControlWorld;		//이걸 사용해서 캐릭터 관리
	D3DXMATRIX				m_matWorld;	// c0						
	D3DXMATRIX				m_matView;	// c4						
	D3DXMATRIX				m_matProj;	// c8	


	T_STR				m_szDirName;
	T_STR				m_szName;


	AScene						m_Scene;

	float				m_fElapseTime;
	int					m_iStartFrame;
	int					m_iLastFrame;


	///  Buffer Comine 
	UINT				m_iMaxVertex;
	UINT				m_iMaxIndex;
	UINT				m_iBeginPosVB;
	UINT				m_iBeginPosIB;
	bool				m_bOptimization;
	int					m_iMatrixIndex;

	FILE_TYPE			m_ObjType;
public:
	virtual  bool		Set(ID3D11Device* device, const TCHAR* shaderName, const TCHAR* fileName = 0, bool pass = false); //  float l = 0, float t = 0, float r = 1, float b = 1);
	
																														  //virtual  bool		Set(ID3D11Device* device, const wchar_t* shaderName, const wchar_t* fileName = 0, float l = 0, float t = 0, float r = 1, float b = 1);
	virtual void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

	//virtual bool		CreateVertexData(float l=0, float t=0, float r=1, float b=1);
	virtual bool		SetInputLayout();
	virtual bool		CreateVertexData();

	virtual bool		CreateIndexData();

	virtual bool		CreateVertexBuffer();
	virtual bool		CreateIndexBuffer();
	virtual bool		CreateConstantBuffer();
	virtual bool		UpdateBuffer();
	virtual bool		CompileShader(ID3D11Device* device, const TCHAR* fileName);
//	virtual bool		CreateLayout(ID3D11Device* device);
	//virtual ID3D11ShaderResourceView*		LoadTexture(ID3D11Device* device, const TCHAR* fileName);
	virtual bool		LoadTexture(ID3D11Device* device, const TCHAR* fileName);

	virtual bool		CreatePrimitiveType();
	virtual void		UpdateConstantBuffer(ID3D11DeviceContext* pContext, AModel* pParent=NULL);

	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				Release();

	virtual bool		Render(ID3D11DeviceContext* context);
	virtual bool		PreRender(ID3D11DeviceContext* context);
	virtual bool		PostRender(ID3D11DeviceContext* context);
	virtual HRESULT		TextureInfo(const TCHAR* texture);

	virtual bool		GenPos(ID3D11Device* device, float l, float t, float r, float b) { return true; }
	virtual bool		UpdatePos(ID3D11Device* device) { return true; }
	virtual Vector2		GenPos(float lt, float rb) { return Vector2(0, 0); }
	virtual bool		SetPos( float l, float t, float r, float b) { return true; }
	virtual Vector2		SetPos(float lt, float rb) { return Vector2(0, 0); }

	virtual void		SetMatrixIndex(int iMatrixIndex) { m_iMatrixIndex = iMatrixIndex; }

	virtual  bool	 Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread);



	virtual void				SetCollisionData(D3DXMATRIX& matWorld);

	static vector<AInstatnce>			m_pInstance;
	static ComPtr<ID3D11Buffer>			m_pVBInstance;
	
	virtual HRESULT	CreateInstance(ID3D11Device* m_pd3dDevice, UINT iNumInstance);
	//bool	RenderInstancing(ID3D11DeviceContext* pContext);
public:
	AModel() ;
	virtual ~AModel() ;
};



struct ObjWM
{
	AModel*				m_pModel;
	float				m_fElapseTime;
	AScene				m_Scene;
	vector<D3DXMATRIX>	m_matCalculation;
	vector<D3DXMATRIX>	m_matAnim;
	vector<int>			m_iParent;
	ObjWM(int iNumMesh)
	{
		m_pModel = NULL;
		m_fElapseTime = 0.0f;
		m_matCalculation.resize(iNumMesh);
		m_matAnim.resize(iNumMesh);
		m_iParent.resize(iNumMesh);
		for (int iMesh = 0; iMesh < iNumMesh; iMesh++)
		{
			D3DXMatrixIdentity(&m_matCalculation[iMesh]);
			D3DXMatrixIdentity(&m_matAnim[iMesh]);
			m_iParent[iMesh] = -1;
		}
	}
};