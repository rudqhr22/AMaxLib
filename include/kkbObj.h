//#pragma once
//#include "AAseParser.h"
//#include "AMesh.h"
//#include "ATexture.h"
//
//
//
////MAX kkb ���� 
//class kkbObj : public AModel
//
//{
//private:
//	const TCHAR*					m_FileName;
//public:
//	float					m_fTickSpeed;
//	float					m_fElapseTime;
//	float					fFrameTick = 5;
//	int						m_iMaxVertex = 0;
//	int						m_iMaxIndex = 0;
//
//
//public:
//	AScene				m_Scene;
//	AAseParser			m_Parser;
//
//	vector<AMtrl>		m_Material;			//�ؽ�ó ������ ��� �ִ�.
//	vector<AGeomMesh*>	m_GeomObjList;		//��������Ʈ�� ��� �� ����ü�� �ʿ�
//
//	vector<AMesh*>		m_MeshList;		//��� ������ ���⿡ ��� �ȴ�.
//
//public:
//	bool	Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
//
//
////	bool	Convert(ID3D11Device* pd3dDevice);
////
//	bool	CreateVertexBuffer();
//	bool	CreateIndexBuffer();
////	//bool	CreateConstantBuffer();
////
////
//	bool	LoadTexture(ID3D11Device* device, const TCHAR* fileName);
////
////	bool	Frame();
////	bool	Render(ID3D11DeviceContext* context);
////
////public:
////	//bool	SetMeshFaceList(AGeomMesh* GObj, vector<AMesh*>& MeshList, AMesh* pMesh);
////	bool	SetMeshFaceList(AGeomMesh* GObj, AMesh* pMesh);
////	bool	SetMeshSubFaceList(AGeomMesh* GObj, vector<AMesh*>& MeshList);
////
////	bool	ConvertAnim(AGeomMesh* GObj, AMesh* pMesh);
////	void	AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick);
////	bool	GetAnimationTrack(float fFrameTick, vector<AAnimTrack*> pTrackList, AAnimTrack** ppStartTrack, AAnimTrack** ppEndTrack);
////	AAnimTrack* SetLinkedTrack(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev);
////
////
////	float PlayTrackTick(float fFrameTick, float fStartTick, float fEndTick);
////	AMesh* SearchToCollects(T_STR	m_strParentName);
////
////	//bool SetInputLayout();
////	bool AseRender(ID3D11DeviceContext*    pContext, AModel* pParent);
////	//int GetMapID(AMtrl* pMtrl, int iTexMapType);
////
////////////////////////////////////////////////////////////
//	bool		Frame();
//	bool		Render(ID3D11DeviceContext*    pContext);
//	virtual bool		Draw(ID3D11DeviceContext*    pContext, AModel* pParent);
//////	bool		Release();
//////	bool		ResetResource();
//	////////////////////////////////////////////////
//	//  ������Ʈ �ε� ���� �Լ�
//	////////////////////////////////////////////////
////	int			GetMeshCounter() { return m_pData.size(); }
////	bool		Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
//	//  �� ���� �ε�
//	bool		LoadScene(const TCHAR* strFileName);
////	//  ���͸��� ���� �ε�
//	bool		LoadMaterial();
//	void		LoadMaterialHeader(AMtrl* pMtrl);
//	void		LoadTexMap(AMtrl* pMtrl, T_STR szDirName);
//
////	 ������Ʈ ���� �ε�
//	bool		LoadObject();
//	int			LoadVertexIndex(AMesh* pMesh);
////	//  ���ϸ��̼� ���� �ε�
//	void		LoadAnimation(AMesh* pMesh);
//	bool		LoadAnimationTrack(int iNumTrack, vector<AAnimTrack*>& pTrackList);
//	float		PlayTrackTick(float fFrameTick, float fStartTick, float fEndTick);
//	D3DXMATRIX AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick);
//	bool GetAnimationTrack(float fFrame, vector<AAnimTrack*> pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack);
////	//--------------------------------------------------------------------------------------
////	// ������ ���� ���� ����
////	//--------------------------------------------------------------------------------------
////	// �ε� �� ������ AMesh�� ��ȯ
//	bool		Convert(ID3D11Device* pd3dDevice);
////	// ���͸��� ����
//	bool		SetMaterial();
//	int			GetMapID(AMtrl* pMtrl, int iTexMapType); // �� �ε��� �˻�
////	// ��Ӱ��� ����
//	bool		InheriteCollect();
//	AMesh*		SearchToCollects(T_STR	m_strParentName);
//	AAnimTrack* SetDoublyLinkedList(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev);
//public:
//	kkbObj();
//	virtual ~kkbObj();
//};
//
